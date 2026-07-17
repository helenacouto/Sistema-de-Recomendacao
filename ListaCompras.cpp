#include <stdio.h>
#include "ListaCompras.h"

using namespace std;

int criaListaCompras(char *arquivoEscolhido, ListaCompras *lista) {
    Leitura compra;
    FILE *arquivo;
                    
    arquivo = fopen(arquivoEscolhido, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo.");
        return false;
    }
    
    fscanf(arquivo, "%*[^\n]\n");

    while(fscanf(arquivo, "%*[^,], %49[^,] , %49[^,] , %49[^\n]\n",
                  compra.cod_cliente,
                  compra.cod_produto,
                  compra.nome_produto) == 3) {

        if (lista->mapaClientes.find(compra.cod_cliente) == lista->mapaClientes.end()) {
            lista->mapaClientes[compra.cod_cliente] = lista->vetorClientes.size();
            lista->vetorClientes.push_back(compra.cod_cliente);
            lista->listaCompras.push_back(list<int>());
        }

        if (lista->mapaProdutos.find(compra.cod_produto) == lista->mapaProdutos.end()) {
            lista->mapaProdutos[compra.cod_produto] = lista->vetorProdutos.size();
            lista->vetorProdutos.push_back(compra.nome_produto);
        }
    }  
    fclose(arquivo);

    arquivo = fopen(arquivoEscolhido, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo.");
        return false;
    }

    fscanf(arquivo, "%*[^\n]\n");
    int indCliente, indProduto;

    while(fscanf(arquivo, "%*[^,], %49[^,] , %49[^,] , %49[^\n]\n",
                  compra.cod_cliente,
                  compra.cod_produto,
                  compra.nome_produto) == 3) {

        indCliente = lista->mapaClientes[compra.cod_cliente];
        indProduto = lista->mapaProdutos[compra.cod_produto];

        lista->listaCompras[indCliente].push_back(indProduto);
    }
    fclose(arquivo);
    return true;
}

void mostrarProdutosCliente(ListaCompras *lista, char *codigoCliente) {
    int indCliente;

    if (lista->mapaClientes.find(codigoCliente) == lista->mapaClientes.end()) {
        printf("Cliente %s nao esta registrado.", codigoCliente);
        return;
    }

    indCliente = lista->mapaClientes[codigoCliente];

    printf("\nProdutos que o cliente %s comprou:\n", codigoCliente);
    for (int produto : lista->listaCompras[indCliente]) {
        cout << "- " << lista->vetorProdutos[produto] << endl;
    }
}

void testadorExibeProdutos(ListaCompras *lista) {
    char cliente1[] = "99CL9Y01";
    char cliente2[] = "9O6OSM01";
    char cliente3[] = "99EF7201";

    mostrarProdutosCliente(lista, cliente1);
    mostrarProdutosCliente(lista, cliente2);
    mostrarProdutosCliente(lista, cliente3);
}