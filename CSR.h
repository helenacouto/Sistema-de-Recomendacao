#ifndef CSR_H
#define CSR_H

#include <vector>
#include "ListaCompras.h"
#include "Similaridade.h"

typedef struct {
    std::vector<int> values;
    std::vector<int> col_index;
    std::vector<int> row_ptr;
    int n_linhas;
    int n_colunas;
} MatrizCSR;

void ordenaListaCompras (ListaCompras *lista, Similaridade *sim, vector<int> &vetorCompras);
int criaMatrizComprasCSR(ListaCompras *lista, Similaridade *sim, MatrizCSR *matrizDestino, vector<int> &vetorCompras);
int criaIntersecaoCSR(MatrizCSR *matrizCompras, MatrizCSR *matrizDestino, Similaridade *sim);
int criaMatrizSimilaridadeCSR (ListaCompras *lista, Similaridade *sim);
double calculaSimilaridadeCSR (Similaridade *sim, int i, int j);
void testadorExibeSimilaridadeCSR (Similaridade *sim, ListaCompras *lista, int indCliente);
void comparaMemoria (Similaridade *sim);

#endif