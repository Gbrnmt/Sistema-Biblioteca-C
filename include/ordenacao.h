#ifndef ORDENACAO_H
#define ORDENACAO_H

#include "livro.h"

typedef enum {
    ORD_BUBBLE,
    ORD_SELECTION,
    ORD_INSERTION
} MetodoOrdenacao;

const char *nomeMetodo(MetodoOrdenacao metodo);
const char *nomeCriterio(CriterioLivro criterio);

// Compara dois livros pelo criterio (<0, 0, >0 como strcmp)
int compararLivros(const Livro *a, const Livro *b, CriterioLivro criterio);

// Os tres algoritmos (implementados do zero)
void bubbleSort(Livro *v, int n, CriterioLivro criterio);
void selectionSort(Livro *v, int n, CriterioLivro criterio);
void insertionSort(Livro *v, int n, CriterioLivro criterio);

// Ordena um vetor puro (usado tambem no relatorio de desempenho)
void ordenarLivros(Livro *v, int n, MetodoOrdenacao metodo, CriterioLivro criterio);

// Ordena o acervo e atualiza lista->ordenadoPor
void ordenarListaLivros(ListaLivros *lista, MetodoOrdenacao metodo, CriterioLivro criterio);

// Menu interativo "Ordenar acervo"
void menuOrdenarAcervo(ListaLivros *lista);

#endif