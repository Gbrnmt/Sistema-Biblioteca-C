#ifndef USUARIO_H
#define USUARIO_H

#include "utils.h"

typedef struct {
    int matricula;
    char nome[TAM_TEXTO];
    char curso[TAM_TEXTO];
} Usuario;

// Array dinamico de usuarios (cresce com realloc)
typedef struct {
    Usuario *dados;
    int total;
    int capacidade;
} ListaUsuarios;

void iniciarListaUsuarios(ListaUsuarios *lista);
void liberarListaUsuarios(ListaUsuarios *lista);

// Adiciona sem interacao. Retorna 1 = ok, 0 = matricula ja existe, -1 = sem memoria
int adicionarUsuario(ListaUsuarios *lista, Usuario novo);

void cadastrarUsuario(ListaUsuarios *lista);
void listarUsuarios(const ListaUsuarios *lista);

// Busca sequencial por matricula. Retorna o indice ou -1
int buscarUsuarioPorMatricula(const ListaUsuarios *lista, int matricula);

// Menu "Buscar usuario" (pede a matricula e mostra os dados)
void buscarUsuario(const ListaUsuarios *lista);

// Arquivo texto (dados/usuarios.txt)
int salvarUsuarios(const ListaUsuarios *lista);
int carregarUsuarios(ListaUsuarios *lista);   // retorna quantos leu

#endif