#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "livro.h"
#include "usuario.h"
#include "emprestimo.h"

// Arquivos binarios (dados/*.bin). Retornam 1 = ok, 0 = erro ao salvar
int salvarLivrosBin(const ListaLivros *lista);
int salvarUsuariosBin(const ListaUsuarios *lista);
int salvarEmprestimosBin(const ListaEmprestimos *lista);

// Retornam quantos registros leu, ou -1 se o arquivo nao existe / esta invalido
int carregarLivrosBin(ListaLivros *lista);
int carregarUsuariosBin(ListaUsuarios *lista);
int carregarEmprestimosBin(ListaEmprestimos *lista);

// Carrega tudo: usa o .bin se existir, senao cai para o .txt
void carregarDados(ListaLivros *livros, ListaUsuarios *usuarios, ListaEmprestimos *emprestimos);

// Salva tudo nos dois formatos (.bin para carregar, .txt legivel)
void salvarDados(const ListaLivros *livros, const ListaUsuarios *usuarios,
                 const ListaEmprestimos *emprestimos);

#endif