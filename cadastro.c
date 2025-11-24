#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_LINHA 256
#define MAX_PRODUTOS 100

struct Produtos {
        int idProduto;
        char nomeProduto[50];
        char tipoProduto[50];
        float precoProduto;
        int quantidadeProduto;
        char fornecedorProduto[50];
};

struct Produtos listaProdutos[MAX_PRODUTOS] = {

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

void removerNovaLinha (char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

void toLower(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}


int main () {
    int opcao, i = 0, id, quantidade;
    float preco;
    int idGlobal = obterId();
    char linha[MAX_LINHA], tipo[50], nome[50], fornecedor[50];

    FILE *arquivoProdutos = fopen("produtos.txt", "r"); 

    while (fgets(linha, sizeof(linha), arquivoProdutos)) {
        int itensLidos = sscanf(linha, "%d,%49[^,],%49[^,],%f,%d,%49[^\n]", // O %49[^,] vai ler uma string até 49 caracteres OU até uma virgula
                                &id, nome, tipo, &preco, &quantidade, fornecedor);
        
        if (itensLidos == 6) {
            listaProdutos[i].idProduto = id;
            strcpy(listaProdutos[i].nomeProduto, nome);
            strcpy(listaProdutos[i].tipoProduto, tipo);
            listaProdutos[i].precoProduto = preco;
            listaProdutos[i].quantidadeProduto = quantidade;
            strcpy(listaProdutos[i].fornecedorProduto, fornecedor);
            i++;
        }
    }
    fclose(arquivoProdutos);
    
    while (1) {
        //Lista de opções que o usuário tem
        printf("\n----------TABELA DADOS ATACADÃO----------\n\n");
        printf("1 - Adicionar Produto\n");
        printf("2 - Listar Produtos\n");
        printf("3 - Alterar dados de um produto\n");
        printf("4 - Exluir Produto\n");
        printf("5 - Buscar\n");
        printf("6 - Sair\n\n");

        printf("Escolha a opção que desejas (1-7) \n");
        scanf("%d", &opcao);

        int c; while ((c = getchar()) != '\n' && c != EOF);

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
                fgets(criarProduto.nomeProduto, 50, stdin);
                removerNovaLinha(criarProduto.nomeProduto);

                printf("Digite o tipo do produto: \n");
                fgets(criarProduto.tipoProduto, 50, stdin);
                removerNovaLinha(criarProduto.tipoProduto);

                printf("Digite o preço do produto: (sem numeros)\n");
                scanf("%f", &criarProduto.precoProduto);

                printf("Digite a quantidade de produto no estoque: \n");
                scanf("%d", &criarProduto.quantidadeProduto);

                int c; while ((c = getchar()) != '\n' && c != EOF); // Limpa o buffer

                printf("Digite qual o fornecedor do produto: \n");
                fgets(criarProduto.fornecedorProduto, 50, stdin);
                removerNovaLinha(criarProduto.fornecedorProduto);

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
                printf("-------------------------------------------------------------------------------------------------\n");
                printf("| ID | Produto           | Categoria           | Preço (R$) | Quantidade | Fornecedor           |\n");
                printf("-------------------------------------------------------------------------------------------------\n");

                // Lê cada linha da tabela e "coleta" apenas os elementos ("elimina" as virgulas)
                while (fgets(linha, sizeof(linha), arquivoProdutos) != NULL) {
                    // O sscanf vai "separar" os valores da string em variáveis (StringSCANF)
                    int itensLidos = sscanf(linha, "%d,%49[^,],%49[^,],%f,%d,%49[^\n]", // O %49[^,] vai ler uma string até 49 caracteres OU até uma virgula
                                            &id, nome, tipo, &preco, &quantidade, fornecedor);
                    if (itensLidos == 6) {
                        printf("| %-2d | %-17s | %-19s | %-10.2f | %-10d | %-20s |\n", // O -2 tá subtraindo a quantidade de caracteres do topo da tabela para ficar alinhado
                                id, nome, tipo, preco, quantidade, fornecedor);
                    }
                    else {
                       printf("Erro de formato na linha.\n");
                       return 1; 
                    }
                }
                printf("-------------------------------------------------------------------------------------------------\n\n");
                fclose(arquivoProdutos);
                break;
            }
            case 3 : {
                int idAlvo, id, quantidade;
                int encontrado = 0;
                float preco;
                char nome[50], tipo[50], fornecedor[50];
                
                printf("Escreva o ID do produto que deseja ALTERAR: \n");
                scanf("%d", &idAlvo);
                int c; while((c = getchar()) != '\n' && c != EOF); // Limpa o buffer do idAlvo
                
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
                        
                        printf("\n1 - Nome\n");
                        printf("2 - Tipo\n");
                        printf("3 - Preço\n");
                        printf("4 - Quantidade\n");
                        printf("5 - Nome do fornecedor\n\n");
                        
                        printf("O que deseja alterar? (1-6)\n");
                        scanf("%d", &opcao);
                        
                        while((c = getchar()) != '\n' && c != EOF);

                        switch (opcao) {
                            case 1: {
                                printf("Para qual nome deseja alterar?\n");
                                fgets(nome, 50, stdin);
                                removerNovaLinha(nome);
                                break;
                            }
                            case 2: {
                                printf("Para qual tipo deseja alterar?\n");
                                fgets(tipo, 50, stdin);
                                removerNovaLinha(tipo);
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
                                fgets(fornecedor, 50, stdin);
                                removerNovaLinha(fornecedor);
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
                        printf("\nProduto ID %d alterado com sucesso!\n", idAlvo);
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
                int idAlvo, id, quantidade;
                int encontrado = 0;
                float preco;
                char nome[50], tipo[50], fornecedor[50];

                printf("Escreva o ID do produto que deseja EXCLUIR: \n");
                scanf("%d", &idAlvo);
                int c; while((c = getchar()) != '\n' && c != EOF); // Limpa o buffer do idAlvo

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

                while (fgets(linha, sizeof(linha), arquivoProdutos)) {
                    int itensLidos = sscanf(linha, "%d,%49[^,],%49[^,],%f,%d,%49[^\n]",  // O sscanf está separando os valores da string nas variáveis (explicado no case 2)
                                            &id, nome, tipo, &preco, &quantidade, fornecedor);
                    if (itensLidos == 6 && id == idAlvo) {
                        encontrado = 1;
                        char opcao;

                        printf("TEM CERTEZA QUE DESEJA APAGAR O PRODUTO? (s/n) \n");
                        scanf(" %c", &opcao);
                        int c; while ((c = getchar()) != '\n' && c != EOF);

                        if (opcao == 's') {
                            continue;
                        }
                        else if (opcao == 'n') {
                            fprintf(arquivoTemporario, "%d,%s,%s,%.2f,%d,%s\n",
                                id, nome, tipo, preco, quantidade, fornecedor);
                        }
                    }
                    else if (itensLidos == 6) {
                        fprintf(arquivoTemporario, "%d,%s,%s,%.2f,%d,%s\n",
                                id, nome, tipo, preco, quantidade, fornecedor);
                    }
                    else {
                        printf("LINHA IGNORADA POR ERRO DE FORMATO");
                        continue;
                    }
                }
                // Fechando arquivos abertos previamente
                fclose(arquivoProdutos);
                fclose(arquivoTemporario);

                if (encontrado == 1) {
                    if (remove("produtos.txt") == 0 && rename("temp.txt", "produtos.txt") == 0) {
                        printf("\nProduto ID %d alterado com sucesso!\n", idAlvo);
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
            case 5: {
                int opcao, id, quantidade;
                int encontrado = 0;
                float preco;
                char nome[50], tipo[50], fornecedor[50], buscaUsuario[50];

                printf("\n----------------- Buscar -------------------\n");
                printf("1 - Nome\n");
                printf("2 - Tipo\n");
                printf("3 - A-Z\n");
                printf("4 - Z-A\n");
                printf("5 - Maior preço para o menor\n");
                printf("6 - Menor preço para o maior\n");
                printf("----------------------------------------------\n");
                
                printf("O que deseja pesquisar?\n");
                scanf("%d", &opcao);
                int c; while ((c = getchar()) != '\n' && c != EOF);

                switch (opcao) {
                    case 1: {
                        char nome_temp[50];

                        printf("Digite o nome do produto a ser pesquisado\n");
                        fgets(buscaUsuario, 50, stdin);
                        removerNovaLinha(buscaUsuario);
                        toLower(buscaUsuario);

                        printf("-------------------------------------------------------------------------------------------------\n");
                        printf("| ID | Produto           | Categoria           | Preço (R$) | Quantidade | Fornecedor           |\n");
                        printf("-------------------------------------------------------------------------------------------------\n");

                        // Abrindo o arquivo produtos em formato de leitura
                        FILE *arquivoProdutos = fopen("produtos.txt", "r");
                        if (arquivoProdutos == NULL) {
                            printf("Nenhum produto cadastrado.\n");
                            break;
                        }

                        while (fgets(linha, sizeof(linha), arquivoProdutos)) {
                            int itensLidos = sscanf(linha, "%d,%49[^,],%49[^,],%f,%d,%49[^\n]",  // O sscanf está separando os valores da string nas variáveis (explicado no case 2)
                                                    &id, nome, tipo, &preco, &quantidade, fornecedor);

                            strcpy(nome_temp, nome);
                            toLower(nome_temp);

                            if (itensLidos == 6 && strstr(nome_temp, buscaUsuario) != NULL) {
                                encontrado = 1;

                                printf("| %-2d | %-17s | %-19s | %-10.2f | %-10d | %-20s |\n", // O -2 tá subtraindo a quantidade de caracteres do topo da tabela para alinhado
                                id, nome, tipo, preco, quantidade, fornecedor);
                            }
                            else {
                                continue;
                            }
                        }
                        fclose(arquivoProdutos);

                        printf("-------------------------------------------------------------------------------------------------\n\n");
                        
                        if (encontrado != 1) {
                            printf("O item com o nome: '%s' não foi encontrado", buscaUsuario);
                        }
                        break;
                    }
                    case 2: {
                        char tipo_temp[50];

                        printf("Digite o tipo (categoria) do produto a ser pesquisado\n");
                        fgets(buscaUsuario, 50, stdin);
                        removerNovaLinha(buscaUsuario);
                        toLower(buscaUsuario);

                        printf("-------------------------------------------------------------------------------------------------\n");
                        printf("| ID | Produto           | Categoria           | Preço (R$) | Quantidade | Fornecedor           |\n");
                        printf("-------------------------------------------------------------------------------------------------\n");

                        // Abrindo o arquivo produtos em formato de leitura
                        FILE *arquivoProdutos = fopen("produtos.txt", "r");
                        if (arquivoProdutos == NULL) {
                            printf("Nenhum produto cadastrado.\n");
                            break;
                        }

                        while (fgets(linha, sizeof(linha), arquivoProdutos)) {
                            int itensLidos = sscanf(linha, "%d,%49[^,],%49[^,],%f,%d,%49[^\n]",  // O sscanf está separando os valores da string nas variáveis (explicado no case 2)
                                                    &id, nome, tipo, &preco, &quantidade, fornecedor);

                            strcpy(tipo_temp, tipo);
                            toLower(tipo_temp);

                            if (itensLidos == 6 && strstr(tipo_temp, buscaUsuario) != NULL) {
                                encontrado = 1;

                                printf("| %-2d | %-17s | %-19s | %-10.2f | %-10d | %-20s |\n", // O -2 tá subtraindo a quantidade de caracteres do topo da tabela para alinhado
                                id, nome, tipo, preco, quantidade, fornecedor);
                            }
                            else {
                                continue;
                            }
                        }
                        fclose(arquivoProdutos);

                        printf("-------------------------------------------------------------------------------------------------\n\n");
                        
                        if (encontrado != 1) {
                            printf("O item com o tipo: '%s' não foi encontrado", buscaUsuario);
                        }
                        break;
                    }
                    case 3: {
                        int i = 0;
                        char nomeTemp1[50], nomeTemp2[50];                        
                        
                        // Abrindo o arquivo produtos em formato de leitura
                        FILE *arquivoProdutos = fopen("produtos.txt", "r");
                        if (arquivoProdutos == NULL) {
                            printf("Nenhum produto cadastrado.\n");
                            break;
                        }
                        
                        // COLETANDO QUANTIDADE DE PRODUTOS
                        while (fgets(linha, sizeof(linha), arquivoProdutos)) {
                            int itensLidos = sscanf(linha, "%d,%49[^,],%49[^,],%f,%d,%49[^\n]",  // O sscanf está separando os valores da string nas variáveis (explicado no case 2)
                                                    &id, nome, tipo, &preco, &quantidade, fornecedor);

                            if (itensLidos == 6) {
                                listaProdutos[i].idProduto = id;
                                strcpy(listaProdutos[i].nomeProduto, nome);
                                strcpy(listaProdutos[i].tipoProduto, tipo);
                                listaProdutos[i].precoProduto = preco;
                                listaProdutos[i].quantidadeProduto = quantidade;
                                strcpy(listaProdutos[i].fornecedorProduto, fornecedor);
                                
                                i++;
                            }
                            else {
                                printf("Erro de formatação de arquivo.");
                                break;
                            }
                        }
                        fclose(arquivoProdutos);

                        int numProdutos = i;

                        struct Produtos temp; // Variável temporária para troca
                        int j, k;
                        
                        // BUBBLE SORT + Comparação de letra:
                        // Loop externo: (percorre o array do início ao fim)
                        for (j = 0; j < numProdutos - 1; j++) {
                            // Loop interno: (compara e troca elementos adjacentes)
                            for (k = 0; k < numProdutos - 1 - j; k++) {
                                
                                // Comparação: Verifica se o produto K é "maior" que o produto K+1
                                // Usamos strcmp: se > 0, o produto está em ordem errada (ex: Z vem antes de A)

                                // Transformando as duas letras comparadas para case insensitive
                                strcpy(nomeTemp1, listaProdutos[k].nomeProduto);
                                toLower(nomeTemp1);

                                strcpy(nomeTemp2, listaProdutos[k+1].nomeProduto);
                                toLower(nomeTemp2);

                                if (strcmp(nomeTemp1, nomeTemp2) > 0) {
                                    
                                    // TROCA COMPLETA DO STRUCT:
                                    temp = listaProdutos[k];
                                    listaProdutos[k] = listaProdutos[k+1];
                                    listaProdutos[k+1] = temp;
                                }
                            }
                        }

                        printf("-------------------------------------------------------------------------------------------------\n");
                        printf("| ID | Produto           | Categoria           | Preço (R$) | Quantidade | Fornecedor           |\n");
                        printf("-------------------------------------------------------------------------------------------------\n");

                        for (j = 0; j < numProdutos; j++) {
                            printf("| %-2d | %-17s | %-19s | %-10.2f | %-10d | %-20s |\n",
                                    listaProdutos[j].idProduto,
                                    listaProdutos[j].nomeProduto,
                                    listaProdutos[j].tipoProduto,
                                    listaProdutos[j].precoProduto,
                                    listaProdutos[j].quantidadeProduto,
                                    listaProdutos[j].fornecedorProduto);
                        }
                        printf("-------------------------------------------------------------------------------------------------\n\n");
                        break;
                    }
                    case 4: {
                        int i = 0;
                        char nomeTemp1[50], nomeTemp2[50];                        
                        
                        // Abrindo o arquivo produtos em formato de leitura
                        FILE *arquivoProdutos = fopen("produtos.txt", "r");
                        if (arquivoProdutos == NULL) {
                            printf("Nenhum produto cadastrado.\n");
                            break;
                        }
                        
                        // COLETANDO QUANTIDADE DE PRODUTOS
                        while (fgets(linha, sizeof(linha), arquivoProdutos)) {
                            int itensLidos = sscanf(linha, "%d,%49[^,],%49[^,],%f,%d,%49[^\n]",  // O sscanf está separando os valores da string nas variáveis (explicado no case 2)
                                                    &id, nome, tipo, &preco, &quantidade, fornecedor);

                            if (itensLidos == 6) {
                                listaProdutos[i].idProduto = id;
                                strcpy(listaProdutos[i].nomeProduto, nome);
                                strcpy(listaProdutos[i].tipoProduto, tipo);
                                listaProdutos[i].precoProduto = preco;
                                listaProdutos[i].quantidadeProduto = quantidade;
                                strcpy(listaProdutos[i].fornecedorProduto, fornecedor);
                                
                                i++;
                            }
                            else {
                                printf("Erro de formatação de arquivo.");
                                break;
                            }
                        }
                        fclose(arquivoProdutos);

                        int numProdutos = i;

                        struct Produtos temp; // Variável temporária para troca
                        int j, k;
                        
                        // BUBBLE SORT + Comparação de letra:
                        // Loop externo: (percorre o array do início ao fim)
                        for (j = 0; j < numProdutos - 1; j++) {
                            // Loop interno: (compara e troca elementos adjacentes)
                            for (k = 0; k < numProdutos - 1 - j; k++) {
                                
                                // Comparação: Verifica se o produto K é "maior" que o produto K+1
                                // Usamos strcmp: se > 0, o produto está em ordem errada (ex: Z vem antes de A)

                                // Transformando as duas letras comparadas para case insensitive
                                strcpy(nomeTemp1, listaProdutos[k].nomeProduto);
                                toLower(nomeTemp1);

                                strcpy(nomeTemp2, listaProdutos[k+1].nomeProduto);
                                toLower(nomeTemp2);

                                if (strcmp(nomeTemp1, nomeTemp2) < 0) {
                                    
                                    // TROCA COMPLETA DO STRUCT:
                                    temp = listaProdutos[k];
                                    listaProdutos[k] = listaProdutos[k+1];
                                    listaProdutos[k+1] = temp;
                                }
                            }
                        }

                        printf("-------------------------------------------------------------------------------------------------\n");
                        printf("| ID | Produto           | Categoria           | Preço (R$) | Quantidade | Fornecedor           |\n");
                        printf("-------------------------------------------------------------------------------------------------\n");

                        for (j = 0; j < numProdutos; j++) {
                            printf("| %-2d | %-17s | %-19s | %-10.2f | %-10d | %-20s |\n",
                                    listaProdutos[j].idProduto,
                                    listaProdutos[j].nomeProduto,
                                    listaProdutos[j].tipoProduto,
                                    listaProdutos[j].precoProduto,
                                    listaProdutos[j].quantidadeProduto,
                                    listaProdutos[j].fornecedorProduto);
                        }
                        printf("-------------------------------------------------------------------------------------------------\n\n");
                        break;
                    }
                    case 5: {
                        int i = 0;
                        char nomeTemp1[50], nomeTemp2[50];                        
                        
                        // Abrindo o arquivo produtos em formato de leitura
                        FILE *arquivoProdutos = fopen("produtos.txt", "r");
                        if (arquivoProdutos == NULL) {
                            printf("Nenhum produto cadastrado.\n");
                            break;
                        }
                        
                        // COLETANDO QUANTIDADE DE PRODUTOS
                        while (fgets(linha, sizeof(linha), arquivoProdutos)) {
                            int itensLidos = sscanf(linha, "%d,%49[^,],%49[^,],%f,%d,%49[^\n]",  // O sscanf está separando os valores da string nas variáveis (explicado no case 2)
                                                    &id, nome, tipo, &preco, &quantidade, fornecedor);

                            if (itensLidos == 6) {
                                listaProdutos[i].idProduto = id;
                                strcpy(listaProdutos[i].nomeProduto, nome);
                                strcpy(listaProdutos[i].tipoProduto, tipo);
                                listaProdutos[i].precoProduto = preco;
                                listaProdutos[i].quantidadeProduto = quantidade;
                                strcpy(listaProdutos[i].fornecedorProduto, fornecedor);
                                
                                i++;
                            }
                            else {
                                printf("Erro de formatação de arquivo.");
                                break;
                            }
                        }
                        fclose(arquivoProdutos);

                        int numProdutos = i;

                        struct Produtos temp; // Variável temporária para troca
                        int j, k;
                        
                        // BUBBLE SORT + Comparação de letra:
                        // Loop externo: (percorre o array do início ao fim)
                        for (j = 0; j < numProdutos - 1; j++) {
                            // Loop interno: (compara e troca elementos adjacentes)
                            for (k = 0; k < numProdutos - 1 - j; k++) {
                                
                                // Comparação: Verifica se o produto K é "maior" que o produto K+1
                                if (listaProdutos[k].precoProduto < listaProdutos[k + 1].precoProduto) {
                                    
                                    // TROCA COMPLETA DO STRUCT:
                                    temp = listaProdutos[k];
                                    listaProdutos[k] = listaProdutos[k+1];
                                    listaProdutos[k+1] = temp;
                                }
                            }
                        }

                        printf("-------------------------------------------------------------------------------------------------\n");
                        printf("| ID | Produto           | Categoria           | Preço (R$) | Quantidade | Fornecedor           |\n");
                        printf("-------------------------------------------------------------------------------------------------\n");

                        for (j = 0; j < numProdutos; j++) {
                            printf("| %-2d | %-17s | %-19s | %-10.2f | %-10d | %-20s |\n",
                                    listaProdutos[j].idProduto,
                                    listaProdutos[j].nomeProduto,
                                    listaProdutos[j].tipoProduto,
                                    listaProdutos[j].precoProduto,
                                    listaProdutos[j].quantidadeProduto,
                                    listaProdutos[j].fornecedorProduto);
                        }
                        printf("-------------------------------------------------------------------------------------------------\n\n");
                        break;
                    }
                    case 6: {
                        int i = 0;
                        char nomeTemp1[50], nomeTemp2[50];                        
                        
                        // Abrindo o arquivo produtos em formato de leitura
                        FILE *arquivoProdutos = fopen("produtos.txt", "r");
                        if (arquivoProdutos == NULL) {
                            printf("Nenhum produto cadastrado.\n");
                            break;
                        }
                        
                        // COLETANDO QUANTIDADE DE PRODUTOS
                        while (fgets(linha, sizeof(linha), arquivoProdutos)) {
                            int itensLidos = sscanf(linha, "%d,%49[^,],%49[^,],%f,%d,%49[^\n]",  // O sscanf está separando os valores da string nas variáveis (explicado no case 2)
                                                    &id, nome, tipo, &preco, &quantidade, fornecedor);

                            if (itensLidos == 6) {
                                listaProdutos[i].idProduto = id;
                                strcpy(listaProdutos[i].nomeProduto, nome);
                                strcpy(listaProdutos[i].tipoProduto, tipo);
                                listaProdutos[i].precoProduto = preco;
                                listaProdutos[i].quantidadeProduto = quantidade;
                                strcpy(listaProdutos[i].fornecedorProduto, fornecedor);
                                
                                i++;
                            }
                            else {
                                printf("Erro de formatação de arquivo.");
                                break;
                            }
                        }
                        fclose(arquivoProdutos);

                        int numProdutos = i;

                        struct Produtos temp; // Variável temporária para troca
                        int j, k;
                        
                        // BUBBLE SORT + Comparação de letra:
                        // Loop externo: (percorre o array do início ao fim)
                        for (j = 0; j < numProdutos - 1; j++) {
                            // Loop interno: (compara e troca elementos adjacentes)
                            for (k = 0; k < numProdutos - 1 - j; k++) {
                                
                                // Comparação: Verifica se o produto K é "maior" que o produto K+1
                                if (listaProdutos[k].precoProduto > listaProdutos[k + 1].precoProduto) {
                                    
                                    // TROCA COMPLETA DO STRUCT:
                                    temp = listaProdutos[k];
                                    listaProdutos[k] = listaProdutos[k+1];
                                    listaProdutos[k+1] = temp;
                                }
                            }
                        }

                        printf("-------------------------------------------------------------------------------------------------\n");
                        printf("| ID | Produto           | Categoria           | Preço (R$) | Quantidade | Fornecedor           |\n");
                        printf("-------------------------------------------------------------------------------------------------\n");

                        for (j = 0; j < numProdutos; j++) {
                            printf("| %-2d | %-17s | %-19s | %-10.2f | %-10d | %-20s |\n",
                                    listaProdutos[j].idProduto,
                                    listaProdutos[j].nomeProduto,
                                    listaProdutos[j].tipoProduto,
                                    listaProdutos[j].precoProduto,
                                    listaProdutos[j].quantidadeProduto,
                                    listaProdutos[j].fornecedorProduto);
                        }
                        printf("-------------------------------------------------------------------------------------------------\n\n");
                        break;
                    }
                }
                break;
            }
            case 6: {
                printf("Programa finalizado com sucesso.");
                return 0;
            }
        }
    }
}