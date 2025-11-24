#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINHA 256
#define MAX_PRODUTOS 100

// Estrutura Global
struct Produtos {
    int idProduto;
    char nomeProduto[50];
    char tipoProduto[50];
    float precoProduto;
    int quantidadeProduto;
    char fornecedorProduto[50];
};

// Variável global para operações de ordenação (evita estouro de pilha em arrays muito grandes)
struct Produtos listaProdutos[MAX_PRODUTOS];

// --- FUNÇÕES UTILITÁRIAS (Helpers) ---

// Limpa o buffer do teclado (substitui aquele while repetitivo)
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Remove o \n do final das strings lidas com fgets
void removerNovaLinha(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

// Converte string para minúsculo
void toLower(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

// Gerencia a persistência do ID
int obterId() {
    FILE *arquivoId = fopen("proximoId.txt", "r");
    int proximoId = 1;
    if (arquivoId != NULL) {
        if (fscanf(arquivoId, "%d", &proximoId) != 1) proximoId = 1;
        fclose(arquivoId);
    }
    return proximoId;
}

void salvarId(int id) {
    FILE *arquivoId = fopen("proximoId.txt", "w");
    if (arquivoId != NULL) {
        fprintf(arquivoId, "%d", id);
        fclose(arquivoId);
    }
}

// --- FUNÇÕES PRINCIPAIS DO CRUD ---

void adicionarProduto(int *idGlobal) {
    struct Produtos p;
    p.idProduto = *idGlobal;

    // Atualiza o ID global e salva
    (*idGlobal)++;
    salvarId(*idGlobal);

    printf("\n--- NOVO PRODUTO ---\n");
    printf("Nome: ");
    limparBuffer(); // Garante buffer limpo antes do fgets
    fgets(p.nomeProduto, 50, stdin);
    removerNovaLinha(p.nomeProduto);

    printf("Tipo: ");
    fgets(p.tipoProduto, 50, stdin);
    removerNovaLinha(p.tipoProduto);

    printf("Preço: ");
    scanf("%f", &p.precoProduto);

    printf("Quantidade: ");
    scanf("%d", &p.quantidadeProduto);
    limparBuffer();

    printf("Fornecedor: ");
    fgets(p.fornecedorProduto, 50, stdin);
    removerNovaLinha(p.fornecedorProduto);

    FILE *arquivo = fopen("produtos.txt", "a+");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de dados.\n");
        return;
    }
    
    fprintf(arquivo, "%d,%s,%s,%.2f,%d,%s\n", 
            p.idProduto, p.nomeProduto, p.tipoProduto, p.precoProduto, p.quantidadeProduto, p.fornecedorProduto);
    fclose(arquivo);
    printf("Produto cadastrado com sucesso!\n");
}

void listarProdutos() {
    FILE *arquivo = fopen("produtos.txt", "r");
    if (arquivo == NULL) {
        printf("Nenhum produto cadastrado ou erro de leitura.\n");
        return;
    }

    char linha[MAX_LINHA];
    struct Produtos p;

    printf("-------------------------------------------------------------------------------------------------\n");
    printf("| ID | Produto           | Categoria           | Preço (R$) | Quantidade | Fornecedor           |\n");
    printf("-------------------------------------------------------------------------------------------------\n");

    while (fgets(linha, sizeof(linha), arquivo)) {
        int itens = sscanf(linha, "%d,%49[^,],%49[^,],%f,%d,%49[^\n]", 
                           &p.idProduto, p.nomeProduto, p.tipoProduto, &p.precoProduto, &p.quantidadeProduto, p.fornecedorProduto);
        if (itens == 6) {
            printf("| %-2d | %-17s | %-19s | %-10.2f | %-10d | %-20s |\n", 
                   p.idProduto, p.nomeProduto, p.tipoProduto, p.precoProduto, p.quantidadeProduto, p.fornecedorProduto);
        }
    }
    printf("-------------------------------------------------------------------------------------------------\n\n");
    fclose(arquivo);
}

void alterarProduto() {
    int idAlvo, encontrado = 0;
    char linha[MAX_LINHA];
    struct Produtos p;

    printf("ID do produto para ALTERAR: ");
    scanf("%d", &idAlvo);
    limparBuffer();

    FILE *origem = fopen("produtos.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (origem == NULL || temp == NULL) {
        printf("Erro ao manipular arquivos.\n");
        return;
    }

    while (fgets(linha, sizeof(linha), origem)) {
        int itens = sscanf(linha, "%d,%49[^,],%49[^,],%f,%d,%49[^\n]", 
                           &p.idProduto, p.nomeProduto, p.tipoProduto, &p.precoProduto, &p.quantidadeProduto, p.fornecedorProduto);

        if (itens == 6 && p.idProduto == idAlvo) {
            encontrado = 1;
            int opcao;
            printf("\n1-Nome, 2-Tipo, 3-Preço, 4-Qtd, 5-Fornecedor\nO que mudar? ");
            scanf("%d", &opcao);
            limparBuffer();

            switch(opcao) {
                case 1: printf("Novo Nome: "); fgets(p.nomeProduto, 50, stdin); removerNovaLinha(p.nomeProduto); break;
                case 2: printf("Novo Tipo: "); fgets(p.tipoProduto, 50, stdin); removerNovaLinha(p.tipoProduto); break;
                case 3: printf("Novo Preço: "); scanf("%f", &p.precoProduto); break;
                case 4: printf("Nova Qtd: "); scanf("%d", &p.quantidadeProduto); break;
                case 5: printf("Novo Fornecedor: "); fgets(p.fornecedorProduto, 50, stdin); removerNovaLinha(p.fornecedorProduto); break;
            }
        }
        // Escreve no temporário (seja o dado antigo ou o novo)
        fprintf(temp, "%d,%s,%s,%.2f,%d,%s\n", p.idProduto, p.nomeProduto, p.tipoProduto, p.precoProduto, p.quantidadeProduto, p.fornecedorProduto);
    }

    fclose(origem);
    fclose(temp);

    if (encontrado) {
        remove("produtos.txt");
        rename("temp.txt", "produtos.txt");
        printf("Alteração realizada!\n");
    } else {
        remove("temp.txt");
        printf("ID não encontrado.\n");
    }
}

void excluirProduto() {
    int idAlvo, encontrado = 0;
    char linha[MAX_LINHA];
    struct Produtos p;

    printf("ID do produto para EXCLUIR: ");
    scanf("%d", &idAlvo);
    limparBuffer();

    FILE *origem = fopen("produtos.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (origem == NULL || temp == NULL) {
        printf("Erro ao manipular arquivos.\n");
        return;
    }

    while (fgets(linha, sizeof(linha), origem)) {
        int itens = sscanf(linha, "%d,%49[^,],%49[^,],%f,%d,%49[^\n]", 
                           &p.idProduto, p.nomeProduto, p.tipoProduto, &p.precoProduto, &p.quantidadeProduto, p.fornecedorProduto);

        if (itens == 6 && p.idProduto == idAlvo) {
            encontrado = 1;
            char conf;
            printf("Tem certeza? (s/n): ");
            scanf(" %c", &conf);
            limparBuffer();

            if (conf == 's' || conf == 'S') {
                printf("Excluindo...\n");
                continue; // Pula a escrita no temp (Apaga)
            }
        }
        // Mantém o produto
        fprintf(temp, "%d,%s,%s,%.2f,%d,%s\n", p.idProduto, p.nomeProduto, p.tipoProduto, p.precoProduto, p.quantidadeProduto, p.fornecedorProduto);
    }

    fclose(origem);
    fclose(temp);

    if (encontrado) {
        remove("produtos.txt");
        rename("temp.txt", "produtos.txt");
        printf("Operação concluída.\n");
    } else {
        remove("temp.txt");
        printf("ID não encontrado.\n");
    }
}

// --- FUNÇÃO ESPECIAL: BUSCAR E ORDENAR ---

// Função auxiliar para carregar o arquivo no array (usada na busca)
int carregarDadosParaMemoria() {
    FILE *arquivo = fopen("produtos.txt", "r");
    if (arquivo == NULL) return 0;

    char linha[MAX_LINHA];
    int i = 0;
    struct Produtos p;

    while (fgets(linha, sizeof(linha), arquivo) && i < MAX_PRODUTOS) {
        int itens = sscanf(linha, "%d,%49[^,],%49[^,],%f,%d,%49[^\n]", 
                           &p.idProduto, p.nomeProduto, p.tipoProduto, &p.precoProduto, &p.quantidadeProduto, p.fornecedorProduto);
        if (itens == 6) {
            listaProdutos[i] = p;
            i++;
        }
    }
    fclose(arquivo);
    return i; // Retorna a quantidade carregada
}

void menuBuscarOrdenar() {
    int subOpcao, qtdProdutos;
    char buscaUsuario[50];
    
    // Carrega dados para memória (necessário para ordenação)
    qtdProdutos = carregarDadosParaMemoria();
    if (qtdProdutos == 0) {
        printf("Nenhum dado para processar.\n");
        return;
    }

    printf("\n--- BUSCAR E ORDENAR ---\n");
    printf("1-Nome.\n2-Tipo.\n3-Ord A-Z\n4-Ord Z-A\n5-Maior Preço\n6-Menor Preço\n");
    printf("Opção: ");
    scanf("%d", &subOpcao);
    limparBuffer();

    // Lógica de Busca (Não precisa ordenar, busca direto no array carregado)
    if (subOpcao == 1 || subOpcao == 2) {
        int achou = 0;
        printf("Digite o termo: ");
        fgets(buscaUsuario, 50, stdin);
        removerNovaLinha(buscaUsuario);
        toLower(buscaUsuario);

        printf("\n--- RESULTADOS ---\n");
        for (int i = 0; i < qtdProdutos; i++) {
            char temp[50];
            // Escolhe qual campo comparar baseado na opção
            if (subOpcao == 1) strcpy(temp, listaProdutos[i].nomeProduto);
            else strcpy(temp, listaProdutos[i].tipoProduto);
            
            toLower(temp);
            if (strstr(temp, buscaUsuario) != NULL) {
                printf("| %d | %s | %s | R$ %.2f |\n", listaProdutos[i].idProduto, listaProdutos[i].nomeProduto, listaProdutos[i].tipoProduto, listaProdutos[i].precoProduto);
                achou = 1;
            }
        }
        if (!achou) printf("Nada encontrado.\n");
        return; // Sai da função após buscar
    }

    // Lógica de Ordenação (Bubble Sort)
    struct Produtos tempStruct;
    char n1[50], n2[50];

    for (int j = 0; j < qtdProdutos - 1; j++) {
        for (int k = 0; k < qtdProdutos - 1 - j; k++) {
            int trocar = 0;

            // Configura as variáveis de comparação de string (para os casos 3 e 4)
            if (subOpcao == 3 || subOpcao == 4) {
                strcpy(n1, listaProdutos[k].nomeProduto); toLower(n1);
                strcpy(n2, listaProdutos[k+1].nomeProduto); toLower(n2);
            }

            switch (subOpcao) {
                case 3: if (strcmp(n1, n2) > 0) trocar = 1; break; // A-Z
                case 4: if (strcmp(n1, n2) < 0) trocar = 1; break; // Z-A
                case 5: if (listaProdutos[k].precoProduto < listaProdutos[k+1].precoProduto) trocar = 1; break; // Maior Preço
                case 6: if (listaProdutos[k].precoProduto > listaProdutos[k+1].precoProduto) trocar = 1; break; // Menor Preço
            }

            if (trocar) {
                tempStruct = listaProdutos[k];
                listaProdutos[k] = listaProdutos[k+1];
                listaProdutos[k+1] = tempStruct;
            }
        }
    }

    // Imprime a lista ordenada
    printf("\n--- LISTA ORDENADA ---\n");
    for (int i = 0; i < qtdProdutos; i++) {
        printf("| %-15s | R$ %-8.2f | %-15s |\n", 
               listaProdutos[i].nomeProduto, listaProdutos[i].precoProduto, listaProdutos[i].tipoProduto);
    }
}

// --- MAIN SIMPLIFICADO ---

int main() {
    int idGlobal = obterId();
    int opcao;

    while (1) {
        printf("\n=== SISTEMA DE ESTOQUE ===\n");
        printf("1. Adicionar\n2. Listar\n3. Alterar\n4. Excluir\n5. Buscar/Ordenar\n6. Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);
        limparBuffer(); // Limpeza essencial aqui

        switch (opcao) {
            case 1: adicionarProduto(&idGlobal); break;
            case 2: listarProdutos(); break;
            case 3: alterarProduto(); break;
            case 4: excluirProduto(); break;
            case 5: menuBuscarOrdenar(); break;
            case 6: 
                printf("Saindo...\n"); 
                return 0;
            default: 
                printf("Opção inválida!\n");
        }
    }
    return 0;
}