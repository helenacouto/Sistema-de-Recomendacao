#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ListaCompras.h"
#include <vector>
#include <map>
#include <string>

int criaListaCompras(char *arquivoEscolhido, ListaCompras *lista) {
    Leitura compra;
    FILE *arquivo;

    arquivo = fopen(arquivoEscolhido, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo.");
        return false;
    }
    
    fscanf(arquivo, "%*[^\n]\n");

    while(fscanf(arquivo, "%*[^,], %49[^,] , %49[^,] , %99[^\n]\n",
                  compra.cod_cliente,
                  compra.cod_produto,
                  compra.nome_produto) == 3) {

        if (lista->mapaClientes.find(compra.cod_cliente) == lista->mapaClientes.end()) {
            lista->mapaClientes[compra.cod_cliente] = lista->vetorClientes.size();
            lista->vetorClientes.push_back(compra.cod_cliente);
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

    while(fscanf(arquivo, "%*[^,], %49[^,] , %49[^,] , %99[^\n]\n",
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

int mostrarProdutosCliente(ListaCompras *lista, char *codigoCliente) {

}