#ifndef SIMILARIDADE_H
#define SIMILARIDADE_H

#include "ListaCompras.h"

typedef struct {
    double **A;
    double **At;
    double **I;
    double **S;
    int *P;
    int m;
    int n;
    double tempo;
} Similaridade;

int criaMatrizCompras(ListaCompras *lista, Similaridade *sim);
int criaTranspostaCompras(Similaridade *sim);
void liberaMatrizDouble(double **matriz, int linhas);
int criaMatrizIntersecao(Similaridade *sim);
int criaIntersecaoEficiente(Similaridade *sim);
int criaVetorP(ListaCompras *lista, Similaridade *sim);
int criaMatrizSimilaridade(ListaCompras *lista, Similaridade *sim, int alg);
double calculaSimilaridade(Similaridade *sim, int i, int j);
void testadorExibeSimilaridade (Similaridade *sim, ListaCompras *lista, int indCliente);
void exibeTempoExecucao(Similaridade *sim);

#endif