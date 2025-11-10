#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINHA 256

struct Produtos{
        int idProduto;
        char nomeProduto[50];
        char tipoProduto[50];
        float precoProduto;
        int quantidadeProduto;
        char fornecedorProduto[50];
    };

struct Produtos listaProdutos[] = {
    
};

// Função para ler o maior ID de um produto
int obterId() {
    FILE *arquivoId;
    int proximoId = 1;

    arquivoId = fopen("proximoId.txt", "r");

    if (arquivoId != NULL) {
        // Se o arquivo existir ele tentar ler o número
        if(fscanf(arquivoId, "%d", &proximoId) != 1) {
            // Se a leitura falhar, assume 1, mas fecha o arquivo
            proximoId = 1;
        }
        fclose(arquivoId);
    }
    return(proximoId);
}


int main () {
    int opcao, i = 0;
    int idGlobal = obterId();
    char linha[MAX_LINHA];

    while (1) {
        //Lista de opções que o usuário tem
        printf("----------TABELA DADOS ATACADÃO----------\n\n");
        printf("1 - Adicionar Produto\n");
        printf("2 - Listar Produtos\n");
        printf("3 - Inserir Informações\n");
        printf("4 - Alterar Produto\n");
        printf("5 - Exluir Produto\n");
        printf("6 - Buscar\n");
        printf("7 - Sair\n\n");

        printf("Escolha a opção que desejas (1-7) \n");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                //Caso 1: Adicionar produto ao struct array produtos e depois ao arquivo 
                struct Produtos criarProduto;

                // Incrementa o ID no struct e adiciona ele em um arquivo separado
                criarProduto.idProduto = idGlobal;
                idGlobal++;
                FILE *arquivoId = fopen("proximoId.txt", "w");
                if (arquivoId == NULL) {
                    printf("Erro ao abrir arquivos. ");
                    return 1;
                }
                fprintf(arquivoId, "%d", idGlobal);
                fclose(arquivoId);

                // Pergunta os valores a serem adicionados ao usuário
                printf("Digite o nome do produto: \n");
                scanf("%s", &criarProduto.nomeProduto);

                printf("Digite o tipo do produto: \n");
                scanf("%s", &criarProduto.tipoProduto);

                printf("Digite o preço do produto: (sem numeros)\n");
                scanf("%f", &criarProduto.precoProduto);

                printf("Digite a quantidade de produto no estoque: \n");
                scanf("%d", &criarProduto.quantidadeProduto);

                printf("Digite qual o fornecedor do produto: \n");
                scanf("%s", &criarProduto.fornecedorProduto);

                // Adiciona os valores ao arquivo "produtos.txt"
                FILE *arquivoProdutos = fopen("produtos.txt", "a+");
                if (arquivoProdutos == NULL) {
                    printf("Erro ao abrir o arquivo.");
                    return 1;
                }
                fprintf(arquivoProdutos, "%d,", criarProduto.idProduto);
                fprintf(arquivoProdutos, "%s,", criarProduto.nomeProduto);
                fprintf(arquivoProdutos, "%s,", criarProduto.tipoProduto);
                fprintf(arquivoProdutos, "%.2f,", criarProduto.precoProduto);
                fprintf(arquivoProdutos, "%d,", criarProduto.quantidadeProduto);
                fprintf(arquivoProdutos, "%s\n", criarProduto.fornecedorProduto);
                fclose(arquivoProdutos);
                break;
            }
            case 2: {
                int id, quantidade;
                float preco;
                char nome[50], tipo[50], fornecedor[50];
                // Abre o arquivo produtos.txt para leitura
                FILE *arquivoProdutos = fopen("produtos.txt", "r");
                if (arquivoProdutos == NULL) {
                    printf("Nenhum produto cadastrado.\n");
                    break;
                }
                // Imprime topo da tabela
                printf("----------------------------------------------------------------------------\n");
                printf("| ID | Produto | Categoria | Preço (R$) | Quantidade | Fornecedor |\n");
                printf("----------------------------------------------------------------------------\n");

                // Lê cada linha da tabela e "coleta" apenas os elementos ("elimina" as virgulas)
                while (fgets(linha, sizeof(linha), arquivoProdutos) != NULL) {
                    int itensLidos = sscanf(linha, "%d,%49[^,],%49[^,],%f,%d,%49[^,]", // O %49[^,] vai ler uma string até 49 caracteres OU até uma virgula
                                            &id, nome, tipo, &preco, &quantidade, fornecedor);
                    if (itensLidos == 6) {
                        printf("| %-2d | %-7s | %-9s | %-10.2f | %-10d | %-10s \n",
                                id, nome, tipo, preco, quantidade, fornecedor);
                    }
                    else {
                       printf("Erro de formato na linha.\n");
                       return 1; 
                    }
                }
                fclose(arquivoProdutos);
                printf("----------------------------------------------------------------------------\n");
                break;
            }
            case 7: {
                printf("Programa finalizado com sucesso.");
                return 0;
            }
        }
    }
}