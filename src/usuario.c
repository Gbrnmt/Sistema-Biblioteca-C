#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usuario.h"
#include "utils.h"

#define ARQUIVO_USUARIOS_TXT "dados/usuarios.txt"

// ---------------------------------------------------------------
// Memoria dinamica
// ---------------------------------------------------------------

void iniciarListaUsuarios(ListaUsuarios *lista) {
    lista->dados = NULL;
    lista->total = 0;
    lista->capacidade = 0;
}

void liberarListaUsuarios(ListaUsuarios *lista) {
    free(lista->dados);
    iniciarListaUsuarios(lista);
}

// Garante espaco para mais um usuario (dobra a capacidade com realloc).
static int garantirCapacidade(ListaUsuarios *lista) {
    if (lista->total < lista->capacidade) {
        return 1;
    }

    int novaCapacidade = (lista->capacidade == 0) ? 4 : lista->capacidade * 2;
    Usuario *novo = realloc(lista->dados, novaCapacidade * sizeof(Usuario));
    if (novo == NULL) {
        return 0;
    }

    lista->dados = novo;
    lista->capacidade = novaCapacidade;
    return 1;
}

int adicionarUsuario(ListaUsuarios *lista, Usuario novo) {
    if (buscarUsuarioPorMatricula(lista, novo.matricula) != -1) {
        return 0;
    }
    if (!garantirCapacidade(lista)) {
        return -1;
    }

    lista->dados[lista->total] = novo;
    lista->total++;
    return 1;
}
// ---------------------------------------------------------------
// Cadastro, listagem e busca
// ---------------------------------------------------------------

void cadastrarUsuario(ListaUsuarios *lista) {
    Usuario novo;
    memset(&novo, 0, sizeof(novo));

    printf("\n=== CADASTRAR USUARIO ===\n");

    novo.matricula = lerInteiro("Matricula: ", 1, 2000000000);
    if (buscarUsuarioPorMatricula(lista, novo.matricula) != -1) {
        printf("\nEssa matricula ja existe!\n");
        return;
    }

    lerTexto("Nome: ", novo.nome, TAM_TEXTO);
    lerTexto("Curso: ", novo.curso, TAM_TEXTO);

    int resultado = adicionarUsuario(lista, novo);
    if (resultado == 1) {
        printf("\nUsuario cadastrado com sucesso!\n");
    } else if (resultado == 0) {
        printf("\nEssa matricula ja existe!\n");
    } else {
        printf("\nErro: sem memoria para cadastrar o usuario.\n");
    }
}
void listarUsuarios(const ListaUsuarios *lista) {
    if (lista->total == 0) {
        printf("\nNenhum usuario cadastrado.\n");
        return;
    }

    printf("\n=== USUARIOS CADASTRADOS ===\n");
    printf("\n%-12s %-30s %s\n", "Matricula", "Nome", "Curso");
    printf("------------------------------------------------------------------\n");
    for (int i = 0; i < lista->total; i++) {
        printf("%-12d %-30.30s %s\n",
               lista->dados[i].matricula, lista->dados[i].nome, lista->dados[i].curso);
    }
    printf("\nTotal: %d usuario(s)\n", lista->total);
}
// Busca sequencial por matricula
int buscarUsuarioPorMatricula(const ListaUsuarios *lista, int matricula) {
    for (int i = 0; i < lista->total; i++) {
        if (lista->dados[i].matricula == matricula) {
            return i;
        }
    }
    return -1;
}

void buscarUsuario(const ListaUsuarios *lista) {
    if (lista->total == 0) {
        printf("\nNenhum usuario cadastrado.\n");
        return;
    }

    printf("\n=== BUSCAR USUARIO ===\n");
    int matricula = lerInteiro("Digite a matricula: ", 1, 2000000000);

    int i = buscarUsuarioPorMatricula(lista, matricula);
    if (i == -1) {
        printf("\nUsuario nao encontrado.\n");
        return;
    }

    printf("\nUsuario encontrado!\n");
    printf("Matricula: %d\n", lista->dados[i].matricula);
    printf("Nome: %s\n", lista->dados[i].nome);
    printf("Curso: %s\n", lista->dados[i].curso);
}
// ---------------------------------------------------------------
// Arquivo texto
// ---------------------------------------------------------------

int salvarUsuarios(const ListaUsuarios *lista) {
    FILE *arquivo = fopen(ARQUIVO_USUARIOS_TXT, "w");
    if (arquivo == NULL) {
        printf("Erro ao salvar usuarios.\n");
        return 0;
    }

    for (int i = 0; i < lista->total; i++) {
        fprintf(arquivo, "%d;%s;%s\n",
                lista->dados[i].matricula, lista->dados[i].nome, lista->dados[i].curso);
    }

    fclose(arquivo);
    return 1;
}

int carregarUsuarios(ListaUsuarios *lista) {
    FILE *arquivo = fopen(ARQUIVO_USUARIOS_TXT, "r");
    if (arquivo == NULL) {
        return 0;
    }

    char linha[400];
    int lidos = 0;

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        Usuario u;
        memset(&u, 0, sizeof(u));

        if (sscanf(linha, "%d;%99[^;];%99[^\n]", &u.matricula, u.nome, u.curso) == 3) {
            if (adicionarUsuario(lista, u) == 1) {
                lidos++;
            }
        }
    }

    fclose(arquivo);
    return lidos;
}