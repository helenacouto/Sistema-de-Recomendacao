#ifndef CSR_H
#define CSR_H

#include <vector>
#include "Similaridade.h"

typedef struct {
    std::vector<int> values;
    std::vector<int> col_index;
    std::vector<int> row_ptr;
} MatrizCSR;

int criaIntersecaoCSR(MatrizCSR *A, MatrizCSR *C, Similaridade *sim);

#endif