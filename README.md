# Sistema de Gerenciamento de Estoque

Um sistema completo de controle de estoque desenvolvido em C, com persistência de dados em arquivos texto e funcionalidades avançadas de busca e ordenação.

## Sobre o Projeto

Este repositório contém duas versões do sistema:

- **`cadastro-modulado.c`**: Versão otimizada com funções auxiliares e código mais limpo
- **`cadastro.c`**: Versão original com código mais direto, ideal para estudantes iniciantes

Ambas as versões implementam as mesmas funcionalidades, mas a versão modular oferece melhor manutenibilidade e organização do código.

## Funcionalidades

### CRUD Completo
- **Adicionar produtos** com ID auto-incremental
- **Listar todos os produtos** em formato de tabela
- **Alterar dados** de produtos existentes
- **Excluir produtos** com confirmação de segurança

### Recursos Avançados
- **Busca inteligente** por nome ou tipo (case-insensitive e busca parcial)
- **Ordenação múltipla**:
  - A-Z / Z-A (alfabética)
  - Maior/menor preço
- **Persistência de dados** em arquivo CSV
- **Sistema de ID automático** mantido entre execuções

## Estrutura de Dados

Cada produto contém:
```c
struct Produtos {
    int idProduto;              // ID único auto-incremental
    char nomeProduto[50];       // Nome do produto
    char tipoProduto[50];       // Categoria/tipo
    float precoProduto;         // Preço em reais
    int quantidadeProduto;      // Quantidade em estoque
    char fornecedorProduto[50]; // Nome do fornecedor
};
```

## Arquivos Gerados

O sistema cria automaticamente dois arquivos:

- **`produtos.txt`**: Armazena todos os produtos em formato CSV
- **`proximoId.txt`**: Mantém o controle do próximo ID disponível

## Como Usar

### Compilação

```bash
# Versão modular (recomendada)
gcc cadastro-modulado.c -o estoque

# Versão didática
gcc cadastro.c -o estoque_didatico
```

### Menu Principal

```
=== SISTEMA DE ESTOQUE ===
1. Adicionar
2. Listar
3. Alterar
4. Excluir
5. Buscar/Ordenar
6. Sair
```

## Exemplos de Uso

### Adicionar Produto
```
Nome: Notebook Dell
Tipo: Eletrônicos
Preço: 3500.00
Quantidade: 15
Fornecedor: Dell Brasil
```

### Buscar por Nome
```
Digite o termo: note
Resultado: Notebook Dell | R$ 3500.00
```

### Ordenar por Preço
Ordena automaticamente do maior para o menor (ou vice-versa) e exibe a lista formatada.

## Diferenças Entre as Versões

### Versão Modular (`cadastro.c`)
- Funções auxiliares reutilizáveis
- Código mais limpo e organizado
- Melhor tratamento de erros
- Fácil manutenção e expansão
- Comentários objetivos

### Versão Didática (`cadastro-modulado.c`)
- Código mais linear e direto
- Comentários explicativos detalhados
- Ideal para aprendizado de C
- Mais fácil de seguir linha a linha

## Tecnologias Utilizadas

- **Linguagem**: C (C99)
- **Bibliotecas**:
  - `stdio.h` - Entrada/saída
  - `stdlib.h` - Funções gerais
  - `string.h` - Manipulação de strings
  - `ctype.h` - Conversão de caracteres

## Formato de Armazenamento

Os dados são salvos em formato CSV:
```
1,Notebook Dell,Eletrônicos,3500.00,15,Dell Brasil
2,Mouse Logitech,Periféricos,89.90,50,Logitech Inc
```

## Conceitos Aplicados

Este projeto demonstra:
- Estruturas de dados (structs)
- Manipulação de arquivos
- Algoritmos de ordenação (Bubble Sort)
- Busca em strings
- Validação de entrada
- Gerenciamento de memória
- Modularização de código

## Melhorias Futuras

- [ ] Implementar algoritmos de ordenação mais eficientes (QuickSort/MergeSort)
- [ ] Adicionar relatórios de estoque baixo
- [ ] Interface gráfica (GTK ou ncurses)
- [ ] Exportação para formatos adicionais (JSON, XML)
- [ ] Sistema de backup automático
- [ ] Histórico de movimentações

## Autor

Feito por Paulo Henrique.
Projeto desenvolvido como parte de estudos na matéria da faculdade "Algoritmos e Pensamento Computacional".

## 📄 Licença

Este projeto está sob a licença MIT. Sinta-se livre para usar, modificar e distribuir.

---
