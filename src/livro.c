#include <stdio.h>
#include <string.h>
#include "livro.h"

// Cadastra um novo livro
void cadastrarLivro(Livro livros[], int *total) {

    if (*total >= MAX_LIVROS) {
        printf("\nLimite de livros atingido!\n");
        return;
    }

    printf("\n=== CADASTRAR LIVRO ===\n");

    printf("Codigo: ");
    scanf("%d", &livros[*total].codigo);

    // Verifica se o código já existe
    for (int i = 0; i < *total; i++) {
        if (livros[i].codigo == livros[*total].codigo) {
            printf("Esse codigo ja existe!\n");
            return;
        }
    }

    getchar();

    printf("Titulo: ");
    fgets(livros[*total].titulo, 100, stdin);

    livros[*total].titulo[
        strcspn(livros[*total].titulo, "\n")
    ] = '\0';

    printf("Autor: ");
    fgets(livros[*total].autor, 100, stdin);

    livros[*total].autor[
        strcspn(livros[*total].autor, "\n")
    ] = '\0';

    printf("Ano: ");
    scanf("%d", &livros[*total].ano);

    if (livros[*total].ano <= 0) {
        printf("Ano invalido!\n");
        return;
    }

    printf("Quantidade: ");
    scanf("%d", &livros[*total].quantidade);

    if (livros[*total].quantidade < 0) {
        printf("Quantidade invalida!\n");
        return;
    }

    *total = *total + 1;

    printf("\nLivro cadastrado com sucesso!\n");
}


// Lista todos os livros
void listarLivros(Livro livros[], int total) {

    if (total == 0) {
        printf("\nNenhum livro cadastrado.\n");
        return;
    }

    printf("\n=== LIVROS CADASTRADOS ===\n");

    for (int i = 0; i < total; i++) {

        printf("\nLivro %d\n", i + 1);
        printf("Codigo: %d\n", livros[i].codigo);
        printf("Titulo: %s\n", livros[i].titulo);
        printf("Autor: %s\n", livros[i].autor);
        printf("Ano: %d\n", livros[i].ano);
        printf("Quantidade: %d\n", livros[i].quantidade);
    }
}


// Busca livro por titulo ou autor
void buscarLivro(Livro livros[], int total) {

    if (total == 0) {
        printf("\nNenhum livro cadastrado.\n");
        return;
    }

    int opcao;
    char busca[100];

    printf("\n=== BUSCAR LIVRO ===\n");
    printf("1 - Buscar por titulo\n");
    printf("2 - Buscar por autor\n");
    printf("Escolha: ");
    scanf("%d", &opcao);

    getchar();

    printf("Digite o que deseja buscar: ");
    fgets(busca, 100, stdin);

    busca[strcspn(busca, "\n")] = '\0';

    int encontrou = 0;

    for (int i = 0; i < total; i++) {

        if (opcao == 1) {

            if (strstr(livros[i].titulo, busca) != NULL) {

                printf("\nCodigo: %d\n", livros[i].codigo);
                printf("Titulo: %s\n", livros[i].titulo);
                printf("Autor: %s\n", livros[i].autor);
                printf("Ano: %d\n", livros[i].ano);
                printf("Quantidade: %d\n", livros[i].quantidade);

                encontrou = 1;
            }

        } else if (opcao == 2) {

            if (strstr(livros[i].autor, busca) != NULL) {

                printf("\nCodigo: %d\n", livros[i].codigo);
                printf("Titulo: %s\n", livros[i].titulo);
                printf("Autor: %s\n", livros[i].autor);
                printf("Ano: %d\n", livros[i].ano);
                printf("Quantidade: %d\n", livros[i].quantidade);

                encontrou = 1;
            }

        } else {

            printf("\nOpcao invalida!\n");
            return;
        }
    }

    if (!encontrou) {
        printf("\nNenhum livro encontrado.\n");
    }
}


// Altera a quantidade de um livro
void alterarQuantidade(
    Livro livros[],
    int total,
    int codigo,
    int quantidade
) {

    for (int i = 0; i < total; i++) {

        if (livros[i].codigo == codigo) {

            livros[i].quantidade = livros[i].quantidade + quantidade;

            return;
        }
    }
}


// Salva os livros no arquivo
void salvarLivros(Livro livros[], int total) {

    FILE *arquivo;

    arquivo = fopen("dados/livros.txt", "w");

    if (arquivo == NULL) {
        printf("Erro ao salvar livros.\n");
        return;
    }

    for (int i = 0; i < total; i++) {

        fprintf(
            arquivo,
            "%d;%s;%s;%d;%d\n",
            livros[i].codigo,
            livros[i].titulo,
            livros[i].autor,
            livros[i].ano,
            livros[i].quantidade
        );
    }

    fclose(arquivo);
}


// Carrega os livros do arquivo
int carregarLivros(Livro livros[]) {

    FILE *arquivo;

    arquivo = fopen("dados/livros.txt", "r");

    if (arquivo == NULL) {
        return 0;
    }

    int total = 0;

    while (
        fscanf(
            arquivo,
            "%d;%99[^;];%99[^;];%d;%d\n",
            &livros[total].codigo,
            livros[total].titulo,
            livros[total].autor,
            &livros[total].ano,
            &livros[total].quantidade
        ) == 5
    ) {

        total++;

        if (total >= MAX_LIVROS) {
            break;
        }
    }

    fclose(arquivo);

    return total;
}