#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
// Número máximo de produtos cadastrados
#define MAX_PRODUTOS 50  
// Número máximo de produtos no carrinho
#define MAX_CARRINHO 50  

// Estrutura para armazenar informações de um produto
typedef struct {
    int codigo;
    char nome[30];
    float preco;
} Produto;

// Estrutura para armazenar um item do carrinho (produto + quantidade)
typedef struct {
    Produto produto;
    int quantidade;
} Carrinho;

// Vetores globais para armazenar os produtos cadastrados e os itens no carrinho
Produto produtos[MAX_PRODUTOS];
Carrinho carrinho[MAX_CARRINHO];
// Contadores globais
int totalProdutos = 0; 
int totalCarrinho = 0;  

// Declaração das funções
void menu();
void cadastrarProduto();
void listarProdutos();
void comprarProduto();
void visualizarCarrinho();
void removerDoCarrinho();
void fecharPedido();
int temNoCarrinho(int codigo);
Produto *pegarProdutoPorCodigo(int codigo);
void infoProduto(Produto prod);
void limparTela();
int validarEntradaInt();
float validarEntradaFloat();
void editarProduto();
void excluirProduto();
int main() {
	 // Define o idioma e o encoding para Português UTF-8, para não quebrar em carácteres especiais
    setlocale(LC_ALL, "");
    menu();  
    return 0;
}


// Função para limpar a tela
void limparTela() {
     system("cls");
}

// Função que exibe o menu de opções e processa a entrada do usuário
void menu() {
    int opcao;
    do {
        printf("\n--- Sistema de Supermercado ---\n");
        printf("1. Cadastrar Produto\n");
        printf("2. Listar Produtos\n");
        printf("3. Editar Produto\n");  // Nova opÃ§Ã£o
        printf("4. Excluir Produto\n"); // Nova opÃ§Ã£o
        printf("5. Comprar Produto\n");
        printf("6. Visualizar Carrinho\n");
        printf("7. Remover Item do Carrinho\n");
        printf("8. Fechar Pedido\n");
        printf("9. Sair\n");
        printf("Escolha uma opção: ");
        opcao = validarEntradaInt();

        switch (opcao) {
            case 1:
				cadastrarProduto();
			break;
            case 2:
				listarProdutos();
			break;
            case 3:
				editarProduto();
			break;
            case 4: 
				excluirProduto(); 
			break;
            case 5:
				comprarProduto();
			break;
            case 6:
				visualizarCarrinho();
			break;
            case 7:
				removerDoCarrinho();
			break;
            case 8: 
				fecharPedido();
			break;
            case 9:
				printf("Saindo do sistema...\n");
			break;
            default: 
				printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 9);
}

// Função para editar o nome e o preço de um produto
void editarProduto() {
    limparTela();
    listarProdutos();

    if (totalProdutos == 0) {
        printf("Nenhum produto cadastrado para editar.\n");
        return;
    }

    printf("Digite o código do produto que deseja editar: ");
    int codigo = validarEntradaInt();

    Produto *p = pegarProdutoPorCodigo(codigo);
    if (!p) {
        printf("Produto não encontrado!\n");
        return;
    }

    // Exibe as informações atuais do produto
    printf("Produto encontrado:\n");
    printf("Nome atual: %s\n", p->nome);
    printf("Preço atual: R$%.2f\n", p->preco);

    // Pergunta se o usuário deseja editar o nome
    char editarNome;
    printf("Deseja editar o nome? (s/n): ");
    scanf(" %c", &editarNome); 

    if (editarNome == 's' || editarNome == 'S') {
        printf("Digite o novo nome: ");
        scanf("%s", p->nome);
    }

    // Pergunta se o usuário deseja editar o preço
    char editarPreco;
    printf("Deseja editar o preço? (s/n): ");
    scanf(" %c", &editarPreco);

    if (editarPreco == 's' || editarPreco == 'S') {
        printf("Digite o novo preço: ");
        p->preco = validarEntradaFloat();
    }

    printf("Produto atualizado com sucesso!\n");
}

// Função para excluir um produto (do estoque e do carrinho)
void excluirProduto() {
    limparTela();
    listarProdutos();

    if (totalProdutos == 0) {
        printf("Nenhum produto cadastrado para excluir.\n");
        return;
    }

    printf("Digite o código do produto que deseja excluir: ");
    int codigo = validarEntradaInt();

    int indice = -1;
    for (int i = 0; i < totalProdutos; i++) {
        if (produtos[i].codigo == codigo) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("Produto não encontrado!\n");
        return;
    }

    // Remove o produto do estoque
    for (int i = indice; i < totalProdutos - 1; i++) {
        produtos[i] = produtos[i + 1];
    }
    totalProdutos--;

    // Remove o produto do carrinho
    for (int i = 0; i < totalCarrinho; i++) {
        if (carrinho[i].produto.codigo == codigo) {
            // Remove o produto do carrinho e reorganiza o array
            for (int j = i; j < totalCarrinho - 1; j++) {
                carrinho[j] = carrinho[j + 1];
            }
            totalCarrinho--;
            printf("Produto removido do carrinho também!\n");
            break;
        }
    }

    printf("Produto excluído com sucesso!\n");
}

// Função para validar entrada de nÃºmero inteiro
int validarEntradaInt() {
    int valor;
    while (scanf("%d", &valor) != 1) {
        printf("Entrada inválida! Digite um número válido: ");
        while (getchar() != '\n'); 
    }
    return valor;
}

// Função para validar entrada de nÃºmero float
float validarEntradaFloat() {
    float valor;
    while (scanf("%f", &valor) != 1) {
        printf("Entrada inválida! Digite um valor numérico: ");
        while (getchar() != '\n');  // Limpar buffer do teclado
    }
    return valor;
}

// Função para cadastrar um novo produto no sistema
void cadastrarProduto() {
    limparTela();
    if (totalProdutos >= MAX_PRODUTOS) {
        printf("Limite de produtos atingido!\n");
        return;
    }

    Produto p;
     printf("Cadastre um novo Produto\n");
    // Validação para não permitir códigos duplicados
    do {
        printf("Codigo: ");
        p.codigo = validarEntradaInt();

        if (pegarProdutoPorCodigo(p.codigo) != NULL) {
            printf("Erro: já existe um produto com esse cÃ³digo! Tente novamente.\n");
        }
    } while (pegarProdutoPorCodigo(p.codigo) != NULL);

    printf("Nome: ");
    scanf("%s", p.nome);

    printf("Preco: ");
    p.preco = validarEntradaFloat();

    produtos[totalProdutos++] = p; 
    limparTela();
    printf("Produto cadastrado com sucesso!\n");
}
// Função para listar todos os produtos cadastrados
void listarProdutos() {
    limparTela();

    if (totalProdutos == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    printf("\n--- Produtos Disponíveis ---\n");
    for (int i = 0; i < totalProdutos; i++) {
        infoProduto(produtos[i]);
    }
}

// Função para adicionar um produto ao carrinho de compras
void comprarProduto() {
    limparTela();
    listarProdutos();

    printf("Digite o código do produto que deseja comprar: ");
    int codigo = validarEntradaInt();

    Produto *p = pegarProdutoPorCodigo(codigo);
    if (!p) {
        printf("Produto nao encontrado!\n");
        return;
    }

    int indice = temNoCarrinho(codigo);
    if (indice != -1) {
        carrinho[indice].quantidade++;
    } else {
        if (totalCarrinho >= MAX_CARRINHO) {
            printf("Carrinho cheio!\n");
            return;
        }
        carrinho[totalCarrinho].produto = *p;
        carrinho[totalCarrinho].quantidade = 1;
        totalCarrinho++;
    }

    printf("Produto adicionado ao carrinho!\n");
}

// Função para visualizar os itens no carrinho
void visualizarCarrinho() {
    limparTela();

    if (totalCarrinho == 0) {
        printf("Carrinho vazio!\n");
        return;
    }

    printf("\n--- Carrinho ---\n");
    for (int i = 0; i < totalCarrinho; i++) {
        printf("%s - %d unidade(s) - R$%.2f\n",
               carrinho[i].produto.nome,
               carrinho[i].quantidade,
               carrinho[i].produto.preco * carrinho[i].quantidade);
    }
}

// Função para remover um item do carrinho
void removerDoCarrinho() {
    limparTela();
    listarProdutos();
    if (totalCarrinho == 0) {
        printf("Carrinho vazio!\n");
        return;
    }

    printf("Digite o código do produto a remover: ");
    int codigo = validarEntradaInt();

    int indice = temNoCarrinho(codigo);
    if (indice == -1) {
        printf("Produto nao encontrado no carrinho!\n");
        return;
    }

    if (carrinho[indice].quantidade > 1) {
        carrinho[indice].quantidade--;
    } else {
        for (int i = indice; i < totalCarrinho - 1; i++) {
            carrinho[i] = carrinho[i + 1];
        }
        totalCarrinho--;
    }

    printf("Produto removido do carrinho!\n");
}

// Função para finalizar a compra e exibir o total
void fecharPedido() {
    limparTela();

    if (totalCarrinho == 0) {
        printf("Carrinho vazio!\n");
        return;
    }

    float total = 0;
    printf("\n--- Fatura ---\n");
    for (int i = 0; i < totalCarrinho; i++) {
        printf("%s - %d unidade(s) - R$%.2f\n",
               carrinho[i].produto.nome,
               carrinho[i].quantidade,
               carrinho[i].produto.preco * carrinho[i].quantidade);
        total += carrinho[i].produto.preco * carrinho[i].quantidade;
    }

    printf("Total a pagar: R$%.2f\n", total);
    totalCarrinho = 0;
    printf("Pedido finalizado!\n");
}

// Funções auxiliares
int temNoCarrinho(int codigo) {
    for (int i = 0; i < totalCarrinho; i++) {
        if (carrinho[i].produto.codigo == codigo) {
            return i;
        }
    }
    return -1;
}

Produto *pegarProdutoPorCodigo(int codigo) {
    for (int i = 0; i < totalProdutos; i++) {
        if (produtos[i].codigo == codigo) {
            return &produtos[i];
        }
    }
    return NULL;
}

void infoProduto(Produto prod) {
    printf("Codigo: %d, Nome: %s, Preco: R$%.2f\n", prod.codigo, prod.nome, prod.preco);
}
