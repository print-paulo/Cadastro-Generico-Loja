#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Necessária para a função tolower()

#define MAX_LINHA 256      // Tamanho máximo para leitura de uma linha do arquivo
#define MAX_PRODUTOS 100   // Limite máximo de produtos no array para ordenação

// Definição da estrutura (Struct) que representa um Produto na memória
struct Produtos {
    int idProduto;
    char nomeProduto[50];
    char tipoProduto[50];
    float precoProduto;
    int quantidadeProduto;
    char fornecedorProduto[50];
};

// Array global para armazenar produtos durante operações que exigem memória (como ordenação)
struct Produtos listaProdutos[MAX_PRODUTOS];

// --- FUNÇÕES AUXILIARES ---

// Função para obter o próximo ID disponível (Auto-incremento persistente)
// Lê de um arquivo "proximoId.txt" para saber onde parou.
int obterId() {
    FILE *arquivoId;
    int proximoId = 1; // Padrão caso o arquivo não exista

    arquivoId = fopen("proximoId.txt", "r");

    if (arquivoId != NULL) {
        // Tenta ler o ID salvo. Se falhar, mantém 1.
        if(fscanf(arquivoId, "%d", &proximoId) != 1) {
            proximoId = 1;
        }
        fclose(arquivoId);
    }
    return(proximoId);
}

// Remove o caractere de nova linha '\n' que o fgets captura automaticamente
// Isso evita problemas de formatação e comparação de strings.
void removerNovaLinha (char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

// Converte uma string inteira para minúsculo (para buscas Case-Insensitive)
void toLower(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

// --- FUNÇÃO PRINCIPAL ---

int main () {
    // Declaração de variáveis gerais
    int opcao, i = 0, id, quantidade;
    float preco;
    int idGlobal = obterId(); // Carrega o último ID ao iniciar
    char linha[MAX_LINHA], tipo[50], nome[50], fornecedor[50];

    // Carregamento inicial dos dados do arquivo para a memória (listaProdutos)
    // Isso garante que o array comece atualizado.
    FILE *arquivoProdutos = fopen("produtos.txt", "r"); 
    if (arquivoProdutos != NULL) {
        while (fgets(linha, sizeof(linha), arquivoProdutos)) {
            // sscanf faz o "parse" da linha separada por vírgulas
            int itensLidos = sscanf(linha, "%d,%49[^,],%49[^,],%f,%d,%49[^\n]", 
                                    &id, nome, tipo, &preco, &quantidade, fornecedor);
            
            // Só adiciona ao array se a linha estiver formatada corretamente (6 campos)
            if (itensLidos == 6) {
                listaProdutos[i].idProduto = id;
                strcpy(listaProdutos[i].nomeProduto, nome);
                strcpy(listaProdutos[i].tipoProduto, tipo);
                listaProdutos[i].precoProduto = preco;
                listaProdutos[i].quantidadeProduto = quantidade;
                strcpy(listaProdutos[i].fornecedorProduto, fornecedor);
                i++; // Incrementa o contador de produtos carregados
            }
        }
        fclose(arquivoProdutos);
    }
    
    // Loop Principal do Menu (Mantém o programa rodando)
    while (1) {
        printf("\n----------TABELA DADOS ATACADÃO----------\n\n");
        printf("1 - Adicionar Produto\n");
        printf("2 - Listar Produtos\n");
        printf("3 - Alterar dados de um produto\n");
        printf("4 - Exluir Produto\n");
        printf("5 - Buscar e Ordenar\n");
        printf("6 - Sair\n\n");

        printf("Escolha a opção que desejas (1-6): ");
        scanf("%d", &opcao);

        // Limpeza de buffer essencial após ler um número e antes de ler strings/chars
        int c; while ((c = getchar()) != '\n' && c != EOF);

        switch (opcao) {
            // --- CASE 1: ADICIONAR PRODUTO ---
            case 1: {
                struct Produtos criarProduto;

                // Atualiza o ID para o próximo e salva no arquivo de IDs
                criarProduto.idProduto = idGlobal;
                idGlobal++;
                FILE *arquivoId = fopen("proximoId.txt", "w");
                if (arquivoId != NULL) {
                    fprintf(arquivoId, "%d", idGlobal);
                    fclose(arquivoId);
                } else {
                    printf("Erro ao atualizar ID.\n");
                }

                // Coleta de dados do usuário
                printf("Digite o nome do produto: \n");
                fgets(criarProduto.nomeProduto, 50, stdin);
                removerNovaLinha(criarProduto.nomeProduto);

                printf("Digite o tipo do produto: \n");
                fgets(criarProduto.tipoProduto, 50, stdin);
                removerNovaLinha(criarProduto.tipoProduto);

                printf("Digite o preço do produto: (use ponto para decimais)\n");
                scanf("%f", &criarProduto.precoProduto);

                printf("Digite a quantidade de produto no estoque: \n");
                scanf("%d", &criarProduto.quantidadeProduto);

                // Limpa buffer antes de ler a próxima string (fornecedor)
                int c; while ((c = getchar()) != '\n' && c != EOF); 

                printf("Digite qual o fornecedor do produto: \n");
                fgets(criarProduto.fornecedorProduto, 50, stdin);
                removerNovaLinha(criarProduto.fornecedorProduto);

                // Append (a+) no arquivo: adiciona ao final sem apagar o conteúdo anterior
                FILE *arquivoProdutos = fopen("produtos.txt", "a+");
                if (arquivoProdutos == NULL) {
                    printf("Erro ao abrir o arquivo de produtos.");
                    return 1;
                }
                // Salva no formato CSV (Separado por vírgulas)
                fprintf(arquivoProdutos, "%d,%s,%s,%.2f,%d,%s\n", 
                        criarProduto.idProduto, criarProduto.nomeProduto, criarProduto.tipoProduto, 
                        criarProduto.precoProduto, criarProduto.quantidadeProduto, criarProduto.fornecedorProduto);
                fclose(arquivoProdutos);
                printf("\nProduto adicionado com sucesso!\n");
                break;
            }

            // --- CASE 2: LISTAR PRODUTOS ---
            case 2: {
                FILE *arquivoProdutos = fopen("produtos.txt", "r");
                if (arquivoProdutos == NULL) {
                    printf("Nenhum produto cadastrado.\n");
                    break;
                }
                
                // Cabeçalho da tabela formatado
                printf("-------------------------------------------------------------------------------------------------\n");
                printf("| ID | Produto           | Categoria           | Preço (R$) | Quantidade | Fornecedor           |\n");
                printf("-------------------------------------------------------------------------------------------------\n");

                // Lê linha a linha e imprime formatado
                while (fgets(linha, sizeof(linha), arquivoProdutos) != NULL) {
                    int itensLidos = sscanf(linha, "%d,%49[^,],%49[^,],%f,%d,%49[^\n]", 
                                            &id, nome, tipo, &preco, &quantidade, fornecedor);
                    if (itensLidos == 6) {
                        // %-Xs alinha strings à esquerda com X espaços. %.2f limita float a 2 casas.
                        printf("| %-2d | %-17s | %-19s | %-10.2f | %-10d | %-20s |\n", 
                                id, nome, tipo, preco, quantidade, fornecedor);
                    }
                }
                printf("-------------------------------------------------------------------------------------------------\n\n");
                fclose(arquivoProdutos);
                break;
            }

            // --- CASE 3: EDITAR PRODUTO (ALTERAR) ---
            case 3 : {
                int idAlvo, encontrado = 0;
                
                printf("Escreva o ID do produto que deseja ALTERAR: \n");
                scanf("%d", &idAlvo);
                int c; while((c = getchar()) != '\n' && c != EOF); 
                
                FILE *arquivoProdutos = fopen("produtos.txt", "r");
                // Cria um arquivo temporário para escrever os dados novos + dados antigos mantidos
                FILE *arquivoTemporario = fopen("temp.txt", "w");
                
                if (arquivoProdutos == NULL || arquivoTemporario == NULL) {
                    printf("Erro ao abrir arquivos.\n");
                    break;
                }
                
                while (fgets(linha, sizeof(linha), arquivoProdutos)) {
                    int itensLidos = sscanf(linha, "%d,%49[^,],%49[^,],%f,%d,%49[^\n]", 
                                            &id, nome, tipo, &preco, &quantidade, fornecedor);
                    
                    // Se achou o ID, pergunta o que mudar
                    if (itensLidos == 6 && id == idAlvo) {
                        encontrado = 1;
                        int opcaoAlterar;
                        
                        printf("\n1 - Nome\n2 - Tipo\n3 - Preço\n4 - Quantidade\n5 - Nome do fornecedor\n\n");
                        printf("O que deseja alterar? (1-5): ");
                        scanf("%d", &opcaoAlterar);
                        while((c = getchar()) != '\n' && c != EOF); // Limpa buffer

                        switch (opcaoAlterar) {
                            case 1:
                                printf("Novo nome: "); fgets(nome, 50, stdin); removerNovaLinha(nome); break;
                            case 2:
                                printf("Novo tipo: "); fgets(tipo, 50, stdin); removerNovaLinha(tipo); break;
                            case 3:
                                printf("Novo preço: "); scanf("%f", &preco); break;
                            case 4:
                                printf("Nova quantidade: "); scanf("%d", &quantidade); break;
                            case 5:
                                printf("Novo fornecedor: "); fgets(fornecedor, 50, stdin); removerNovaLinha(fornecedor); break;
                        }
                        // Escreve os dados ATUALIZADOS no arquivo temporário
                        fprintf(arquivoTemporario, "%d,%s,%s,%.2f,%d,%s\n", id, nome, tipo, preco, quantidade, fornecedor);
                    }
                    else if (itensLidos == 6) {
                        // Se não é o ID alvo, apenas copia os dados originais para o temporário
                        fprintf(arquivoTemporario, "%d,%s,%s,%.2f,%d,%s\n", id, nome, tipo, preco, quantidade, fornecedor);
                    }
                }
                
                fclose(arquivoProdutos);
                fclose(arquivoTemporario);

                // Substitui o arquivo original pelo temporário atualizado
                if (encontrado) {
                    remove("produtos.txt");
                    rename("temp.txt", "produtos.txt");
                    printf("\nAlteração realizada com sucesso!\n");
                } else {
                    printf("\nID não encontrado.\n");
                    remove("temp.txt");
                }
                break;
            }

            // --- CASE 4: EXCLUIR PRODUTO ---
            case 4: {
                int idAlvo, encontrado = 0;

                printf("Escreva o ID do produto que deseja EXCLUIR: \n");
                scanf("%d", &idAlvo);
                int c; while((c = getchar()) != '\n' && c != EOF);

                FILE *arquivoProdutos = fopen("produtos.txt", "r");
                FILE *arquivoTemporario = fopen("temp.txt", "w");
                
                if (arquivoProdutos == NULL || arquivoTemporario == NULL) {
                    printf("Erro ao manipular arquivos.\n");
                    break;
                }

                while (fgets(linha, sizeof(linha), arquivoProdutos)) {
                    int itensLidos = sscanf(linha, "%d,%49[^,],%49[^,],%f,%d,%49[^\n]", 
                                            &id, nome, tipo, &preco, &quantidade, fornecedor);
                    
                    if (itensLidos == 6 && id == idAlvo) {
                        encontrado = 1;
                        char conf;
                        printf("TEM CERTEZA QUE DESEJA APAGAR O PRODUTO? (s/n): ");
                        scanf(" %c", &conf); // Espaço antes de %c ignora espaços em branco
                        int c; while ((c = getchar()) != '\n' && c != EOF);

                        if (conf == 's' || conf == 'S') {
                            // Se confirmar, NÃO escreve no temporário (pula = apaga)
                            printf("Produto excluído.\n");
                            continue; 
                        } else {
                            // Se negar, escreve no arquivo (mantém)
                            fprintf(arquivoTemporario, "%d,%s,%s,%.2f,%d,%s\n", id, nome, tipo, preco, quantidade, fornecedor);
                        }
                    }
                    else if (itensLidos == 6) {
                        // Copia os outros produtos normalmente
                        fprintf(arquivoTemporario, "%d,%s,%s,%.2f,%d,%s\n", id, nome, tipo, preco, quantidade, fornecedor);
                    }
                }
                
                fclose(arquivoProdutos);
                fclose(arquivoTemporario);

                if (encontrado) {
                    remove("produtos.txt");
                    rename("temp.txt", "produtos.txt");
                } else {
                    printf("ID não encontrado.\n");
                    remove("temp.txt");
                }
                break;
            }

            // --- CASE 5: BUSCA E ORDENAÇÃO ---
            case 5: {
                int subOpcao, encontrado = 0;
                char buscaUsuario[50];

                printf("\n----------------- MENU BUSCAR -------------------\n");
                printf("1 - Buscar por Nome\n");
                printf("2 - Buscar por Tipo\n");
                printf("3 - Ordenar A-Z (Nome)\n");
                printf("4 - Ordenar Z-A (Nome)\n");
                printf("5 - Maior preço para o menor\n");
                printf("6 - Menor preço para o maior\n");
                printf("-------------------------------------------------\n");
                
                printf("Escolha uma opção: ");
                scanf("%d", &subOpcao);
                int c; while ((c = getchar()) != '\n' && c != EOF);

                // Abre o arquivo para leitura (usado por todas as sub-opções)
                FILE *arquivoProdutos = fopen("produtos.txt", "r");
                if (arquivoProdutos == NULL) {
                    printf("Nenhum produto cadastrado.\n");
                    break;
                }

                switch (subOpcao) {
                    // SUB 1: Buscar por NOME (Parcial e Case-Insensitive)
                    case 1: {
                        char nome_temp[50];
                        printf("Digite o nome para pesquisar: ");
                        fgets(buscaUsuario, 50, stdin);
                        removerNovaLinha(buscaUsuario);
                        toLower(buscaUsuario);

                        printf("\nResultados:\n");
                        // Lê linha a linha sem carregar tudo na memória
                        while (fgets(linha, sizeof(linha), arquivoProdutos)) {
                            sscanf(linha, "%d,%49[^,],%49[^,],%f,%d,%49[^\n]", &id, nome, tipo, &preco, &quantidade, fornecedor);
                            
                            // Cria cópia temporária para não alterar o original ao converter para minúsculo
                            strcpy(nome_temp, nome);
                            toLower(nome_temp);

                            // strstr verifica se buscaUsuario existe dentro de nome_temp
                            if (strstr(nome_temp, buscaUsuario) != NULL) {
                                printf("| %d | %s | %s | R$ %.2f |\n", id, nome, tipo, preco);
                                encontrado = 1;
                            }
                        }
                        if (!encontrado) printf("Nenhum registro encontrado.\n");
                        break;
                    }

                    // SUB 2: Buscar por TIPO (Parcial e Case-Insensitive)
                    case 2: {
                        char tipo_temp[50];
                        printf("Digite o tipo para pesquisar: ");
                        fgets(buscaUsuario, 50, stdin);
                        removerNovaLinha(buscaUsuario);
                        toLower(buscaUsuario);

                        printf("\nResultados:\n");
                        while (fgets(linha, sizeof(linha), arquivoProdutos)) {
                            sscanf(linha, "%d,%49[^,],%49[^,],%f,%d,%49[^\n]", &id, nome, tipo, &preco, &quantidade, fornecedor);
                            
                            strcpy(tipo_temp, tipo);
                            toLower(tipo_temp);

                            if (strstr(tipo_temp, buscaUsuario) != NULL) {
                                printf("| %d | %s | %s | R$ %.2f |\n", id, nome, tipo, preco);
                                encontrado = 1;
                            }
                        }
                        if (!encontrado) printf("Nenhum registro encontrado.\n");
                        break;
                    }

                    // SUB 3: ORDENAR A-Z (Crescente por Nome)
                    case 3: {
                        int count = 0;
                        // Carrega arquivo para memória (array) para poder ordenar
                        rewind(arquivoProdutos); // Volta ponteiro do arquivo para o início (se já lido) ou garante posição
                        // Nota: Melhor reabrir ou garantir rewind. Aqui estamos lendo do zero.
                        
                        while (fgets(linha, sizeof(linha), arquivoProdutos)) {
                            if(sscanf(linha, "%d,%49[^,],%49[^,],%f,%d,%49[^\n]", 
                                &listaProdutos[count].idProduto, listaProdutos[count].nomeProduto, 
                                listaProdutos[count].tipoProduto, &listaProdutos[count].precoProduto, 
                                &listaProdutos[count].quantidadeProduto, listaProdutos[count].fornecedorProduto) == 6) {
                                count++;
                            }
                        }

                        // Bubble Sort A-Z
                        struct Produtos temp;
                        char n1[50], n2[50];
                        for (int j = 0; j < count - 1; j++) {
                            for (int k = 0; k < count - 1 - j; k++) {
                                strcpy(n1, listaProdutos[k].nomeProduto); toLower(n1);
                                strcpy(n2, listaProdutos[k+1].nomeProduto); toLower(n2);
                                
                                // Se n1 for maior que n2 alfabeticamente, troca
                                if (strcmp(n1, n2) > 0) {
                                    temp = listaProdutos[k];
                                    listaProdutos[k] = listaProdutos[k+1];
                                    listaProdutos[k+1] = temp;
                                }
                            }
                        }
                        // Imprime lista ordenada
                        for(int j=0; j<count; j++) printf("| %s | R$ %.2f |\n", listaProdutos[j].nomeProduto, listaProdutos[j].precoProduto);
                        break;
                    }

                    // SUB 4: ORDENAR Z-A (Decrescente por Nome)
                    case 4: {
                        int count = 0;
                        rewind(arquivoProdutos);
                        while (fgets(linha, sizeof(linha), arquivoProdutos)) {
                            if(sscanf(linha, "%d,%49[^,],%49[^,],%f,%d,%49[^\n]", 
                                &listaProdutos[count].idProduto, listaProdutos[count].nomeProduto, 
                                listaProdutos[count].tipoProduto, &listaProdutos[count].precoProduto, 
                                &listaProdutos[count].quantidadeProduto, listaProdutos[count].fornecedorProduto) == 6) {
                                count++;
                            }
                        }

                        struct Produtos temp;
                        char n1[50], n2[50];
                        for (int j = 0; j < count - 1; j++) {
                            for (int k = 0; k < count - 1 - j; k++) {
                                strcpy(n1, listaProdutos[k].nomeProduto); toLower(n1);
                                strcpy(n2, listaProdutos[k+1].nomeProduto); toLower(n2);
                                
                                // Inverso: Se n1 for MENOR (<) que n2, troca
                                if (strcmp(n1, n2) < 0) {
                                    temp = listaProdutos[k];
                                    listaProdutos[k] = listaProdutos[k+1];
                                    listaProdutos[k+1] = temp;
                                }
                            }
                        }
                        for(int j=0; j<count; j++) printf("| %s | R$ %.2f |\n", listaProdutos[j].nomeProduto, listaProdutos[j].precoProduto);
                        break;
                    }

                    // SUB 5: MAIOR PREÇO PARA MENOR (Decrescente numérico)
                    case 5: {
                        int count = 0;
                        rewind(arquivoProdutos);
                        while (fgets(linha, sizeof(linha), arquivoProdutos)) {
                            if(sscanf(linha, "%d,%49[^,],%49[^,],%f,%d,%49[^\n]", 
                                &listaProdutos[count].idProduto, listaProdutos[count].nomeProduto, 
                                listaProdutos[count].tipoProduto, &listaProdutos[count].precoProduto, 
                                &listaProdutos[count].quantidadeProduto, listaProdutos[count].fornecedorProduto) == 6) {
                                count++;
                            }
                        }

                        struct Produtos temp;
                        for (int j = 0; j < count - 1; j++) {
                            for (int k = 0; k < count - 1 - j; k++) {
                                // Se preço atual for MENOR que o próximo, troca (para o menor ir pro final)
                                if (listaProdutos[k].precoProduto < listaProdutos[k+1].precoProduto) {
                                    temp = listaProdutos[k];
                                    listaProdutos[k] = listaProdutos[k+1];
                                    listaProdutos[k+1] = temp;
                                }
                            }
                        }
                        for(int j=0; j<count; j++) printf("| %s | R$ %.2f |\n", listaProdutos[j].nomeProduto, listaProdutos[j].precoProduto);
                        break;
                    }

                    // SUB 6: MENOR PREÇO PARA MAIOR (Crescente numérico)
                    case 6: {
                        int count = 0;
                        rewind(arquivoProdutos);
                        while (fgets(linha, sizeof(linha), arquivoProdutos)) {
                            if(sscanf(linha, "%d,%49[^,],%49[^,],%f,%d,%49[^\n]", 
                                &listaProdutos[count].idProduto, listaProdutos[count].nomeProduto, 
                                listaProdutos[count].tipoProduto, &listaProdutos[count].precoProduto, 
                                &listaProdutos[count].quantidadeProduto, listaProdutos[count].fornecedorProduto) == 6) {
                                count++;
                            }
                        }

                        struct Produtos temp;
                        for (int j = 0; j < count - 1; j++) {
                            for (int k = 0; k < count - 1 - j; k++) {
                                // Se preço atual for MAIOR que o próximo, troca
                                if (listaProdutos[k].precoProduto > listaProdutos[k+1].precoProduto) {
                                    temp = listaProdutos[k];
                                    listaProdutos[k] = listaProdutos[k+1];
                                    listaProdutos[k+1] = temp;
                                }
                            }
                        }
                        for(int j=0; j<count; j++) printf("| %s | R$ %.2f |\n", listaProdutos[j].nomeProduto, listaProdutos[j].precoProduto);
                        break;
                    }
                }
                fclose(arquivoProdutos); // Fecha o arquivo aberto no início do Case 5
                break;
            }

            // --- CASE 6: SAIR ---
            case 6: {
                printf("Programa finalizado com sucesso.\n");
                return 0;
            }
            
            default:
                printf("Opção inválida.\n");
        }
    }
}