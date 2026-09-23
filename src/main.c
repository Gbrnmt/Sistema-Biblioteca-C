#include <stdio.h>
#include "livro.h"
#include "usuario.h"

int main() {

    // Array que vai armazenar os livros
    Livro livros[MAX_LIVROS];

    // Array que vai armazenar os usuários
    Usuario usuarios[MAX_USUARIOS];

    // Quantidade atual de livros cadastrados
    int totalLivros = 0;

    // Quantidade atual de usuários cadastrados
    int totalUsuarios = 0;

    int opcao;

    // O menu continua aparecendo até o usuário escolher 0
    do {

        printf("\n================================\n");
        printf("    SISTEMA DE BIBLIOTECA\n");
        printf("================================\n");

        printf("1 - Cadastrar livro\n");
        printf("2 - Listar livros\n");
        printf("3 - Cadastrar usuario\n");
        printf("4 - Listar usuarios\n");
        printf("0 - Sair\n");

        printf("================================\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);

        // Verifica qual opção foi escolhida
        switch (opcao) {

            case 1:
                // Chama a função de cadastro de livros
                cadastrarLivro(livros, &totalLivros);
                break;

            case 2:
                // Chama a função de listagem de livros
                listarLivros(livros, totalLivros);
                break;

            case 3:
                // Chama a função de cadastro de usuários
                cadastrarUsuario(usuarios, &totalUsuarios);
                break;

            case 4:
                // Chama a função de listagem de usuários
                listarUsuarios(usuarios, totalUsuarios);
                break;

            case 0:
                printf("\nEncerrando o sistema...\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}