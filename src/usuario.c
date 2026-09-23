#include <stdio.h>
#include <string.h>
#include "usuario.h"

// Função responsável por cadastrar um usuário
void cadastrarUsuario(Usuario usuarios[], int *total) {

    // Verifica se o limite foi atingido
    if (*total >= MAX_USUARIOS) {
        printf("Limite de usuarios atingido!\n");
        return;
    }

    printf("\n=== CADASTRAR USUARIO ===\n");

    // Lê a matrícula
    printf("Matricula: ");
    scanf("%d", &usuarios[*total].matricula);

    // Limpa o ENTER deixado pelo scanf
    getchar();

    // Lê o nome do usuário
    printf("Nome: ");
    fgets(usuarios[*total].nome, 100, stdin);

    // Remove o ENTER do final do nome
    usuarios[*total].nome[
        strcspn(usuarios[*total].nome, "\n")
    ] = '\0';

    // Lê o curso
    printf("Curso: ");
    fgets(usuarios[*total].curso, 100, stdin);

    // Remove o ENTER do final do curso
    usuarios[*total].curso[
        strcspn(usuarios[*total].curso, "\n")
    ] = '\0';

    // Aumenta a quantidade de usuários
    *total = *total + 1;

    printf("\nUsuario cadastrado com sucesso!\n");
}


// Função responsável por listar os usuários
void listarUsuarios(Usuario usuarios[], int total) {

    // Verifica se existem usuários
    if (total == 0) {
        printf("\nNenhum usuario cadastrado.\n");
        return;
    }

    printf("\n=== USUARIOS CADASTRADOS ===\n");

    // Percorre todos os usuários
    for (int i = 0; i < total; i++) {

        printf("\nUsuario %d\n", i + 1);

        printf("Matricula: %d\n", usuarios[i].matricula);
        printf("Nome: %s\n", usuarios[i].nome);
        printf("Curso: %s\n", usuarios[i].curso);
    }
}