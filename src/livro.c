#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "livro.h"
#include "utils.h"

#define ARQUIVO_LIVROS_TXT "dados/livros.txt"

// ---------------------------------------------------------------
// Memoria dinamica
// ---------------------------------------------------------------

void iniciarListaLivros(ListaLivros *lista) {
    lista->dados = NULL;
    lista->total = 0;
    lista->capacidade = 0;
    lista->ordenadoPor = ORDEM_NENHUMA;
}

void liberarListaLivros(ListaLivros *lista) {
    free(lista->dados);
    iniciarListaLivros(lista);
}
// Garante espaco para mais um livro (dobra a capacidade com realloc).
// Retorna 1 se deu certo, 0 se faltou memoria.
static int garantirCapacidade(ListaLivros *lista) {
    if (lista->total < lista->capacidade) {
        return 1;
    }

    int novaCapacidade = (lista->capacidade == 0) ? 4 : lista->capacidade * 2;
    Livro *novo = realloc(lista->dados, novaCapacidade * sizeof(Livro));
    if (novo == NULL) {
        return 0;   // a lista antiga continua valida
    }

    lista->dados = novo;
    lista->capacidade = novaCapacidade;
    return 1;
}

int adicionarLivro(ListaLivros *lista, Livro novo) {
    if (buscarLivroPorCodigo(lista, novo.codigo) != -1) {
        return 0;
    }
    if (!garantirCapacidade(lista)) {
        return -1;
    }

    lista->dados[lista->total] = novo;
    lista->total++;
    lista->ordenadoPor = ORDEM_NENHUMA;   // a ordem pode ter se perdido
    return 1;
}

// ---------------------------------------------------------------
// Cadastro e listagem
// ---------------------------------------------------------------

void cadastrarLivro(ListaLivros *lista) {
    Livro novo;
    memset(&novo, 0, sizeof(novo));

    printf("\n=== CADASTRAR LIVRO ===\n");

    novo.codigo = lerInteiro("Codigo: ", 1, 2000000000);
    if (buscarLivroPorCodigo(lista, novo.codigo) != -1) {
        printf("\nEsse codigo ja existe!\n");
        return;
    }

    lerTexto("Titulo: ", novo.titulo, TAM_TEXTO);
    lerTexto("Autor: ", novo.autor, TAM_TEXTO);
    novo.ano = lerInteiro("Ano: ", 1, 2100);
    novo.quantidade = lerInteiro("Quantidade: ", 0, 100000);

    int resultado = adicionarLivro(lista, novo);
    if (resultado == 1) {
        printf("\nLivro cadastrado com sucesso!\n");
    } else if (resultado == 0) {
        printf("\nEsse codigo ja existe!\n");
    } else {
        printf("\nErro: sem memoria para cadastrar o livro.\n");
    }
}
// ---------------------------------------------------------------
// Impressao (tabela)
// ---------------------------------------------------------------

static void imprimirCabecalho(void) {
    printf("\n%-10s %-30s %-22s %-5s %s\n", "Codigo", "Titulo", "Autor", "Ano", "Qtd");
    printf("------------------------------------------------------------------------------\n");
}

static void imprimirLivro(const Livro *l) {
    printf("%-10d %-30.30s %-22.22s %-5d %d\n",
           l->codigo, l->titulo, l->autor, l->ano, l->quantidade);
}

void listarLivros(const ListaLivros *lista) {
    if (lista->total == 0) {
        printf("\nNenhum livro cadastrado.\n");
        return;
    }

    printf("\n=== LIVROS CADASTRADOS ===\n");
    imprimirCabecalho();
    for (int i = 0; i < lista->total; i++) {
        imprimirLivro(&lista->dados[i]);
    }
    printf("\nTotal: %d livro(s)\n", lista->total);
}