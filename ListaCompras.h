#ifndef LISTACOMPRAS_H
#define LISTACOMPRAS_H

#include <vector>
#include <map>
#include <list>
#include <string>
using namespace std;

typedef struct {
    char cod_cliente[50];
    char cod_produto[50];
    char nome_produto[100];
} Leitura;

typedef struct {
    vector<string> vetorClientes;
    map<string, int> mapaClientes;

    vector<string> vetorProdutos;
    map<string, int> mapaProdutos;

    vector<list<int>> listaCompras;
} ListaCompras;

int criaListaCompras(char *nome_arquivo, ListaCompras *lista);
int mostrarProdutosCliente(ListaCompras *lista, char *codigoCliente);

#endif