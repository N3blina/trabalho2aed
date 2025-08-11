#include "biblioteca.h"

// Funções auxiliares


// Imprimir o livros em ordem
// Input: Ponteiro para o arquivo bin e indice
// Output: Imprime o livro com suas informacoes
void listar_in_ordem(FILE* arq, int indice) {
    if (indice == -1) return;

    Livro aux;
    ler_livro(arq, indice, &aux);
    listar_in_ordem(arq, aux.indice_esquerda);
    printf("Codigo: %d | Titulo: %s | Autor: %s | Exemplares: %d\n",
           aux.codigo, aux.titulo, aux.autor, aux.exemplares);
    listar_in_ordem(arq, aux.indice_direita);
}

// Imprime total de livros cadastrados
// Input: Ponteiro para o arquivo bin e indice
// Output: Imprime total de livros cadastrados
static int contar_livros(FILE* arq, int indice) {
    if (indice == -1) return 0;
    Livro aux;
    ler_livro(arq, indice, &aux);
    return 1 + contar_livros(arq, aux.indice_esquerda) + contar_livros(arq, aux.indice_direita);
}

// Inseri livro na arvore
// Input: Ponteiro para o arquivo bin, ponteiro para a cabeca e ponteiro para novo livro
// Output: livro inserido
static void inserir_livro_abb(FILE* arq, Cabecalho* cab, Livro* novo) {
    int indice_novo;

    if (cab->indice_livres != -1) {
        indice_novo = cab->indice_livres;
        Livro livre;
        ler_livro(arq, indice_novo, &livre);
        cab->indice_livres = livre.indice_esquerda;
    } else {
        indice_novo = cab->indice_topo++;
    }

    novo->indice_esquerda = -1;
    novo->indice_direita = -1;

    if (cab->indice_raiz == -1) {
        cab->indice_raiz = indice_novo;
    } else {
        int atual = cab->indice_raiz;
        while (1) {
            Livro aux;
            ler_livro(arq, atual, &aux);
            if (novo->codigo < aux.codigo) {
                if (aux.indice_esquerda == -1) {
                    aux.indice_esquerda = indice_novo;
                    escrever_livro(arq, atual, &aux);
                    break;
                }
                atual = aux.indice_esquerda;
            } else {
                if (aux.indice_direita == -1) {
                    aux.indice_direita = indice_novo;
                    escrever_livro(arq, atual, &aux);
                    break;
                }
                atual = aux.indice_direita;
            }
        }
    }

    escrever_livro(arq, indice_novo, novo);
}

// Remove um livro 
// Input: Ponteiro para o arquivo bin, indice, codigo, ponteiro para cabeca e ponteiro inteiro para removido
// Output: Arvore atualizada
static int remover_rec(FILE* arq, int indice, int codigo, Cabecalho* cab, int* removido) {
    if (indice == -1) return -1;

    Livro atual;
    ler_livro(arq, indice, &atual);

    if (codigo < atual.codigo) {
        atual.indice_esquerda = remover_rec(arq, atual.indice_esquerda, codigo, cab, removido);
        escrever_livro(arq, indice, &atual);
    }
    else if (codigo > atual.codigo) {
        atual.indice_direita = remover_rec(arq, atual.indice_direita, codigo, cab, removido);
        escrever_livro(arq, indice, &atual);
    }
    else {
        *removido = 1;
        if (atual.indice_esquerda == -1 && atual.indice_direita == -1) {
            atual.indice_esquerda = cab->indice_livres;
            cab->indice_livres = indice;
            escrever_livro(arq, indice, &atual);
            return -1;
        }
        else if (atual.indice_esquerda == -1) {
            atual.indice_esquerda = cab->indice_livres;
            cab->indice_livres = indice;
            escrever_livro(arq, indice, &atual);
            return atual.indice_direita;
        }
        else if (atual.indice_direita == -1) {
            atual.indice_esquerda = cab->indice_livres;
            cab->indice_livres = indice;
            escrever_livro(arq, indice, &atual);
            return atual.indice_esquerda;
        }
        else {
            int sucessor = atual.indice_direita;
            Livro suc;
            while (1) {
                ler_livro(arq, sucessor, &suc);
                if (suc.indice_esquerda == -1) break;
                sucessor = suc.indice_esquerda;
            }
            atual.codigo = suc.codigo;
            strcpy(atual.titulo, suc.titulo);
            strcpy(atual.autor, suc.autor);
            strcpy(atual.editora, suc.editora);
            atual.edicao = suc.edicao;
            atual.ano = suc.ano;
            atual.exemplares = suc.exemplares;
            atual.preco = suc.preco;
            atual.indice_direita = remover_rec(arq, atual.indice_direita, suc.codigo, cab, removido);
            escrever_livro(arq, indice, &atual);
        }
    }
    return indice;
}

// Verifica se o livro existe
// Input: Ponteiro para nome do aquirvo e codigo
// Output: Retorna 1 se encontrou e 0 caso contrario
int livro_existe(const char *nome_bin, int codigo) {
    FILE *arq = fopen(nome_bin, "rb");
    if (!arq) return 0;

    Cabecalho cab;
    fread(&cab, sizeof(Cabecalho), 1, arq);

    int pos = cab.indice_raiz;
    Livro livro;
    while (pos != -1) {
        fseek(arq, sizeof(Cabecalho) + pos * sizeof(Livro), SEEK_SET);
        fread(&livro, sizeof(Livro), 1, arq);

        if (codigo == livro.codigo) {
            fclose(arq);
            return 1; // Existe
        }
        if (codigo < livro.codigo)
            pos = livro.indice_esquerda;
        else
            pos = livro.indice_direita;
    }

    fclose(arq);
    return 0; // Não existe
}

// Funções da biblioteca

// Cadastra o livro do usuario
// Input: Ponteiro para o nome do arquivo 
// Output: Cadastra o novo livro
void cadastrar_livro(const char* nome_arquivo) {
    FILE* arq = abrir_arquivo(nome_arquivo);
    Cabecalho cab;
    ler_cabecalho(arq, &cab);

    Livro novo;
    printf("Cadigo: ");
    scanf("%d%*c", &novo.codigo);

    while(livro_existe(nome_arquivo, novo.codigo)){
        printf("O codigo ja existe, coloque outro: ");
        scanf("%d", &novo.codigo);
    }

    printf("Titulo: ");
    scanf("%[^\n]%*c", novo.titulo);

    printf("Autor: ");
    scanf("%[^\n]%*c", novo.autor);

    printf("Editora: ");
    scanf("%[^\n]%*c", novo.editora);

    printf("Edicao: ");
    scanf("%d%*c", &novo.edicao);

    printf("Ano: ");
    scanf("%d%*c", &novo.ano);

    printf("Exemplares: ");
    scanf("%d%*c", &novo.exemplares);

    printf("Preco: ");
    scanf("%f%*c", &novo.preco);

    novo.indice_esquerda = -1;
    novo.indice_direita = -1;

    int indice_novo;
    if (cab.indice_livres != -1) {
        indice_novo = cab.indice_livres;
        Livro livre;
        ler_livro(arq, indice_novo, &livre);
        cab.indice_livres = livre.indice_esquerda;
    } else {
        indice_novo = cab.indice_topo;
        cab.indice_topo++;
    }

    if (cab.indice_raiz == -1) {
        cab.indice_raiz = indice_novo;
    } else {
        int atual = cab.indice_raiz;
        while (1) {
            Livro aux;
            ler_livro(arq, atual, &aux);
            if (novo.codigo < aux.codigo) {
                if (aux.indice_esquerda == -1) {
                    aux.indice_esquerda = indice_novo;
                    escrever_livro(arq, atual, &aux);
                    break;
                }
                atual = aux.indice_esquerda;
            } else {
                if (aux.indice_direita == -1) {
                    aux.indice_direita = indice_novo;
                    escrever_livro(arq, atual, &aux);
                    break;
                }
                atual = aux.indice_direita;
            }
        }
    }

    escrever_livro(arq, indice_novo, &novo);
    escrever_cabecalho(arq, &cab);
    fclose(arq);

    printf("Livro cadastrado com sucesso!\n");
}

// Imprime todos os dados de um livro 
// Input: Ponteiro para o nome arquivo bin
// Output: Todas as informacoes do livro sao imprimidas
void imprimir_dados_do_livro(const char* nome_arquivo) {
    FILE* arq = abrir_arquivo(nome_arquivo);
    Cabecalho cab;
    ler_cabecalho(arq, &cab);

    int codigo;
    printf("Digite o codigo do livro: ");
    scanf("%d", &codigo);

    int atual = cab.indice_raiz;
    while (atual != -1) {
        Livro aux;
        ler_livro(arq, atual, &aux);
        if (codigo == aux.codigo) {
            printf("\nCodigo: %d\nTitulo: %s\nAutor: %s\nEditora: %s\nEdicao: %d\nAno: %d\nExemplares: %d\nPreco: %.2f\n",
                   aux.codigo, aux.titulo, aux.autor, aux.editora, aux.edicao, aux.ano, aux.exemplares, aux.preco);
            fclose(arq);
            return;
        } else if (codigo < aux.codigo) {
            atual = aux.indice_esquerda;
        } else {
            atual = aux.indice_direita;
        }
    }

    printf("Livro nao encontrado.\n");
    fclose(arq);
}

// Funcao que percorre todos os livros e chama outra funcao para imprimir individualmente
// Input: Ponteiro para o nome do arquivo bin
// Output: Imprime todos os livros
void listar_todos_os_livros(const char* nome_arquivo) {
    FILE* arq = abrir_arquivo(nome_arquivo);
    Cabecalho cab;
    ler_cabecalho(arq, &cab);

    if (cab.indice_raiz == -1) {
        printf("Nenhum livro cadastrado.\n");
    } else {
        listar_in_ordem(arq, cab.indice_raiz);
    }
    fclose(arq);
}

// Calcula total de livros cadastrados
// Input: Ponteiro para o nome do arquivo bin
// Output: Imprime total de livros cadastrados
void calcular_total(const char* nome_arquivo) {
    FILE* arq = abrir_arquivo(nome_arquivo);
    Cabecalho cab;
    ler_cabecalho(arq, &cab);

    int total = contar_livros(arq, cab.indice_raiz);
    printf("Total de livros cadastrados: %d\n", total);

    fclose(arq);
}

// Recebe as informacoes do livro para remover
// Input: Ponteiro para o nome do arquivo bin
// Output: Remove o livro da arvore
void remover_livro(const char* nome_arquivo) {
    FILE* arq = abrir_arquivo(nome_arquivo);
    Cabecalho cab;
    ler_cabecalho(arq, &cab);

    int codigo;
    printf("Digite o codigo do livro a remover: ");
    scanf("%d", &codigo);

    int removido = 0;
    cab.indice_raiz = remover_rec(arq, cab.indice_raiz, codigo, &cab, &removido);

    if (removido) {
        escrever_cabecalho(arq, &cab);
        printf("Livro removido com sucesso.\n");
    } else {
        printf("Livro não encontrado.\n");
    }

    fclose(arq);
}

// Carrega do arquivo txt e insere-os na arvore em um arquivo binário
// Input: Ponteiro para o nome do arquivo bin e arquivo txt
// Output: Todos os livros do txt sao passados para o bin
void carregar_arquivo(const char* nome_arquivo, const char* nome_txt) {
    
    FILE* txt = fopen(nome_txt, "r");
    if (!txt) {
        printf("Erro ao abrir o arquivo de texto.\n");
        return;
    }

    FILE* arq = abrir_arquivo(nome_arquivo);
    Cabecalho cab;
    ler_cabecalho(arq, &cab);

    char linha[512];
    while (fgets(linha, sizeof(linha), txt)) {
        Livro novo;
        char preco_str[20];

        sscanf(linha, "%d ; %150[^;] ; %200[^;] ; %50[^;] ; %d ; %d ; %d ; %19s",
               &novo.codigo,
               novo.titulo,
               novo.autor,
               novo.editora,
               &novo.edicao,
               &novo.ano,
               &novo.exemplares,
               preco_str);

        // Troca vírgula por ponto
        for (int i = 0; preco_str[i]; i++)
            if (preco_str[i] == ',') preco_str[i] = '.';
        novo.preco = atof(preco_str);

        novo.indice_esquerda = -1;
        novo.indice_direita = -1;

        if (livro_existe(nome_arquivo, novo.codigo)) {
            continue;
        }

        int indice_novo;
        if (cab.indice_livres != -1) {
            indice_novo = cab.indice_livres;
            Livro livre;
            ler_livro(arq, indice_novo, &livre);
            cab.indice_livres = livre.indice_esquerda;
        } else {
            indice_novo = cab.indice_topo++;
        }

        if (cab.indice_raiz == -1) {
            cab.indice_raiz = indice_novo;
        } else {
            int atual = cab.indice_raiz;
            while (1) {
                Livro aux;
                ler_livro(arq, atual, &aux);
                if (novo.codigo < aux.codigo) {
                    if (aux.indice_esquerda == -1) {
                        aux.indice_esquerda = indice_novo;
                        escrever_livro(arq, atual, &aux);
                        break;
                    }
                    atual = aux.indice_esquerda;
                } else {
                    if (aux.indice_direita == -1) {
                        aux.indice_direita = indice_novo;
                        escrever_livro(arq, atual, &aux);
                        break;
                    }
                    atual = aux.indice_direita;
                }
            }
        }

        escrever_livro(arq, indice_novo, &novo);
    }

    escrever_cabecalho(arq, &cab);
    fclose(txt);
    fclose(arq);

    printf("Carregamento concluido!\n");
}

// Imprime lista de registros livres
// Input: Ponteiro para o nome do arquivo bin
// Output: Imprime a lista de registros livres
void imprimir_lista_de_registros_livres(const char* nome_arquivo) {
    FILE* arq = abrir_arquivo(nome_arquivo);
    Cabecalho cab;
    ler_cabecalho(arq, &cab);

    if (cab.indice_livres == -1) {
        printf("Nenhum registro livre.\n");
        fclose(arq);
        return;
    }

    printf("Registros livres: ");
    int atual = cab.indice_livres;
    while (atual != -1) {
        printf("%d ", atual);
        Livro livre;
        ler_livro(arq, atual, &livre);
        atual = livre.indice_esquerda; // próximo livre
    }
    printf("\n");

    fclose(arq);
}

// Imprime a arvore por niveis comecando pela raiz  
// Input: Ponteiro para o nome do arquivo bin   
// Output: A arvore imprimida por niveis
void imprimir_arvore_por_niveis(const char* nome_arquivo) {
    FILE* arq = abrir_arquivo(nome_arquivo);
    Cabecalho cab;
    ler_cabecalho(arq, &cab);

    if (cab.indice_raiz == -1) {
        printf("Arvore vazia.\n");
        fclose(arq);
        return;
    }

    int fila[MAX];
    int ini = 0, fim = 0;
    fila[fim++] = cab.indice_raiz;

    while (ini < fim) {
        int nivel_count = fim - ini; // quantos nós neste nível
        for (int i = 0; i < nivel_count; i++) {
            int indice = fila[ini++];
            Livro aux;
            ler_livro(arq, indice, &aux);
            printf("%d ", aux.codigo);

            if (aux.indice_esquerda != -1)
                fila[fim++] = aux.indice_esquerda;
            if (aux.indice_direita != -1)
                fila[fim++] = aux.indice_direita;
        }
        printf("\n");
    }

    fclose(arq);
}

// Funções de baixo nível para manipulação do arquivo

// Funcao para abrir o arquivo ou criar caso nao exista
// Input: Ponteiro para o nome do arquivo
// Output: retorna o ponteiro para o arquivo criado
FILE* abrir_arquivo(const char* nome_arquivo) {
    FILE* arq = fopen(nome_arquivo, "rb+");
    if (!arq) {
        // Se não existir, cria no modo wb+ e fecha para reabrir no modo rb+
        arq = fopen(nome_arquivo, "wb+");
        if (!arq) {
            perror("Erro ao criar arquivo");
            exit(1);
        }
        fclose(arq);
        arq = fopen(nome_arquivo, "rb+");
    }
    return arq;
}

// Inicializa o arquivo de maneira padrao
// Input: Ponteiro para o nome do arquivo bin
// Output: Cabecalho inicializado
void inicializar_arquivo(const char* nome_arquivo) {
    FILE* arq = abrir_arquivo(nome_arquivo);
    Cabecalho cab;
    cab.indice_raiz = -1;   // Árvore vazia
    cab.indice_topo = 0;    // Primeiro índice de livro a ser usado
    cab.indice_livres = -1; // Nenhum registro livre
    escrever_cabecalho(arq, &cab);
    fclose(arq);
}

//
//
//
void ler_cabecalho(FILE* arquivo, Cabecalho* cabecalho) {
    fseek(arquivo, 0, SEEK_SET);
    fread(cabecalho, sizeof(Cabecalho), 1, arquivo);
}

//
//
//
void escrever_cabecalho(FILE* arquivo, Cabecalho* cabecalho) {
    fseek(arquivo, 0, SEEK_SET);
    fwrite(cabecalho, sizeof(Cabecalho), 1, arquivo);
    fflush(arquivo);
}

//
//
//
void ler_livro(FILE* arquivo, int indice, Livro* livro) {
    fseek(arquivo, sizeof(Cabecalho) + indice * sizeof(Livro), SEEK_SET);
    fread(livro, sizeof(Livro), 1, arquivo);
}
//
//
//
void escrever_livro(FILE* arquivo, int indice, Livro* livro) {
    fseek(arquivo, sizeof(Cabecalho) + indice * sizeof(Livro), SEEK_SET);
    fwrite(livro, sizeof(Livro), 1, arquivo);
    fflush(arquivo);
}
