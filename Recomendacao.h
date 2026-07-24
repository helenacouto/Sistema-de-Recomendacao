#ifndef RECOMENDACAO_H
#define RECOMENDACAO_H

#include "ListaCompras.h"
#include "Similaridade.h"
#include "CSR.h"

typedef struct {
    int indProduto;
    double valor;
} Rank;

typedef struct {
    std::list<int> L;
    std::vector<Rank> R;
} Recomendacao;

void criaClientesSimilares(Similaridade *sim, Recomendacao *rec, int c);
void criaVetorRanqueamento(int qntdProdutos, Recomendacao *rec);
void calculaRanqueamento(Similaridade *sim, ListaCompras *lista, Recomendacao *rec, int indCliente);
void ordenaRanqueamento(Recomendacao *rec);
void recomendaProdutos(ListaCompras *lista, Similaridade *sim, char *codigo, int k);
void testadorExibeRecomendados(ListaCompras *lista, Similaridade *sim, int k);
void calculaRanqueamentoCSR(SimilaridadeCSR *simCSR, ListaCompras *lista, Recomendacao *rec, int c);
void recomendaProdutosCSR(ListaCompras *lista, SimilaridadeCSR *simCSR, char *codigo, int k);
void testadorExibeRecomendadosCSR(ListaCompras *lista, SimilaridadeCSR *simCSR, int k);

#endif