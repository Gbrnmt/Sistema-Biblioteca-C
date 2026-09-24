#ifndef USUARIO_H
#define USUARIO_H

#define MAX_USUARIOS 100

typedef struct {
    int matricula;
    char nome[100];
    char curso[100];
} Usuario;

void cadastrarUsuario(Usuario usuarios[], int *total);

void listarUsuarios(Usuario usuarios[], int total);

void buscarUsuario(Usuario usuarios[], int total);

void salvarUsuarios(Usuario usuarios[], int total);

int carregarUsuarios(Usuario usuarios[]);

#endif