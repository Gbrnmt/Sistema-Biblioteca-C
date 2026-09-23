#ifndef LIVRO_H
#define LIVRO_H

// Quantidade máxima de livros que o sistema pode guardar
#define MAX_LIVROS 100

// Estrutura que representa um livro
typedef struct {
    int codigo;
    char titulo[100];
    char autor[100];
    int ano;
    int quantidade;
} Livro;

// Função para cadastrar um livro
void cadastrarLivro(Livro livros[], int *total);

// Função para listar os livros
void listarLivros(Livro livros[], int total);

#endif