#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constantes para o tamanho das strings
#define TAM_T 151
#define TAM_A 201
#define TAM_E 51
#define MAX 100

// Estrutura para representar um livro
typedef struct {
    int codigo;
    char titulo[TAM_T];
    char autor[TAM_A];
    char editora[TAM_E];
    int edicao;
    int ano;
    int exemplares;
    float preco;
    // Adicionar os 'ponteiros' para a ABB, que na verdade são índices de registro no arquivo
    int indice_esquerda;
    int indice_direita;
} Livro;

// Estrutura para o cabeçalho do arquivo binário
typedef struct {
    int indice_raiz;
    int indice_topo;
    int indice_livres;
} Cabecalho;


// Funções da biblioteca


// Cadastra o livro do usuario
// Input: Ponteiro para o nome do arquivo 
// Output: Cadastra o novo livro
void cadastrar_livro(const char* nome_arquivo);

// Imprime todos os dados de um livro 
// Input: Ponteiro para o nome arquivo bin
// Output: Todas as informacoes do livro sao imprimidas
void imprimir_dados_do_livro(const char* nome_arquivo);

// Funcao que percorre todos os livros e chama outra funcao para imprimir individualmente
// Input: Ponteiro para o nome do arquivo bin
// Output: Imprime todos os livros
void listar_todos_os_livros(const char* nome_arquivo);

// Calcula total de livros cadastrados
// Input: Ponteiro para o nome do arquivo bin
// Output: Imprime total de livros cadastrados
void calcular_total(const char* nome_arquivo);

// Recebe as informacoes do livro para remover
// Input: Ponteiro para o nome do arquivo bin
// Output: Remove o livro da arvore
void remover_livro(const char* nome_arquivo);

// Carrega do arquivo txt e insere-os na arvore em um arquivo binário
// Input: Ponteiro para o nome do arquivo bin e arquivo txt
// Output: Todos os livros do txt sao passados para o bin
void carregar_arquivo(const char* nome_arquivo, const char* nome_txt);

// Imprime lista de registros livres
// Input: Ponteiro para o nome do arquivo bin
// Output: Imprime a lista de registros livres
void imprimir_lista_de_registros_livres(const char* nome_arquivo);

// Imprime a arvore por niveis comecando pela raiz  
// Input: Ponteiro para o nome do arquivo bin   
// Output: A arvore imprimida por niveis
void imprimir_arvore_por_niveis(const char* nome_arquivo);


// Funções para manipulação do arquivo


// Funcao para abrir o arquivo ou criar caso nao exista
// Input: Ponteiro para o nome do arquivo
// Output: retorna o ponteiro para o arquivo criado
FILE* abrir_arquivo(const char* nome_arquivo);

// Inicializa o arquivo de maneira padrao
// Input: Ponteiro para o nome do arquivo bin
// Output: Cabecalho inicializado
void inicializar_arquivo(const char* nome_arquivo);

// Le o cabecalho do arquivo bin
// Input: Ponteiro para arquivo aberto e ponteiro cabecalho
// Output: Estrutura cabecalho preenchida com os dados do inicio do arquivo
void ler_cabecalho(FILE* arquivo, Cabecalho* cabecalho);

// Grava a estrutura cabecalho no inicio do arquivo bin
// Input: Ponteiro para arquivo aberto, ponteiro para cabecalho
// Output: cabecalho do arquivo atualizado
void escrever_cabecalho(FILE* arquivo, Cabecalho* cabecalho);

// Le do arquivo bin o registro de um livro no indice informado
// Input: Ponteiro para arquivo aberto, indice e ponteiro para a estrutura do livro
// Output: estrutura livro preenchido com os dados
void ler_livro(FILE* arquivo, int indice, Livro* livro);

// Grava no arquivo bin os dados de um livro no indice especificado
// Input: Ponteiro para arquivo aberto, indice e ponteiro para a estrutura do livro
// Output: Registro do livro atualizado no arquivo e alteracoes salvas
void escrever_livro(FILE* arquivo, int indice, Livro* livro);

#endif