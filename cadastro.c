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
    FILE *arquivo;
    int proximoId = 1; // Valor padrão se não houver ID

    arquivo = fopen("proximoId.txt", "r");

    if (arquivo != NULL) {
        // Se o arquivo existir ele tentar ler o número
        if(fscanf(arquivo, "%d", &proximoId) != 1) {
            // Se a leitura falhar, assume 1, mas fecha o arquivo
            proximoId = 1;
        }
        fclose(arquivo);
    }
    // Se o arquivo for NULL ele retorna 1

    return(proximoId);
}

// Função para salvar o proximo ID
void salvarProximoId (int novoId) {
    FILE *arquivo;

    // Abre o arquivo para escrita
    arquivo = fopen("proximoId.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.");
        return;
    }

    // Escreve o novo valor do próximo ID
    
    fprintf(arquivo, "%d", novoId);

    fclose(arquivo);
};


int main () {
    int opcao, i = 0;
    int idGlobal = obterId();

    // Criando arquivos necessários para o programa
    FILE *arquivoProduto = fopen("produtos.txt", "w");
    if (arquivoProduto == NULL) {
        printf("Erro ao criar arquivos.");
        return 1;
    }
    fclose(arquivoProduto);

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
            case 1: //Caso 1: Adicionar produto ao struct array produtos e depois ao arquivo
            struct Produtos criarProduto;

            // Incrementa o ID no struct e adiciona ele em um arquivo separado
            criarProduto.idProduto = idGlobal;
            idGlobal++;
            salvarProximoId(idGlobal);

            printf("Digite o nome do produto: \n");
            scanf("%s", &criarProduto.nomeProduto);

            printf("Digite o tipo do produto: \n");
            scanf("%s", &criarProduto.tipoProduto);

            printf("Digite o preço do produto: \n");
            scanf("%s", &criarProduto.precoProduto);

            printf("Digite a quantidade de produto no estoque: \n");
            scanf("&d", &criarProduto.quantidadeProduto);

            break;
        }
    }
    return 0;
}