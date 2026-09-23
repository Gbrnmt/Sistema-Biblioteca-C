# Sistema-Biblioteca-C
Sistema de Gerenciamento de Biblioteca Universitária

Projeto da disciplina Programação Estruturada — Prof. Monique Soares.

👥 Divisão de tarefas
Gabriel Nepomuceno

Responsável pelo módulo de Livros e funcionalidades relacionadas ao acervo.

Criar struct Livro

Cadastro de livros

Listagem de livros

Busca sequencial por título

Busca sequencial por autor

Alteração da quantidade disponível

Validação de dados dos livros

Persistência dos livros em arquivo .txt/.csv

Leitura dos livros salvos no arquivo

Tratamento de erros relacionados aos livros

Arquivos principais:

include/livro.h
src/livro.c
dados/livros.txt
Ian

Responsável pelo módulo de Usuários e funcionalidades relacionadas aos usuários da biblioteca.

Criar struct Usuario

Cadastro de usuários

Listagem de usuários

Busca sequencial por matrícula

Validação de dados dos usuários

Persistência dos usuários em arquivo .txt/.csv

Leitura dos usuários salvos no arquivo

Tratamento de erros relacionados aos usuários

Arquivos principais:

include/usuario.h
src/usuario.c
dados/usuarios.txt
🤝 Trabalho em conjunto

As funcionalidades que dependem tanto de livros quanto de usuários serão desenvolvidas em conjunto.

Empréstimos e devoluções

Criar struct Emprestimo

Registrar empréstimo

Registrar devolução

Verificar se o livro existe

Verificar se o usuário existe

Verificar quantidade disponível

Atualizar quantidade de livros após empréstimo

Atualizar quantidade de livros após devolução

Listar empréstimos

Persistência dos empréstimos em arquivo

Tratamento de erros

Arquivos:

include/emprestimo.h
src/emprestimo.c
dados/emprestimos.txt
🖥️ Menu principal

O menu será desenvolvido em conjunto para integrar todos os módulos:

1 - Cadastrar livro
2 - Listar livros
3 - Buscar livro
4 - Cadastrar usuário
5 - Listar usuários
6 - Buscar usuário
7 - Realizar empréstimo
8 - Realizar devolução
9 - Listar empréstimos
0 - Sair

Arquivo:

src/main.c
🌿 Organização das branches

Cada integrante possui sua própria branch:

main
├── gabriel-nepomuceno
└── ian

As funcionalidades serão desenvolvidas nas respectivas branches e posteriormente integradas à main através de Pull Requests/Merge.

📝 Histórico de desenvolvimento

O projeto será desenvolvido de forma incremental, com commits pequenos e significativos, registrando a evolução do sistema ao longo do semestre.

Exemplos:

Cria struct Livro
Implementa cadastro de livros
Implementa listagem de livros
Implementa busca por título
Adiciona persistência de livros
📚 Tecnologias e restrições
Linguagem: C
Biblioteca: somente biblioteca padrão da linguagem C
Interface: terminal
Persistência: arquivos .txt ou .csv
Busca: implementação manual de busca sequencial
Organização: arquivos .h e .c
Controle de versão: Git/GitHub
Trabalho: dupla