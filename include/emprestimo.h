#ifndef EMPRESTIMO_H
#define EMPRESTIMO_H

#include "datas.h"
#include "livro.h"
#include "usuario.h"

#define PRAZO_PADRAO_DIAS 7
#define MULTA_PADRAO_POR_DIA 1.00

typedef struct {
    int id;
    int matricula;
    int codigoLivro;
    Data dataEmprestimo;
    Data dataPrevista;     // data limite para devolver
    Data dataDevolucao;    // so vale se devolvido == 1
    int devolvido;         // 0 = em aberto, 1 = devolvido
    double multa;          // multa cobrada na devolucao (R$)
} Emprestimo;

// Array dinamico de emprestimos (cresce com realloc)
typedef struct {
    Emprestimo *dados;
    int total;
    int capacidade;
} ListaEmprestimos;

void iniciarListaEmprestimos(ListaEmprestimos *lista);
void liberarListaEmprestimos(ListaEmprestimos *lista);

// Adiciona sem interacao (usada ao carregar arquivo). Retorna 1 = ok, -1 = sem memoria
int adicionarEmprestimo(ListaEmprestimos *lista, Emprestimo novo);

// Operacoes principais (interativas). Retornam 1 se alteraram algo, 0 se nao
int registrarEmprestimo(ListaEmprestimos *emprestimos, ListaLivros *livros,
                        const ListaUsuarios *usuarios);
int registrarDevolucao(ListaEmprestimos *emprestimos, ListaLivros *livros);

void listarEmprestimos(const ListaEmprestimos *emprestimos, const ListaLivros *livros,
                       const ListaUsuarios *usuarios);

// EXTRA 1: controle de atrasos e multa
double calcularMulta(Data prevista, Data devolucao);
void listarAtrasados(const ListaEmprestimos *emprestimos, const ListaLivros *livros,
                     const ListaUsuarios *usuarios);
void configurarMulta(void);   // altera prazo (dias) e taxa diaria (R$)

// EXTRA 2: historico por usuario
void historicoUsuario(const ListaEmprestimos *emprestimos, const ListaLivros *livros,
                      const ListaUsuarios *usuarios);

// Arquivo texto (dados/emprestimos.txt)
int salvarEmprestimos(const ListaEmprestimos *lista);
int carregarEmprestimos(ListaEmprestimos *lista);   // retorna quantos leu

#endif