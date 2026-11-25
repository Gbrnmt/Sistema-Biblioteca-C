# Sistema-Biblioteca-C

Sistema de Gerenciamento de uma Biblioteca Universitária.
Projeto da disciplina **Programação Estruturada** — Prof. Monique Soares.

Equipe: Gabriel e Ian *(coloquem os nomes completos)*

Feito em **C puro**, usando **somente a biblioteca padrão** (`stdio`, `stdlib`, `string`, `ctype`, `time`).
Toda a lógica de busca e ordenação foi implementada do zero (nada de `strstr`, `bsearch` ou `qsort`).

## Como compilar e executar

Execute sempre a partir da **raiz do projeto** (o programa lê e grava em `dados/`).

**Linux / macOS / MSYS2:**

```bash
make            # compila e gera ./biblioteca
./biblioteca
```

Ou sem `make`:

```bash
gcc -Wall -Wextra -Iinclude src/*.c -o biblioteca
./biblioteca
```

**VS Code (Windows):** `Ctrl+Shift+B` roda a tarefa "Compilar Biblioteca" (gera `biblioteca.exe`).

## Menu

| Opção | Função |
|-------|--------|
| 1 | Cadastrar livro |
| 2 | Listar livros |
| 3 | Buscar livro (título, autor, código — sequencial e binária) |
| 4 | Cadastrar usuário |
| 5 | Listar usuários |
| 6 | Buscar usuário (por matrícula) |
| 7 | Realizar empréstimo |
| 8 | Realizar devolução |
| 9 | Listar empréstimos |
| 10 | Ordenar acervo (Bubble, Selection ou Insertion Sort) |
| 11 | Histórico de um usuário *(extra)* |
| 12 | Empréstimos atrasados *(extra)* |
| 13 | Configurar prazo e multa *(extra)* |
| 14 | Relatório de desempenho |
| 0 | Sair |

Em empréstimo, devolução e atrasados, a data pode ser digitada (`dd/mm/aaaa`) ou use `0` para a data de hoje.
Isso permite testar multas sem esperar dias de verdade.

## O que foi implementado

**Parte 1 (escopo mínimo)**
- Cadastro de livros, usuários e empréstimos/devoluções
- Listagens, busca sequencial por título, autor e matrícula
- Persistência em arquivos texto (`dados/*.txt`, campos separados por `;`)
- Menu interativo e tratamento de erros (livro/usuário não encontrado, quantidade insuficiente, entrada inválida)

**Parte 2 (evolução)**
- **Alocação dinâmica:** `ListaLivros`, `ListaUsuarios` e `ListaEmprestimos` são arrays dinâmicos (`malloc`/`realloc`/`free`), que dobram de capacidade quando enchem
- **Ordenação:** Bubble Sort, Selection Sort e Insertion Sort (os três) por código, título, autor ou ano
- **Pesquisa binária:** por código e por título exato; se o acervo não estiver ordenado, ele é ordenado antes
- **Arquivos binários:** `dados/*.bin` (carregados no início; se não existirem, lê o `.txt`)
- **Relatório de desempenho:** opção 14 compara os três métodos de ordenação e busca sequencial × binária (veja `RELATORIO_DESEMPENHO.md`)
- **Extras (2 escolhidos):** controle de atrasos e multa; histórico por usuário

## Estrutura

```
include/   cabeçalhos (.h)
src/       implementações (.c)
dados/     arquivos de dados (.txt de exemplo; .bin gerado pelo programa)
```

| Módulo | Conteúdo |
|--------|----------|
| `utils` | leitura segura do teclado, comparação e busca de texto sem diferenciar maiúsculas |
| `datas` | data de hoje, validação, diferença de dias, soma de dias |
| `livro` | struct `Livro`, lista dinâmica, cadastro, listagem, busca, quantidade, arquivo texto |
| `ordenacao` | Bubble, Selection e Insertion Sort, menu de ordenação |
| `busca` | busca binária por código e por título |
| `desempenho` | relatório de tempos |
| `usuario` | struct `Usuario`, lista dinâmica, cadastro, listagem, busca, arquivo texto |
| `emprestimo` | empréstimo, devolução, multa, atrasados, histórico, arquivo texto |
| `arquivo` | persistência binária e carga/salvamento geral |
| `main` | menu principal |

## Observações

- `.bin` tem prioridade sobre `.txt` na hora de carregar. O `.txt` é uma cópia legível que o programa regrava a cada alteração.
  Se editar um `.txt` na mão, apague os `.bin` correspondentes para o programa ler o texto.
- Campos de texto não aceitam `;` (é o separador do `.txt`).
- Prazo (7 dias) e multa (R$ 1,00/dia) começam com valores padrão e podem ser alterados na opção 13 (valem até fechar o programa).
- Os arquivos `.bin` dependem do compilador/sistema: não os leve de um computador para outro.
RELATORIO_DESEMPENHO.md
markdown
# Relatório de Desempenho (Parte 2)

Gerado pela opção **14** do menu (`src/desempenho.c`). Os tempos foram medidos com `clock()` (`<time.h>`).

## Como foi medido

- Livros gerados automaticamente, com códigos de 1 a N **embaralhados** (caso médio), sempre a mesma sequência.
- Ordenação: cada método ordena uma **cópia** do mesmo vetor, por título.
- Busca: 500 códigos sorteados; as buscas se repetem até passar de 50 ms para o relógio conseguir medir; o resultado é o **tempo médio de uma busca**.
  Sequencial usa o vetor desordenado; binária usa o vetor já ordenado por código.

## 1) Ordenação por título (ms)

| N | Bubble | Selection | Insertion |
|--------:|--------:|--------:|--------:|
| 1000 | 35,823 | 31,027 | 15,463 |
| 2500 | 220,199 | 184,954 | 85,062 |
| 5000 | 897,524 | 717,931 | 337,325 |
| 10000 | 3700,199 | 2897,078 | 1468,040 |

## 2) Busca por código (µs por busca)

| N | Sequencial | Binária | Ganho |
|--------:|--------:|--------:|--------:|
| 1000 | 0,446 | 0,064 | 7,0x |
| 2500 | 1,301 | 0,078 | 16,7x |
| 5000 | 2,603 | 0,089 | 29,4x |
| 10000 | 5,949 | 0,109 | 54,8x |

## Análise

**Ordenação.** Os três métodos são O(n²): quando N dobra (5000 → 10000), o tempo fica ~4 vezes maior
(Bubble 897 → 3700 ms). O **Insertion Sort foi o mais rápido** (cerca de 2,5x mais rápido que o Bubble),
porque desloca elementos em vez de trocá-los par a par e faz menos comparações em média.
O **Bubble foi o mais lento**, pois faz muitas trocas de structs (cada `Livro` ocupa 212 bytes).
O Selection faz poucas trocas (no máximo N), mas sempre percorre o resto do vetor inteiro para achar o menor.

**Busca.** A sequencial é O(n): o tempo cresce junto com N (0,446 → 5,949 µs, ~13x para 10x mais livros).
A binária é O(log n): quase não muda (0,064 → 0,109 µs). O ganho aumenta com N (7x → 55x).

**Ressalva.** A busca binária exige o acervo ordenado. Ordenar custa O(n²) com os métodos implementados,
então a binária só compensa quando há **muitas buscas** para poucas ordenações (como em um acervo que muda pouco).
Para uma única busca num acervo desordenado, a sequencial é melhor.

## Conclusão

Para um acervo pequeno (dezenas de livros) a diferença é imperceptível. Conforme o acervo cresce,
**Insertion Sort + busca binária** é a combinação mais vantajosa entre as implementadas.