#ifndef CSR_H
#define CSR_H

#include <vector>
#include "Similaridade.h"

typedef struct {
    std::vector<int> values;
    std::vector<int> col_index;
    std::vector<int> row_ptr;
    int n_linhas;
    int n_colunas;
} MatrizCSR;

int criaIntersecaoCSR(MatrizCSR *matrizCompras, MatrizCSR *matrizDestino, Similaridade *sim);

#endif