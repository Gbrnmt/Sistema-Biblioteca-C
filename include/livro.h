#ifndef LIVRO_H
#define LIVRO_H

#include "utils.h"

// Um livro do acervo
typedef struct {
    int codigo;
    char titulo[TAM_TEXTO];
    char autor[TAM_TEXTO];
    int ano;
    int quantidade;
} Livro;

// Campos pelos quais o acervo pode ser ordenado
typedef enum {
    CRIT_CODIGO,
    CRIT_TITULO,
    CRIT_AUTOR,
    CRIT_ANO
} CriterioLivro;

#define ORDEM_NENHUMA (-1)

// Array dinamico de livros (cresce com realloc)
typedef struct {
    Livro *dados;
    int total;
    int capacidade;
    int ordenadoPor;   // CriterioLivro atual ou ORDEM_NENHUMA (usado na busca binaria)
} ListaLivros;

// Memoria
void iniciarListaLivros(ListaLivros *lista);
void liberarListaLivros(ListaLivros *lista);

// Adiciona sem interacao. Retorna 1 = ok, 0 = codigo ja existe, -1 = sem memoria
int adicionarLivro(ListaLivros *lista, Livro novo);

// Cadastro e listagem (interativos)
void cadastrarLivro(ListaLivros *lista);
void listarLivros(const ListaLivros *lista);

// Busca sequencial por codigo. Retorna o indice ou -1
int buscarLivroPorCodigo(const ListaLivros *lista, int codigo);

// Menu de busca (titulo, autor, codigo; sequencial e binaria)
void buscarLivro(ListaLivros *lista);

// Soma "delta" a quantidade (use negativo para emprestimo).
// Retorna 1 = ok, 0 = livro nao existe ou quantidade ficaria negativa
int alterarQuantidade(ListaLivros *lista, int codigo, int delta);

// Arquivo texto (dados/livros.txt)
int salvarLivros(const ListaLivros *lista);
int carregarLivros(ListaLivros *lista);   // retorna quantos livros leu

#endif