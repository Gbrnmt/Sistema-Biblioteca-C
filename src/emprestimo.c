#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emprestimo.h"
#include "utils.h"

#define ARQUIVO_EMPRESTIMOS_TXT "dados/emprestimos.txt"

// Configuracao da multa (pode ser alterada no menu)
static int prazoDias = PRAZO_PADRAO_DIAS;
static double taxaDiaria = MULTA_PADRAO_POR_DIA;

// ---------------------------------------------------------------
// Memoria dinamica
// ---------------------------------------------------------------

void iniciarListaEmprestimos(ListaEmprestimos *lista) {
    lista->dados = NULL;
    lista->total = 0;
    lista->capacidade = 0;
}

void liberarListaEmprestimos(ListaEmprestimos *lista) {
    free(lista->dados);
    iniciarListaEmprestimos(lista);
}

static int garantirCapacidade(ListaEmprestimos *lista) {
    if (lista->total < lista->capacidade) {
        return 1;
    }

    int novaCapacidade = (lista->capacidade == 0) ? 4 : lista->capacidade * 2;
    Emprestimo *novo = realloc(lista->dados, novaCapacidade * sizeof(Emprestimo));
    if (novo == NULL) {
        return 0;
    }

    lista->dados = novo;
    lista->capacidade = novaCapacidade;
    return 1;
}

int adicionarEmprestimo(ListaEmprestimos *lista, Emprestimo novo) {
    if (!garantirCapacidade(lista)) {
        return -1;
    }

    lista->dados[lista->total] = novo;
    lista->total++;
    return 1;
}

// Proximo id = maior id existente + 1
static int proximoId(const ListaEmprestimos *lista) {
    int maior = 0;
    for (int i = 0; i < lista->total; i++) {
        if (lista->dados[i].id > maior) {
            maior = lista->dados[i].id;
        }
    }
    return maior + 1;
}
// ---------------------------------------------------------------
// Emprestimo e devolucao
// ---------------------------------------------------------------

int registrarEmprestimo(ListaEmprestimos *emprestimos, ListaLivros *livros,
                        const ListaUsuarios *usuarios) {
    printf("\n=== REALIZAR EMPRESTIMO ===\n");

    if (livros->total == 0 || usuarios->total == 0) {
        printf("E preciso ter livros e usuarios cadastrados.\n");
        return 0;
    }

    int matricula = lerInteiro("Matricula do usuario: ", 1, 2000000000);
    int iUsuario = buscarUsuarioPorMatricula(usuarios, matricula);
    if (iUsuario == -1) {
        printf("\nErro: usuario nao encontrado.\n");
        return 0;
    }

    int codigo = lerInteiro("Codigo do livro: ", 1, 2000000000);
    int iLivro = buscarLivroPorCodigo(livros, codigo);
    if (iLivro == -1) {
        printf("\nErro: livro nao encontrado.\n");
        return 0;
    }
    if (livros->dados[iLivro].quantidade <= 0) {
        printf("\nErro: quantidade insuficiente (nenhum exemplar disponivel).\n");
        return 0;
    }

    Emprestimo e;
    memset(&e, 0, sizeof(e));
    e.id = proximoId(emprestimos);
    e.matricula = matricula;
    e.codigoLivro = codigo;
    lerData("Data do emprestimo", &e.dataEmprestimo);
    e.dataPrevista = somarDias(e.dataEmprestimo, prazoDias);
    e.devolvido = 0;

    if (adicionarEmprestimo(emprestimos, e) != 1) {
        printf("\nErro: sem memoria para registrar o emprestimo.\n");
        return 0;
    }
    alterarQuantidade(livros, codigo, -1);

    char prevista[16];
    formatarData(e.dataPrevista, prevista);
    printf("\nEmprestimo #%d registrado: \"%s\" para %s.\n",
           e.id, livros->dados[iLivro].titulo, usuarios->dados[iUsuario].nome);
    printf("Devolver ate %s.\n", prevista);
    return 1;
}
int registrarDevolucao(ListaEmprestimos *emprestimos, ListaLivros *livros) {
    printf("\n=== REALIZAR DEVOLUCAO ===\n");

    if (emprestimos->total == 0) {
        printf("Nenhum emprestimo registrado.\n");
        return 0;
    }

    int matricula = lerInteiro("Matricula do usuario: ", 1, 2000000000);
    int codigo = lerInteiro("Codigo do livro: ", 1, 2000000000);

    // procura um emprestimo em aberto desse usuario com esse livro
    int indice = -1;
    for (int i = 0; i < emprestimos->total; i++) {
        Emprestimo *e = &emprestimos->dados[i];
        if (!e->devolvido && e->matricula == matricula && e->codigoLivro == codigo) {
            indice = i;
            break;
        }
    }
    if (indice == -1) {
        printf("\nErro: nao ha emprestimo em aberto desse livro para esse usuario.\n");
        return 0;
    }

    Emprestimo *e = &emprestimos->dados[indice];
    Data dataDevolucao;
    lerData("Data da devolucao", &dataDevolucao);

    if (diferencaDias(e->dataEmprestimo, dataDevolucao) < 0) {
        printf("\nErro: a devolucao nao pode ser antes do emprestimo.\n");
        return 0;
    }

    e->dataDevolucao = dataDevolucao;
    e->devolvido = 1;
    e->multa = calcularMulta(e->dataPrevista, dataDevolucao);
    alterarQuantidade(livros, codigo, 1);

    printf("\nDevolucao do emprestimo #%d registrada.\n", e->id);
    if (e->multa > 0.0) {
        printf("Devolucao com atraso de %d dia(s). Multa: R$ %.2f\n",
               diferencaDias(e->dataPrevista, dataDevolucao), e->multa);
    } else {
        printf("Devolvido no prazo. Sem multa.\n");
    }
    return 1;
}
// ---------------------------------------------------------------
// Listagens
// ---------------------------------------------------------------

// Texto da situacao do emprestimo, em relacao a data "referencia"
static void descreverSituacao(const Emprestimo *e, Data referencia, char *saida, int tam) {
    if (e->devolvido) {
        if (e->multa > 0.0) {
            snprintf(saida, tam, "Devolvido com atraso (multa R$ %.2f)", e->multa);
        } else {
            snprintf(saida, tam, "Devolvido");
        }
        return;
    }

    int atraso = diferencaDias(e->dataPrevista, referencia);
    if (atraso > 0) {
        snprintf(saida, tam, "ATRASADO %d dia(s) (multa R$ %.2f)", atraso, atraso * taxaDiaria);
    } else {
        snprintf(saida, tam, "Em aberto");
    }
}

static const char *tituloDoLivro(const ListaLivros *livros, int codigo) {
    int i = buscarLivroPorCodigo(livros, codigo);
    return (i == -1) ? "(livro removido)" : livros->dados[i].titulo;
}

static const char *nomeDoUsuario(const ListaUsuarios *usuarios, int matricula) {
    int i = buscarUsuarioPorMatricula(usuarios, matricula);
    return (i == -1) ? "(usuario removido)" : usuarios->dados[i].nome;
}

static void imprimirCabecalho(void) {
    printf("\n%-4s %-16s %-22s %-10s %-10s %s\n",
           "ID", "Usuario", "Livro", "Emprest.", "Prevista", "Situacao");
    printf("------------------------------------------------------------------------------------------\n");
}

static void imprimirEmprestimo(const Emprestimo *e, const ListaLivros *livros,
                               const ListaUsuarios *usuarios, Data referencia) {
    char dataEmp[16], dataPrev[16], situacao[80];
    formatarData(e->dataEmprestimo, dataEmp);
    formatarData(e->dataPrevista, dataPrev);
    descreverSituacao(e, referencia, situacao, sizeof(situacao));

    printf("%-4d %-16.16s %-22.22s %-10s %-10s %s\n",
           e->id, nomeDoUsuario(usuarios, e->matricula),
           tituloDoLivro(livros, e->codigoLivro), dataEmp, dataPrev, situacao);
}

void listarEmprestimos(const ListaEmprestimos *emprestimos, const ListaLivros *livros,
                       const ListaUsuarios *usuarios) {
    if (emprestimos->total == 0) {
        printf("\nNenhum emprestimo registrado.\n");
        return;
    }

    Data hoje = dataHoje();

    printf("\n=== EMPRESTIMOS ===\n");
    imprimirCabecalho();
    for (int i = 0; i < emprestimos->total; i++) {
        imprimirEmprestimo(&emprestimos->dados[i], livros, usuarios, hoje);
    }
    printf("\nTotal: %d emprestimo(s)\n", emprestimos->total);
}
// ---------------------------------------------------------------
// Multa (extra: controle de atrasos)
// ---------------------------------------------------------------

double calcularMulta(Data prevista, Data devolucao) {
    int atraso = diferencaDias(prevista, devolucao);
    if (atraso <= 0) {
        return 0.0;
    }
    return atraso * taxaDiaria;
}

void configurarMulta(void) {
    printf("\n=== CONFIGURAR PRAZO E MULTA ===\n");
    printf("Prazo atual: %d dia(s)\n", prazoDias);
    printf("Multa atual: R$ %.2f por dia de atraso\n", taxaDiaria);

    prazoDias = lerInteiro("Novo prazo de devolucao (dias): ", 1, 365);
    taxaDiaria = lerReal("Nova multa por dia (R$): ", 0.0);

    printf("\nConfiguracao salva: prazo de %d dia(s), multa de R$ %.2f por dia.\n",
           prazoDias, taxaDiaria);
}
// EXTRA: lista os emprestimos em aberto que passaram do prazo
void listarAtrasados(const ListaEmprestimos *emprestimos, const ListaLivros *livros,
                     const ListaUsuarios *usuarios) {
    printf("\n=== EMPRESTIMOS ATRASADOS ===\n");

    Data referencia;
    lerData("Data de referencia", &referencia);

    int encontrados = 0;
    for (int i = 0; i < emprestimos->total; i++) {
        const Emprestimo *e = &emprestimos->dados[i];
        if (!e->devolvido && diferencaDias(e->dataPrevista, referencia) > 0) {
            if (encontrados == 0) {
                imprimirCabecalho();
            }
            imprimirEmprestimo(e, livros, usuarios, referencia);
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("\nNenhum emprestimo atrasado nessa data.\n");
    } else {
        printf("\nTotal: %d emprestimo(s) atrasado(s)\n", encontrados);
    }
}
// EXTRA: historico completo de um usuario
void historicoUsuario(const ListaEmprestimos *emprestimos, const ListaLivros *livros,
                      const ListaUsuarios *usuarios) {
    printf("\n=== HISTORICO DO USUARIO ===\n");

    if (usuarios->total == 0) {
        printf("Nenhum usuario cadastrado.\n");
        return;
    }

    int matricula = lerInteiro("Matricula: ", 1, 2000000000);
    int iUsuario = buscarUsuarioPorMatricula(usuarios, matricula);
    if (iUsuario == -1) {
        printf("\nErro: usuario nao encontrado.\n");
        return;
    }

    printf("\nUsuario: %s (%s)\n", usuarios->dados[iUsuario].nome, usuarios->dados[iUsuario].curso);

    Data hoje = dataHoje();
    int total = 0;
    int emAberto = 0;
    double somaMultas = 0.0;

    for (int i = 0; i < emprestimos->total; i++) {
        const Emprestimo *e = &emprestimos->dados[i];
        if (e->matricula != matricula) {
            continue;
        }
        if (total == 0) {
            imprimirCabecalho();
        }
        imprimirEmprestimo(e, livros, usuarios, hoje);
        total++;
        if (!e->devolvido) {
            emAberto++;
        }
        somaMultas += e->multa;
    }

    if (total == 0) {
        printf("\nEsse usuario ainda nao fez emprestimos.\n");
    } else {
        printf("\nTotal: %d emprestimo(s), %d em aberto, multas cobradas: R$ %.2f\n",
               total, emAberto, somaMultas);
    }
}
// ---------------------------------------------------------------
// Arquivo texto
// formato: id;matricula;codigo;dia;mes;ano (emprestimo);dia;mes;ano (prevista);
//          dia;mes;ano (devolucao);devolvido;multa
// ---------------------------------------------------------------

int salvarEmprestimos(const ListaEmprestimos *lista) {
    FILE *arquivo = fopen(ARQUIVO_EMPRESTIMOS_TXT, "w");
    if (arquivo == NULL) {
        printf("Erro ao salvar emprestimos.\n");
        return 0;
    }

    for (int i = 0; i < lista->total; i++) {
        const Emprestimo *e = &lista->dados[i];
        fprintf(arquivo, "%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%.2f\n",
                e->id, e->matricula, e->codigoLivro,
                e->dataEmprestimo.dia, e->dataEmprestimo.mes, e->dataEmprestimo.ano,
                e->dataPrevista.dia, e->dataPrevista.mes, e->dataPrevista.ano,
                e->dataDevolucao.dia, e->dataDevolucao.mes, e->dataDevolucao.ano,
                e->devolvido, e->multa);
    }

    fclose(arquivo);
    return 1;
}

int carregarEmprestimos(ListaEmprestimos *lista) {
    FILE *arquivo = fopen(ARQUIVO_EMPRESTIMOS_TXT, "r");
    if (arquivo == NULL) {
        return 0;
    }

    char linha[300];
    int lidos = 0;

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        Emprestimo e;
        memset(&e, 0, sizeof(e));

        int campos = sscanf(linha, "%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%lf",
                            &e.id, &e.matricula, &e.codigoLivro,
                            &e.dataEmprestimo.dia, &e.dataEmprestimo.mes, &e.dataEmprestimo.ano,
                            &e.dataPrevista.dia, &e.dataPrevista.mes, &e.dataPrevista.ano,
                            &e.dataDevolucao.dia, &e.dataDevolucao.mes, &e.dataDevolucao.ano,
                            &e.devolvido, &e.multa);

        if (campos == 14 && adicionarEmprestimo(lista, e) == 1) {
            lidos++;
        }
    }

    fclose(arquivo);
    return lidos;
}