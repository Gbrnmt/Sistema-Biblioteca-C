#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "utils.h"

// Se a linha lida nao terminou em '\n', sobrou texto no buffer: descarta.
static void descartarResto(const char *linha) {
    int i = 0;
    while (linha[i] != '\0') {
        i++;
    }
    if (i > 0 && linha[i - 1] != '\n') {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }
    }
}

// Le uma linha inteira, sem o '\n'. Se a entrada acabar, encerra o programa.
static void lerLinha(char *destino, int tamanho) {
    if (fgets(destino, tamanho, stdin) == NULL) {
        printf("\nEntrada encerrada. Saindo...\n");
        exit(0);
    }
    descartarResto(destino);

    int i = 0;
    while (destino[i] != '\0' && destino[i] != '\n') {
        i++;
    }
    destino[i] = '\0';
}

int lerInteiro(const char *mensagem, int minimo, int maximo) {
    char linha[64];

    while (1) {
        printf("%s", mensagem);
        lerLinha(linha, sizeof(linha));

        char *fim;
        long valor = strtol(linha, &fim, 10);
        while (*fim == ' ') {
            fim++;
        }

        if (fim == linha || *fim != '\0') {
            printf("Valor invalido. Digite apenas numeros.\n");
            continue;
        }
        if (valor < minimo || valor > maximo) {
            printf("Digite um valor entre %d e %d.\n", minimo, maximo);
            continue;
        }
        return (int)valor;
    }
}

double lerReal(const char *mensagem, double minimo) {
    char linha[64];

    while (1) {
        printf("%s", mensagem);
        lerLinha(linha, sizeof(linha));

        char *fim;
        double valor = strtod(linha, &fim);
        while (*fim == ' ') {
            fim++;
        }

        if (fim == linha || *fim != '\0') {
            printf("Valor invalido. Use ponto como separador (ex: 1.50).\n");
            continue;
        }
        if (valor < minimo) {
            printf("O valor nao pode ser menor que %.2f.\n", minimo);
            continue;
        }
        return valor;
    }
}

void lerTexto(const char *mensagem, char *destino, int tamanho) {
    while (1) {
        printf("%s", mensagem);
        lerLinha(destino, tamanho);

        int soEspacos = 1;
        int temPontoVirgula = 0;
        for (int i = 0; destino[i] != '\0'; i++) {
            if (destino[i] != ' ') {
                soEspacos = 0;
            }
            if (destino[i] == ';') {
                temPontoVirgula = 1;
            }
        }

        if (soEspacos) {
            printf("Este campo nao pode ficar vazio.\n");
            continue;
        }
        if (temPontoVirgula) {
            printf("Nao use ponto e virgula (;) no texto.\n");
            continue;
        }
        return;
    }
}

int minuscula(int c) {
    return tolower((unsigned char)c);
}

int compararTexto(const char *a, const char *b) {
    while (*a != '\0' && *b != '\0') {
        int ca = minuscula(*a);
        int cb = minuscula(*b);
        if (ca != cb) {
            return ca - cb;
        }
        a++;
        b++;
    }
    return minuscula(*a) - minuscula(*b);
}

int contemTexto(const char *texto, const char *trecho) {
    if (trecho[0] == '\0') {
        return 1;
    }

    for (int i = 0; texto[i] != '\0'; i++) {
        int j = 0;
        while (trecho[j] != '\0' && texto[i + j] != '\0' &&
               minuscula(texto[i + j]) == minuscula(trecho[j])) {
            j++;
        }
        if (trecho[j] == '\0') {
            return 1;
        }
    }
    return 0;
}