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