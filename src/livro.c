#include <stdio.h>
#include <string.h>
#include "livro.h"

// Função responsável por cadastrar um novo livro
void cadastrarLivro(Livro livros[], int *total) {

    // Verifica se o limite de livros foi atingido
    if (*total >= MAX_LIVROS) {
        printf("Limite de livros atingido!\n");
        return;
    }

    printf("\n=== CADASTRAR LIVRO ===\n");

    // Lê o código do livro
    printf("Codigo: ");
    scanf("%d", &livros[*total].codigo);

    // Limpa o ENTER deixado pelo scanf
    getchar();

    // Lê o título do livro
    printf("Titulo: ");
    fgets(livros[*total].titulo, 100, stdin);

    // Remove o ENTER do final do texto
    livros[*total].titulo[
        strcspn(livros[*total].titulo, "\n")
    ] = '\0';

    // Lê o nome do autor
    printf("Autor: ");
    fgets(livros[*total].autor, 100, stdin);

    // Remove o ENTER do final do texto
    livros[*total].autor[
        strcspn(livros[*total].autor, "\n")
    ] = '\0';

    // Lê o ano de publicação
    printf("Ano: ");
    scanf("%d", &livros[*total].ano);

    // Lê a quantidade disponível
    printf("Quantidade: ");
    scanf("%d", &livros[*total].quantidade);

    // Aumenta a quantidade total de livros cadastrados
    *total = *total + 1;

    printf("\nLivro cadastrado com sucesso!\n");
}


// Função responsável por listar todos os livros
void listarLivros(Livro livros[], int total) {

    // Verifica se não existem livros
    if (total == 0) {
        printf("\nNenhum livro cadastrado.\n");
        return;
    }

    printf("\n=== LIVROS CADASTRADOS ===\n");

    // Percorre todos os livros cadastrados
    for (int i = 0; i < total; i++) {

        printf("\nLivro %d\n", i + 1);

        printf("Codigo: %d\n", livros[i].codigo);
        printf("Titulo: %s\n", livros[i].titulo);
        printf("Autor: %s\n", livros[i].autor);
        printf("Ano: %d\n", livros[i].ano);
        printf("Quantidade: %d\n", livros[i].quantidade);
    }
}