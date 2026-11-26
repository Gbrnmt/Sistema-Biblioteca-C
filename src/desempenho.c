#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "desempenho.h"
#include "livro.h"
#include "ordenacao.h"
#include "busca.h"
#include "datas.h"

#define ARQUIVO_RELATORIO "dados/relatorio_desempenho.txt"
#define NUM_TAMANHOS 4
#define NUM_METODOS 3
#define NUM_BUSCAS 500          // codigos sorteados em cada medicao de busca
#define TEMPO_MINIMO_BUSCA 0.05 // segundos: repete as buscas ate passar disso

static const int tamanhos[NUM_TAMANHOS] = {1000, 2500, 5000, 10000};

// Gerador simples de numeros pseudo-aleatorios (sempre a mesma sequencia,
// assim os testes sao repetiveis em qualquer computador)
static unsigned long semente = 12345UL;

static int numeroAleatorio(void) {
    semente = (semente * 1103515245UL + 12345UL) & 0x7fffffffUL;
    return (int)(semente >> 8);
}

// Preenche n livros com codigos de 1..n embaralhados (caso medio para ordenar)
static void gerarLivros(Livro *v, int n) {
    for (int i = 0; i < n; i++) {
        v[i].codigo = i + 1;
    }
    for (int i = n - 1; i > 0; i--) {
        int j = numeroAleatorio() % (i + 1);
        int temp = v[i].codigo;
        v[i].codigo = v[j].codigo;
        v[j].codigo = temp;
    }
    for (int i = 0; i < n; i++) {
        memset(v[i].titulo, 0, TAM_TEXTO);
        memset(v[i].autor, 0, TAM_TEXTO);
        snprintf(v[i].titulo, TAM_TEXTO, "Livro %06d", v[i].codigo);
        snprintf(v[i].autor, TAM_TEXTO, "Autor %d", v[i].codigo % 100);
        v[i].ano = 1900 + v[i].codigo % 120;
        v[i].quantidade = 1;
    }
}

// Tempo (ms) de ordenar uma COPIA do vetor por titulo
static double medirOrdenacao(const Livro *original, int n, MetodoOrdenacao metodo) {
    Livro *copia = malloc(n * sizeof(Livro));
    if (copia == NULL) {
        return -1.0;
    }
    memcpy(copia, original, n * sizeof(Livro));

    clock_t inicio = clock();
    ordenarLivros(copia, n, metodo, CRIT_TITULO);
    double ms = (double)(clock() - inicio) * 1000.0 / CLOCKS_PER_SEC;

    free(copia);
    return ms;
}

// Tempo medio (microssegundos) de UMA busca por codigo: sequencial x binaria.
// Repete as buscas ate passar TEMPO_MINIMO_BUSCA para o relogio conseguir medir.
static int medirBuscas(const Livro *original, int n, double *seqUs, double *binUs) {
    Livro *desordenado = malloc(n * sizeof(Livro));
    Livro *ordenado = malloc(n * sizeof(Livro));
    if (desordenado == NULL || ordenado == NULL) {
        free(desordenado);
        free(ordenado);
        return 0;
    }
    memcpy(desordenado, original, n * sizeof(Livro));
    memcpy(ordenado, original, n * sizeof(Livro));
    insertionSort(ordenado, n, CRIT_CODIGO);

    int alvos[NUM_BUSCAS];
    for (int k = 0; k < NUM_BUSCAS; k++) {
        alvos[k] = original[numeroAleatorio() % n].codigo;
    }

    ListaLivros lista;
    lista.dados = desordenado;
    lista.total = n;
    lista.capacidade = n;
    lista.ordenadoPor = ORDEM_NENHUMA;

    volatile int soma = 0;   // evita que o compilador apague as buscas
    long repeticoes;
    double segundos;
    clock_t inicio;

    // busca sequencial (vetor desordenado)
    repeticoes = 0;
    inicio = clock();
    do {
        for (int k = 0; k < NUM_BUSCAS; k++) {
            soma += buscarLivroPorCodigo(&lista, alvos[k]);
        }
        repeticoes++;
        segundos = (double)(clock() - inicio) / CLOCKS_PER_SEC;
    } while (segundos < TEMPO_MINIMO_BUSCA);
    *seqUs = segundos * 1e6 / ((double)repeticoes * NUM_BUSCAS);

    // busca binaria (vetor ordenado por codigo)
    repeticoes = 0;
    inicio = clock();
    do {
        for (int k = 0; k < NUM_BUSCAS; k++) {
            soma += buscaBinariaLivroPorCodigo(ordenado, n, alvos[k]);
        }
        repeticoes++;
        segundos = (double)(clock() - inicio) / CLOCKS_PER_SEC;
    } while (segundos < TEMPO_MINIMO_BUSCA);
    *binUs = segundos * 1e6 / ((double)repeticoes * NUM_BUSCAS);

    (void)soma;   // usa a variavel (evita o aviso do compilador)

    free(desordenado);
    free(ordenado);
    return 1;
}

// Escreve o relatorio em "saida" (stdout ou arquivo)
static void imprimirRelatorio(FILE *saida, double tempos[][NUM_METODOS],
                              double seq[], double bin[]) {
    char data[16];
    formatarData(dataHoje(), data);

    fprintf(saida, "RELATORIO DE DESEMPENHO - Sistema de Biblioteca (%s)\n", data);
    fprintf(saida, "Dados gerados de forma pseudo-aleatoria (caso medio).\n\n");

    fprintf(saida, "1) Ordenacao por titulo - tempo em milissegundos\n");
    fprintf(saida, "%8s %14s %16s %16s\n", "N", "Bubble", "Selection", "Insertion");
    for (int t = 0; t < NUM_TAMANHOS; t++) {
        fprintf(saida, "%8d %14.3f %16.3f %16.3f\n", tamanhos[t],
                tempos[t][ORD_BUBBLE], tempos[t][ORD_SELECTION], tempos[t][ORD_INSERTION]);
    }

    fprintf(saida, "\n2) Busca por codigo - tempo medio de UMA busca em microssegundos\n");
    fprintf(saida, "%8s %14s %14s %12s\n", "N", "Sequencial", "Binaria", "Ganho");
    for (int t = 0; t < NUM_TAMANHOS; t++) {
        double ganho = (bin[t] > 0.0) ? seq[t] / bin[t] : 0.0;
        fprintf(saida, "%8d %14.3f %14.3f %11.1fx\n", tamanhos[t], seq[t], bin[t], ganho);
    }
    fprintf(saida, "\n");
}

void executarRelatorioDesempenho(void) {
    double tempos[NUM_TAMANHOS][NUM_METODOS];
    double seq[NUM_TAMANHOS];
    double bin[NUM_TAMANHOS];

    printf("\n=== RELATORIO DE DESEMPENHO ===\n");
    printf("Isso pode levar alguns segundos...\n");

    for (int t = 0; t < NUM_TAMANHOS; t++) {
        int n = tamanhos[t];
        Livro *livros = malloc(n * sizeof(Livro));
        if (livros == NULL) {
            printf("Erro: sem memoria para o teste com %d livros.\n", n);
            return;
        }
        gerarLivros(livros, n);

        printf("Testando com %d livros...\n", n);
        for (int m = 0; m < NUM_METODOS; m++) {
            tempos[t][m] = medirOrdenacao(livros, n, (MetodoOrdenacao)m);
        }
        if (!medirBuscas(livros, n, &seq[t], &bin[t])) {
            printf("Erro: sem memoria para as buscas.\n");
            free(livros);
            return;
        }
        free(livros);
    }

    printf("\n");
    imprimirRelatorio(stdout, tempos, seq, bin);

    FILE *arquivo = fopen(ARQUIVO_RELATORIO, "w");
    if (arquivo == NULL) {
        printf("Nao foi possivel gravar %s\n", ARQUIVO_RELATORIO);
        return;
    }
    imprimirRelatorio(arquivo, tempos, seq, bin);
    fclose(arquivo);
    printf("Relatorio salvo em %s\n", ARQUIVO_RELATORIO);
}