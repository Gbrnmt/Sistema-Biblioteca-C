#include <stdio.h>
#include <stdlib.h>
#include "arquivo.h"

#define ARQUIVO_LIVROS_BIN "dados/livros.bin"
#define ARQUIVO_USUARIOS_BIN "dados/usuarios.bin"
#define ARQUIVO_EMPRESTIMOS_BIN "dados/emprestimos.bin"
#define MAX_REGISTROS 10000000   // protecao contra arquivo corrompido

// Formato do arquivo binario: um int com a quantidade de registros,
// seguido dos registros (structs) gravados direto com fwrite.

static int salvarBinario(const char *caminho, const void *dados, int total, size_t tamanho) {
    FILE *arquivo = fopen(caminho, "wb");
    if (arquivo == NULL) {
        return 0;
    }

    int ok = (fwrite(&total, sizeof(int), 1, arquivo) == 1);
    if (ok && total > 0) {
        ok = (fwrite(dados, tamanho, total, arquivo) == (size_t)total);
    }

    fclose(arquivo);
    return ok;
}

// Le o arquivo para um vetor novo (malloc). Retorna o total de registros,
// ou -1 se o arquivo nao existe ou esta invalido.
static int carregarBinario(const char *caminho, void **dados, int *capacidade, size_t tamanho) {
    FILE *arquivo = fopen(caminho, "rb");
    if (arquivo == NULL) {
        return -1;
    }

    int total;
    if (fread(&total, sizeof(int), 1, arquivo) != 1 || total < 0 || total > MAX_REGISTROS) {
        fclose(arquivo);
        return -1;
    }

    int cap = (total > 0) ? total : 4;
    void *vetor = malloc(cap * tamanho);
    if (vetor == NULL) {
        fclose(arquivo);
        return -1;
    }

    if (total > 0 && fread(vetor, tamanho, total, arquivo) != (size_t)total) {
        free(vetor);
        fclose(arquivo);
        return -1;
    }

    fclose(arquivo);
    *dados = vetor;
    *capacidade = cap;
    return total;
}
// ---------------------------------------------------------------
// Livros
// ---------------------------------------------------------------

int salvarLivrosBin(const ListaLivros *lista) {
    return salvarBinario(ARQUIVO_LIVROS_BIN, lista->dados, lista->total, sizeof(Livro));
}

int carregarLivrosBin(ListaLivros *lista) {
    void *dados;
    int capacidade;
    int total = carregarBinario(ARQUIVO_LIVROS_BIN, &dados, &capacidade, sizeof(Livro));
    if (total < 0) {
        return -1;
    }

    free(lista->dados);
    lista->dados = dados;
    lista->total = total;
    lista->capacidade = capacidade;
    lista->ordenadoPor = ORDEM_NENHUMA;
    return total;
}

// ---------------------------------------------------------------
// Usuarios
// ---------------------------------------------------------------

int salvarUsuariosBin(const ListaUsuarios *lista) {
    return salvarBinario(ARQUIVO_USUARIOS_BIN, lista->dados, lista->total, sizeof(Usuario));
}

int carregarUsuariosBin(ListaUsuarios *lista) {
    void *dados;
    int capacidade;
    int total = carregarBinario(ARQUIVO_USUARIOS_BIN, &dados, &capacidade, sizeof(Usuario));
    if (total < 0) {
        return -1;
    }

    free(lista->dados);
    lista->dados = dados;
    lista->total = total;
    lista->capacidade = capacidade;
    return total;
}

// ---------------------------------------------------------------
// Emprestimos
// ---------------------------------------------------------------

int salvarEmprestimosBin(const ListaEmprestimos *lista) {
    return salvarBinario(ARQUIVO_EMPRESTIMOS_BIN, lista->dados, lista->total, sizeof(Emprestimo));
}

int carregarEmprestimosBin(ListaEmprestimos *lista) {
    void *dados;
    int capacidade;
    int total = carregarBinario(ARQUIVO_EMPRESTIMOS_BIN, &dados, &capacidade, sizeof(Emprestimo));
    if (total < 0) {
        return -1;
    }

    free(lista->dados);
    lista->dados = dados;
    lista->total = total;
    lista->capacidade = capacidade;
    return total;
}
// ---------------------------------------------------------------
// Carregar / salvar tudo
// ---------------------------------------------------------------

void carregarDados(ListaLivros *livros, ListaUsuarios *usuarios, ListaEmprestimos *emprestimos) {
    // prefere o binario; se nao existir, le o texto
    if (carregarLivrosBin(livros) < 0) {
        carregarLivros(livros);
    }
    if (carregarUsuariosBin(usuarios) < 0) {
        carregarUsuarios(usuarios);
    }
    if (carregarEmprestimosBin(emprestimos) < 0) {
        carregarEmprestimos(emprestimos);
    }
}

void salvarDados(const ListaLivros *livros, const ListaUsuarios *usuarios,
                 const ListaEmprestimos *emprestimos) {
    int ok = 1;

    ok = salvarLivrosBin(livros) && ok;
    ok = salvarUsuariosBin(usuarios) && ok;
    ok = salvarEmprestimosBin(emprestimos) && ok;

    ok = salvarLivros(livros) && ok;
    ok = salvarUsuarios(usuarios) && ok;
    ok = salvarEmprestimos(emprestimos) && ok;

    if (!ok) {
        printf("Aviso: houve erro ao gravar algum arquivo em dados/.\n");
    }
}