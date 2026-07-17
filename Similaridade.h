#ifndef SIMILARIDADE_H
#define SIMILARIDADE_H

#include <iostream>
#include "ListaCompras.h"

typedef struct {
    double **A;
    double **At;
    double **I;
    double **S;
    int *P;
    int m;
    int n;
} Similaridade;

int criaMatrizCompras(ListaCompras *lista, Similaridade *sim);
int criaTranspostaCompras(Similaridade *sim);
int criaMatrizIntersecao(Similaridade *sim);
int criaVetorP(ListaCompras *lista, Similaridade *sim);
int criaMatrizSimilaridade(Similaridade *sim);
double calculaSimilaridade(Similaridade *sim, int i, int j);
void testadorExibeSimilaridade (Similaridade *sim, ListaCompras *lista, int indCliente);

#endif