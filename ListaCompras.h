#ifndef LISTACOMPRAS_H
#define LISTACOMPRAS_H

#include <vector>
#include <map>
#include <list>
#include <string>
#include <iostream>

typedef struct {
    char cod_cliente[50];
    char cod_produto[50];
    char nome_produto[50];
} Leitura;

typedef struct {
    std::vector<std::string> vetorClientes;
    std::map<std::string, int> mapaClientes;

    std::vector<std::string> vetorProdutos;
    std::map<std::string, int> mapaProdutos;

    std::vector<std::string> nomeProdutos;
    std::vector<std::list<int>> listaCompras;
} ListaCompras;

int jaComprou(ListaCompras *lista, int c, int p);
int criaListaCompras(char *nome_arquivo, ListaCompras *lista);
void mostrarProdutosCliente(ListaCompras *lista, char *codigoCliente);
void testadorExibeProdutos(ListaCompras *lista);

#endif