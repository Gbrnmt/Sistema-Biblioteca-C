#ifndef UTILS_H
#define UTILS_H

// Tamanho maximo dos campos de texto (titulo, autor, nome, curso)
#define TAM_TEXTO 100

// Le um inteiro do teclado entre minimo e maximo. Repete ate ser valido.
int lerInteiro(const char *mensagem, int minimo, int maximo);

// Le um numero real >= minimo. Repete ate ser valido.
double lerReal(const char *mensagem, double minimo);

// Le uma linha de texto nao vazia e sem ';' (o ';' separa os campos do .txt)
void lerTexto(const char *mensagem, char *destino, int tamanho);

// Converte para minuscula (usada nas comparacoes sem diferenciar caixa)
int minuscula(int c);

// Compara dois textos ignorando maiusculas/minusculas (como strcmp)
int compararTexto(const char *a, const char *b);

// Busca manual: retorna 1 se "trecho" aparece dentro de "texto" (ignora caixa)
int contemTexto(const char *texto, const char *trecho);

#endif