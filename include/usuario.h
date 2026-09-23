#ifndef USUARIO_H
#define USUARIO_H

// Quantidade máxima de usuários
#define MAX_USUARIOS 100

// Estrutura que representa um usuário
typedef struct {
    int matricula;
    char nome[100];
    char curso[100];
} Usuario;

// Função para cadastrar usuário
void cadastrarUsuario(Usuario usuarios[], int *total);

// Função para listar usuários
void listarUsuarios(Usuario usuarios[], int total);

#endif