#ifndef LIVRO_H
#define LIVRO_H

#define MAX_LIVROS 100

// Estrutura que representa um livro
typedef struct {

    int codigo;

    char titulo[100];

    char autor[100];

    int ano;

    int quantidade;

} Livro;

// Cadastro e listagem
void cadastrarLivro(Livro livros[], int *total);

void listarLivros(Livro livros[], int total);

// Busca
void buscarLivro(Livro livros[], int total);

// Alterar quantidade
void alterarQuantidade(
    Livro livros[],
    int total,
    int codigo,
    int quantidade
);

// Arquivo
void salvarLivros(Livro livros[], int total);

int carregarLivros(Livro livros[]);

#endif