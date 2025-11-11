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
        printf("3 - Alterar dados de um produto\n");
        printf("4 - Exluir Produto\n");
        printf("5 - Buscar\n");
        printf("6 - Sair\n\n");

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
                printf("-------------------------------------------------------------------\n");
                printf("| ID | Produto | Categoria | Preço (R$) | Quantidade | Fornecedor |\n");
                printf("-------------------------------------------------------------------\n");

                // Lê cada linha da tabela e "coleta" apenas os elementos ("elimina" as virgulas)
                while (fgets(linha, sizeof(linha), arquivoProdutos) != NULL) {
                    // O sscanf vai "separar" os valores da string em variáveis (StringSCANF)
                    int itensLidos = sscanf(linha, "%d,%49[^,],%49[^,],%f,%d,%49[^\n]", // O %49[^,] vai ler uma string até 49 caracteres OU até uma virgula
                                            &id, nome, tipo, &preco, &quantidade, fornecedor);
                    if (itensLidos == 6) {
                        printf("| %-2d | %-7s | %-9s | %-10.2f | %-10d | %-10s |\n", // O -2 tá subtraindo a quantidade de caracteres do topo da tabela para
                                id, nome, tipo, preco, quantidade, fornecedor);     // ficar alinhado
                    }
                    else {
                       printf("Erro de formato na linha.\n");
                       return 1; 
                    }
                }
                printf("-------------------------------------------------------------------\n");
                fclose(arquivoProdutos);
                break;
            }
            case 3 : {
                int idAlvo, id, quantidade;
                int encontrado = 0;
                float preco;
                char nome[50], tipo[50], fornecedor[50];
                
                printf("Escreva o ID do produto que deseja ALTERAR: \n");
                int c; while((c = getchar()) != '\n' && c != EOF); // Limpa o buffer do idAlvo
                scanf("%d", &idAlvo);
                
                // Criando o arquivo produtos em formato de leitura
                FILE *arquivoProdutos = fopen("produtos.txt", "r");
                if (arquivoProdutos == NULL) {
                    printf("Nenhum produto cadastrado.\n");
                    break;
                }
                // Criando um arquivo temporário para evitar corrupção dos dados
                FILE *arquivoTemporario = fopen("temp.txt", "w");
                if (arquivoTemporario == NULL) {
                    printf("Falha ao criar arquivo temporário.\n");
                    return 1;
                }
                
                // Loop while que verifica cada linha no arquivo
                while (fgets(linha, sizeof(linha), arquivoProdutos)) {
                    int itensLidos = sscanf(linha, "%d,%49[^,],%49[^,],%f,%d,%49[^\n]",  // O sscanf está separando os valores da string nas variáveis (explicado no case 2)
                                            &id, nome, tipo, &preco, &quantidade, fornecedor);
                    if (itensLidos == 6 && id == idAlvo) {
                        encontrado = 1;
                        int opcao;
                        
                        printf("1 - Nome\n");
                        printf("2 - Tipo\n");
                        printf("3 - Preço\n");
                        printf("4 - Quantidade\n");
                        printf("5 - Nome do fornecedor\n\n");
                        
                        printf("O que deseja alterar? (1-6)\n");
                        while((c = getchar()) != '\n' && c != EOF);
                        scanf("%d", &opcao);
                        
                        while((c = getchar()) != '\n' && c != EOF);

                        switch (opcao) {
                            case 1: {
                                printf("Para qual nome deseja alterar?\n");
                                scanf("%s", &nome);
                                break;
                            }
                            case 2: {
                                printf("Para qual tipo deseja alterar?\n");
                                scanf("%s", &tipo);
                                break;
                            }
                            case 3: {
                                printf("Para qual preço deseja alterar?\n");
                                scanf("%f", &preco);
                                break;
                            }
                            case 4: {
                                printf("Para qual quantidade deseja alterar?\n");
                                scanf("%d", &quantidade);
                                break;
                            }
                            case 5: {
                                printf("Para qual nome de fornecedor deseja alterar?\n");
                                scanf("%s", &fornecedor);
                                break;
                            }
                        }
                        fprintf(arquivoTemporario, "%d,%s,%s,%.2f,%d,%s\n",
                                id, nome, tipo, preco, quantidade, fornecedor);
                    }
                    else if (itensLidos == 6) {
                        fprintf(arquivoTemporario, "%d,%s,%s,%.2f,%d,%s\n",
                                id, nome, tipo, preco, quantidade, fornecedor);
                    }
                    else {
                        printf("AVISO: LINHA IGNORADA POR ERRO DE FORMATO.\n");
                        continue;
                    }
                }
                // Fechando arquivos abertos previamente
                fclose(arquivoProdutos);
                fclose(arquivoTemporario);

                if (encontrado == 1) {
                    if (remove("produtos.txt") == 0 && rename("temp.txt", "produtos.txt") == 0) {
                        printf("\nProduto ID %d alterado com sucesso!", idAlvo);
                    }
                    else {
                        printf("ERRO CRÍTICO: Falha ao aplicar alteração.\n");
                    }
                }
                else {
                    printf("\nProduto com ID %d não encontrado.\n", idAlvo);
                    remove("temp.txt");
                }
                break;
            }
            case 4: {
                break;
            }
            case 5: {
                break;
            }
            case 6: {
                printf("Programa finalizado com sucesso.");
                return 0;
            }
        }
    }
}