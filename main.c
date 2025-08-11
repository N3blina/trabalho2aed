#include <stdio.h>
#include "biblioteca.h"

int main() {
    int opcao;
    char arq_texto[MAX];
    char nome_base[MAX];
    char nome_bin[MAX];
    char nome_txt[MAX];

    printf("--- Entre com o nome base dos arquivos (sem extensao) ---\n");
    scanf("%[^\n]%*c", nome_base);

    // Monta os nomes dos arquivos
    sprintf(nome_bin, "%s.bin", nome_base);
    sprintf(nome_txt, "%s.txt", nome_base);

    // Tenta abrir o binário
    FILE* teste = fopen(nome_bin, "rb");
    if (!teste) {
        printf("Binario nao encontrado. Deseja carregar de um arquivo texto?\n[0] Nao  [1] Sim\n");
        int opcao2;
        scanf("%d%*c", &opcao2); // %*c descarta o \n

        if (opcao2 == 1) {
            FILE* teste2 = fopen(nome_txt, "r");
            if (!teste2) {
                printf("Arquivo texto '%s' nao encontrado. Encerrando programa.\n", nome_txt);
                return 1;
            }
            fclose(teste2);

            inicializar_arquivo(nome_bin);
            carregar_arquivo(nome_bin, nome_txt);
            printf("Binario criado e carregado com sucesso!\n");
        } else {
            printf("Encerrando programa.\n");
            return 1;
        }
    } else {
        fclose(teste);
        printf("Arquivo binario encontrado e carregado com sucesso.\n");
    }

    // Loop do menu
    do {
        printf("\n--- MENU DE OPCOES ---\n");
        printf("1. Cadastrar livro\n");
        printf("2. Imprimir dados do livro\n");
        printf("3. Listar todos os livros\n");
        printf("4. Calcular total de livros\n");
        printf("5. Remover livro\n");
        printf("6. Carregar arquivo de texto\n");
        printf("7. Imprimir lista de registros livres\n");
        printf("8. Imprimir arvore por niveis\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d%*c", &opcao); // %*c descarta o \n

        switch (opcao) {
            case 1:
                cadastrar_livro(nome_bin);
                break;
            case 2:
                imprimir_dados_do_livro(nome_bin);
                break;
            case 3:
                listar_todos_os_livros(nome_bin);
                break;
            case 4:
                calcular_total(nome_bin);
                break;
            case 5:
                remover_livro(nome_bin);
                break;
            case 6:
                printf("Digite o nome do arquivo texto: ");
                scanf("%[^\n]%*c", arq_texto);
                strcat(arq_texto, ".txt");
                carregar_arquivo(nome_bin, arq_texto);
                break;
            case 7:
                imprimir_lista_de_registros_livres(nome_bin);
                break;
            case 8:
                imprimir_arvore_por_niveis(nome_bin);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}
