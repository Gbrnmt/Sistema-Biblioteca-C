#include <stdio.h>
#include "livro.h"
#include "usuario.h"

int main() {

    Livro livros[MAX_LIVROS];

    Usuario usuarios[MAX_USUARIOS];

    int totalLivros = 0;
    int totalUsuarios = 0;

    int opcao;

    // Carrega os livros salvos no arquivo
    totalLivros = carregarLivros(livros);

    do {

        printf("\n================================\n");
        printf("    SISTEMA DE BIBLIOTECA\n");
        printf("================================\n");

        printf("1 - Cadastrar livro\n");
        printf("2 - Listar livros\n");
        printf("3 - Buscar livro\n");
        printf("4 - Cadastrar usuario\n");
        printf("5 - Listar usuarios\n");
        printf("0 - Sair\n");

        printf("================================\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);

        switch (opcao) {

            case 1:

                cadastrarLivro(livros, &totalLivros);

                // Salva depois do cadastro
                salvarLivros(livros, totalLivros);

                break;

            case 2:

                listarLivros(livros, totalLivros);

                break;

            case 3:

                buscarLivro(livros, totalLivros);

                break;

            case 4:

                cadastrarUsuario(usuarios, &totalUsuarios);

                break;

            case 5:

                listarUsuarios(usuarios, totalUsuarios);

                break;

            case 0:

                // Salva antes de sair
                salvarLivros(livros, totalLivros);

                printf("\nEncerrando o sistema...\n");

                break;

            default:

                printf("\nOpcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}