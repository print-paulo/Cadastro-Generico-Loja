#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Produtos{
        int idProduto;
        char nomeProduto[50];
        char tipoProduto[50];
        char precoProduto[50];
        int quantidadeProduto;
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

                printf("Digite o preço do produto: \n");
                scanf("%s", &criarProduto.precoProduto);

                printf("Digite a quantidade de produto no estoque: \n");
                scanf("%d", &criarProduto.quantidadeProduto);

                // Adiciona os valores ao arquivo "produtos.txt"
                FILE *arquivoProdutos = fopen("produtos.txt", "a+");
                if (arquivoProdutos == NULL) {
                    printf("Erro ao abrir o arquivo.");
                    return 1;
                }
                fprintf(arquivoProdutos, "%d, ", criarProduto.idProduto);
                fprintf(arquivoProdutos, "%s, ", criarProduto.nomeProduto);
                fprintf(arquivoProdutos, "%s, ", criarProduto.tipoProduto);
                fprintf(arquivoProdutos, "%s, ", criarProduto.precoProduto);
                fprintf(arquivoProdutos, "%d\n", criarProduto.quantidadeProduto);
                fclose(arquivoProdutos);
                break;
            }
            case 7:
                printf("Programa finalizado com sucesso.");
                return 0;
        }
    }
}