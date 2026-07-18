#ifndef RECOMENDACAO_H
#define RECOMENDACAO_H

#include "ListaCompras.h"
#include "Similaridade.h"

typedef struct {
    int indProduto;
    double valor;
} Produto;

typedef struct {
    std::list<int> L;
    std::vector<Produto> R;
} Recomendacao;

void criaClientesSimilares(Similaridade *sim, Recomendacao *rec, int c);
void criaVetorRanqueamento(Similaridade *sim, Recomendacao *rec);

#endif