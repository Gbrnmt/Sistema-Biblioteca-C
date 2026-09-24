#include <stdio.h>
#include <string.h>
#include "usuario.h"


// Cadastra um novo usuário
void cadastrarUsuario(Usuario usuarios[], int *total) {

    if (*total >= MAX_USUARIOS) {
        printf("\nLimite de usuarios atingido!\n");
        return;
    }

    printf("\n=== CADASTRAR USUARIO ===\n");

    printf("Matricula: ");
    scanf("%d", &usuarios[*total].matricula);

    // Verifica se a matrícula já existe
    for (int i = 0; i < *total; i++) {

        if (usuarios[i].matricula == usuarios[*total].matricula) {

            printf("Essa matricula ja existe!\n");

            return;
        }
    }

    getchar();

    printf("Nome: ");
    fgets(usuarios[*total].nome, 100, stdin);

    usuarios[*total].nome[
        strcspn(usuarios[*total].nome, "\n")
    ] = '\0';

    if (strlen(usuarios[*total].nome) == 0) {

        printf("Nome invalido!\n");

        return;
    }

    printf("Curso: ");
    fgets(usuarios[*total].curso, 100, stdin);

    usuarios[*total].curso[
        strcspn(usuarios[*total].curso, "\n")
    ] = '\0';

    if (strlen(usuarios[*total].curso) == 0) {

        printf("Curso invalido!\n");

        return;
    }

    *total = *total + 1;

    printf("\nUsuario cadastrado com sucesso!\n");
}


// Lista os usuários
void listarUsuarios(Usuario usuarios[], int total) {

    if (total == 0) {

        printf("\nNenhum usuario cadastrado.\n");

        return;
    }

    printf("\n=== USUARIOS CADASTRADOS ===\n");

    for (int i = 0; i < total; i++) {

        printf("\nUsuario %d\n", i + 1);

        printf("Matricula: %d\n", usuarios[i].matricula);
        printf("Nome: %s\n", usuarios[i].nome);
        printf("Curso: %s\n", usuarios[i].curso);
    }
}


// Busca usuário pela matrícula
void buscarUsuario(Usuario usuarios[], int total) {

    if (total == 0) {

        printf("\nNenhum usuario cadastrado.\n");

        return;
    }

    int matricula;

    printf("\n=== BUSCAR USUARIO ===\n");

    printf("Digite a matricula: ");
    scanf("%d", &matricula);

    for (int i = 0; i < total; i++) {

        if (usuarios[i].matricula == matricula) {

            printf("\nUsuario encontrado!\n");

            printf("Matricula: %d\n", usuarios[i].matricula);
            printf("Nome: %s\n", usuarios[i].nome);
            printf("Curso: %s\n", usuarios[i].curso);

            return;
        }
    }

    printf("\nUsuario nao encontrado.\n");
}


// Salva usuários no arquivo
void salvarUsuarios(Usuario usuarios[], int total) {

    FILE *arquivo;

    arquivo = fopen("dados/usuarios.txt", "w");

    if (arquivo == NULL) {

        printf("Erro ao salvar usuarios.\n");

        return;
    }

    for (int i = 0; i < total; i++) {

        fprintf(
            arquivo,
            "%d;%s;%s\n",
            usuarios[i].matricula,
            usuarios[i].nome,
            usuarios[i].curso
        );
    }

    fclose(arquivo);
}


// Carrega usuários do arquivo
int carregarUsuarios(Usuario usuarios[]) {

    FILE *arquivo;

    arquivo = fopen("dados/usuarios.txt", "r");

    if (arquivo == NULL) {

        return 0;
    }

    int total = 0;

    while (
        fscanf(
            arquivo,
            "%d;%99[^;];%99[^\n]\n",
            &usuarios[total].matricula,
            usuarios[total].nome,
            usuarios[total].curso
        ) == 3
    ) {

        total++;

        if (total >= MAX_USUARIOS) {
            break;
        }
    }

    fclose(arquivo);

    return total;
}