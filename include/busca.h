#ifndef BUSCA_H
#define BUSCA_H

#include "livro.h"

// Busca binaria por codigo. O vetor PRECISA estar ordenado por CRIT_CODIGO.
// Retorna o indice ou -1.
int buscaBinariaLivroPorCodigo(const Livro *v, int n, int codigo);

// Busca binaria por titulo exato (ignora maiusculas). O vetor PRECISA estar
// ordenado por CRIT_TITULO. Retorna o indice da PRIMEIRA ocorrencia ou -1.
int buscaBinariaLivroPorTitulo(const Livro *v, int n, const char *titulo);

#endif