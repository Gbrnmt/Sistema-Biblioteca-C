#include "busca.h"
#include "utils.h"

// Busca binaria: olha o elemento do meio e descarta metade do vetor
// a cada passo (por isso so funciona com o vetor ordenado).
int buscaBinariaLivroPorCodigo(const Livro *v, int n, int codigo) {
    int inicio = 0;
    int fim = n - 1;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;

        if (v[meio].codigo == codigo) {
            return meio;
        } else if (v[meio].codigo < codigo) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    return -1;
}
// Mesma ideia, comparando titulos. Ao achar, continua procurando a esquerda
// para devolver a PRIMEIRA ocorrencia (titulos podem se repetir).
int buscaBinariaLivroPorTitulo(const Livro *v, int n, const char *titulo) {
    int inicio = 0;
    int fim = n - 1;
    int achou = -1;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        int cmp = compararTexto(v[meio].titulo, titulo);

        if (cmp == 0) {
            achou = meio;
            fim = meio - 1;
        } else if (cmp < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    return achou;
}
