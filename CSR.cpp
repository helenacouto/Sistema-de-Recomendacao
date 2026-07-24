#include <stdio.h>
#include "Similaridade.h"


void criaIntersecaoCSR(MatrizCSR *A, MatrizCSR *C, Similaridade *sim) {
    C->values.clear();
    C->col_index.clear();

    C->row_ptr.clear();
    for (int i = 0; i <= sim->n; i++) {
        C->row_ptr.push_back(0);
    }

    int cont = 0;

    for (int i =0; i < sim->n; i++) {
        if (A->row_ptr[i] == A->row_ptr[i+1]) {
            C->row_ptr[i + 1] = cont;
            continue;
        }

        for ( int j = 0; j < sim->n; j++) {
            if (j == i) continue;

            int soma = 0;
            int p = A->row_ptr[i];
            int q = A->row_ptr[j];
            int fimLinhaI = A->row_ptr[i + 1];
            int fimLinhaJ = A->row_ptr[j + 1];

            while (p < fimLinhaI && q < fimLinhaJ) {
                if (A->col_index[p] == A->col_index[q]) {
                    soma += A->values[p] * A->values[q];
                    p++;
                    q++;
                } else if (A->col_index[p] < A->col_index[q]) {
                    p++;
                } else {
                    q++;
                }
            }

            if (soma != 0) {
                C->values.push_back(soma);
                C->col_index.push_back(j);
                cont++;
            }
        }

        C->row_ptr[i + 1] = cont;
    }

    return;
}