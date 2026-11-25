#include <stdio.h>
#include <time.h>
#include "ordenacao.h"
#include "utils.h"

const char *nomeMetodo(MetodoOrdenacao metodo) {
    switch (metodo) {
        case ORD_BUBBLE:    return "Bubble Sort";
        case ORD_SELECTION: return "Selection Sort";
        default:            return "Insertion Sort";
    }
}

const char *nomeCriterio(CriterioLivro criterio) {
    switch (criterio) {
        case CRIT_CODIGO: return "codigo";
        case CRIT_TITULO: return "titulo";
        case CRIT_AUTOR:  return "autor";
        default:          return "ano";
    }
}

int compararLivros(const Livro *a, const Livro *b, CriterioLivro criterio) {
    switch (criterio) {
        case CRIT_CODIGO: return (a->codigo > b->codigo) - (a->codigo < b->codigo);
        case CRIT_TITULO: return compararTexto(a->titulo, b->titulo);
        case CRIT_AUTOR:  return compararTexto(a->autor, b->autor);
        default:          return (a->ano > b->ano) - (a->ano < b->ano);
    }
}
// Bubble Sort: compara vizinhos e troca; o maior "sobe" a cada passada.
// Se uma passada nao trocar nada, o vetor ja esta ordenado.
void bubbleSort(Livro *v, int n, CriterioLivro criterio) {
    for (int i = 0; i < n - 1; i++) {
        int trocou = 0;

        for (int j = 0; j < n - 1 - i; j++) {
            if (compararLivros(&v[j], &v[j + 1], criterio) > 0) {
                Livro temp = v[j];
                v[j] = v[j + 1];
                v[j + 1] = temp;
                trocou = 1;
            }
        }

        if (!trocou) {
            break;
        }
    }
}
// Selection Sort: a cada passo acha o menor do resto e coloca na posicao i.
void selectionSort(Livro *v, int n, CriterioLivro criterio) {
    for (int i = 0; i < n - 1; i++) {
        int menor = i;

        for (int j = i + 1; j < n; j++) {
            if (compararLivros(&v[j], &v[menor], criterio) < 0) {
                menor = j;
            }
        }

        if (menor != i) {
            Livro temp = v[i];
            v[i] = v[menor];
            v[menor] = temp;
        }
    }
}
// Insertion Sort: pega cada elemento e o insere na posicao certa
// dentro da parte da esquerda, que ja esta ordenada.
void insertionSort(Livro *v, int n, CriterioLivro criterio) {
    for (int i = 1; i < n; i++) {
        Livro chave = v[i];
        int j = i - 1;

        while (j >= 0 && compararLivros(&v[j], &chave, criterio) > 0) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = chave;
    }
}

void ordenarLivros(Livro *v, int n, MetodoOrdenacao metodo, CriterioLivro criterio) {
    if (metodo == ORD_BUBBLE) {
        bubbleSort(v, n, criterio);
    } else if (metodo == ORD_SELECTION) {
        selectionSort(v, n, criterio);
    } else {
        insertionSort(v, n, criterio);
    }
}
void ordenarListaLivros(ListaLivros *lista, MetodoOrdenacao metodo, CriterioLivro criterio) {
    ordenarLivros(lista->dados, lista->total, metodo, criterio);
    lista->ordenadoPor = (int)criterio;
}

void menuOrdenarAcervo(ListaLivros *lista) {
    if (lista->total < 2) {
        printf("\nPrecisa de pelo menos 2 livros para ordenar.\n");
        return;
    }

    printf("\n=== ORDENAR ACERVO ===\n");
    printf("Metodo:\n");
    printf("1 - Bubble Sort\n");
    printf("2 - Selection Sort\n");
    printf("3 - Insertion Sort\n");
    MetodoOrdenacao metodo = (MetodoOrdenacao)(lerInteiro("Escolha: ", 1, 3) - 1);

    printf("Ordenar por:\n");
    printf("1 - Codigo\n");
    printf("2 - Titulo\n");
    printf("3 - Autor\n");
    printf("4 - Ano\n");
    CriterioLivro criterio = (CriterioLivro)(lerInteiro("Escolha: ", 1, 4) - 1);

    clock_t inicio = clock();
    ordenarListaLivros(lista, metodo, criterio);
    double ms = (double)(clock() - inicio) * 1000.0 / CLOCKS_PER_SEC;

    printf("\nAcervo ordenado por %s com %s (%.3f ms).\n",
           nomeCriterio(criterio), nomeMetodo(metodo), ms);
}
