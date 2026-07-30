#ifndef CSR_H
#define CSR_H

#include <vector>
#include "ListaCompras.h"

typedef struct {
    std::vector<double> values;
    std::vector<int> col_index;
    std::vector<int> row_ptr;
} MatrizCSR;

typedef struct {
    MatrizCSR comprasCSR;
    MatrizCSR intersecaoCSR;
    MatrizCSR similaridadeCSR;
    std::vector<int> P;
    int m;
    int n;
    double tempo;
} SimilaridadeCSR;

void criaMatrizComprasCSR (ListaCompras *lista, MatrizCSR *A, SimilaridadeCSR *simCSR);
void criaIntersecaoCSR(MatrizCSR *A, MatrizCSR *C, SimilaridadeCSR *simCSR);
void criaMatrizSimilaridadeCSR (ListaCompras *lista, SimilaridadeCSR *simCSR);
double calculaSimilaridadeCSR (SimilaridadeCSR *simCSR, int i, int j);
void testadorExibeSimilaridadeCSR (ListaCompras *lista, SimilaridadeCSR *simCSR, int indCliente);
void comparaMemoria(SimilaridadeCSR *simCSR);

#endif