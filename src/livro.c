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