#ifndef SIMILARIDADE_H
#define SIMILARIDADE_H

#include <vector>
#include <iostream>
#include "ListaCompras.h"

typedef struct {
    std::vector<int> values;
    std::vector<int> col_index;
    std::vector<int> row_ptr;
    int n_linhas;
    int n_colunas;
} MatrizCSR;

typedef struct {
    double **A;
    double **At;
    double **I;
    double **S;
    int *P;
    int m;
    int n;
    double tempo;

    MatrizCSR comprasCSR;
    MatrizCSR intersecaoCSR;
    MatrizCSR similaridadeCSR;
} Similaridade;

double **criaMatrizDouble(int linhas, int colunas);
int criaMatrizCompras(ListaCompras *lista, Similaridade *sim);
int criaTranspostaCompras(Similaridade *sim);
void liberaMatrizDouble(double **matriz, int linhas);
int criaMatrizIntersecao(Similaridade *sim);
int criaIntersecaoEficiente(Similaridade *sim);
int criaVetorP(ListaCompras *lista, Similaridade *sim);
int criaMatrizSimilaridade(ListaCompras *lista, Similaridade *sim, int alg);
double calculaSimilaridade(Similaridade *sim, int i, int j);
void testadorExibeSimilaridade (Similaridade *sim, ListaCompras *lista, int indCliente);
void exibeTempoExecucao(Similaridade *sim, const char *nome);

#endif