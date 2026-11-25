#include <stdio.h>
#include "utils.h"
#include "livro.h"
#include "usuario.h"
#include "emprestimo.h"
#include "ordenacao.h"
#include "arquivo.h"
#include "desempenho.h"

static void mostrarMenu(void) {
    printf("\n================================\n");
    printf("    SISTEMA DE BIBLIOTECA\n");
    printf("================================\n");
    printf(" 1 - Cadastrar livro\n");
    printf(" 2 - Listar livros\n");
    printf(" 3 - Buscar livro\n");
    printf(" 4 - Cadastrar usuario\n");
    printf(" 5 - Listar usuarios\n");
    printf(" 6 - Buscar usuario\n");
    printf(" 7 - Realizar emprestimo\n");
    printf(" 8 - Realizar devolucao\n");
    printf(" 9 - Listar emprestimos\n");
    printf("--------------------------------\n");
    printf("10 - Ordenar acervo\n");
    printf("11 - Historico de um usuario\n");
    printf("12 - Emprestimos atrasados\n");
    printf("13 - Configurar prazo e multa\n");
    printf("14 - Relatorio de desempenho\n");
    printf(" 0 - Sair\n");
    printf("================================\n");
}

int main(void) {
    ListaLivros livros;
    ListaUsuarios usuarios;
    ListaEmprestimos emprestimos;

    iniciarListaLivros(&livros);
    iniciarListaUsuarios(&usuarios);
    iniciarListaEmprestimos(&emprestimos);

    // Carrega os dados salvos (.bin, ou .txt se ainda nao existir .bin)
    carregarDados(&livros, &usuarios, &emprestimos);

    int opcao;
    do {
        mostrarMenu();
        opcao = lerInteiro("Escolha uma opcao: ", 0, 14);

        switch (opcao) {
            case 1:
                cadastrarLivro(&livros);
                salvarDados(&livros, &usuarios, &emprestimos);
                break;
            case 2:
                listarLivros(&livros);
                break;
            case 3:
                buscarLivro(&livros);
                break;
            case 4:
                cadastrarUsuario(&usuarios);
                salvarDados(&livros, &usuarios, &emprestimos);
                break;
            case 5:
                listarUsuarios(&usuarios);
                break;
            case 6:
                buscarUsuario(&usuarios);
                break;
            case 7:
                if (registrarEmprestimo(&emprestimos, &livros, &usuarios)) {
                    salvarDados(&livros, &usuarios, &emprestimos);
                }
                break;
            case 8:
                if (registrarDevolucao(&emprestimos, &livros)) {
                    salvarDados(&livros, &usuarios, &emprestimos);
                }
                break;
            case 9:
                listarEmprestimos(&emprestimos, &livros, &usuarios);
                break;
            case 10:
                menuOrdenarAcervo(&livros);
                salvarDados(&livros, &usuarios, &emprestimos);
                break;
            case 11:
                historicoUsuario(&emprestimos, &livros, &usuarios);
                break;
            case 12:
                listarAtrasados(&emprestimos, &livros, &usuarios);
                break;
            case 13:
                configurarMulta();
                break;
            case 14:
                executarRelatorioDesempenho();
                break;
            case 0:
                salvarDados(&livros, &usuarios, &emprestimos);
                printf("\nEncerrando o sistema...\n");
                break;
        }
    } while (opcao != 0);

    // Libera a memoria alocada dinamicamente
    liberarListaLivros(&livros);
    liberarListaUsuarios(&usuarios);
    liberarListaEmprestimos(&emprestimos);

    return 0;
}