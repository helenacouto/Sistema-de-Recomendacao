#include <stdio.h>
#include "Similaridade.h"


int criaIntersecaoCSR(MatrizCSR *matrizCompras, MatrizCSR *matrizDestino, Similaridade *sim) {
    matrizDestino->n_linhas = sim->n;
    matrizDestino->n_colunas = sim->n;

    matrizDestino->row_ptr.push_back(0);

    int cont = 0;

    for (int i = 0; i < sim->n; i ++) {
        for (int j = 0; j < sim->n; j++) {
            if (i == j) continue;

            int indLinhaI = matrizCompras->row_ptr[i];
            int fimLinhaI = matrizCompras->row_ptr[i + 1];
            int indLinhaJ = matrizCompras->row_ptr[j];
            int fimLinhaJ = matrizCompras->row_ptr[j + 1];

            int soma = 0;

            while (indLinhaI < fimLinhaI && indLinhaJ < fimLinhaJ) {
                int colunaI = matrizCompras->col_index[indLinhaI];
                int colunaJ = matrizCompras->col_index[indLinhaJ];

                if (colunaI < colunaJ) {
                    indLinhaI++;
                } else if (colunaI > colunaJ) {
                    indLinhaJ++;
                } else {
                    soma += matrizCompras->values[indLinhaI] * matrizCompras->values[indLinhaJ];
                    indLinhaI++;
                    indLinhaJ++;
                }
            }

            if (soma > 0) {
                matrizDestino->values.push_back(soma);
                matrizDestino->col_index.push_back(j);
                cont++;
            }
        }

        matrizDestino->row_ptr.push_back(cont);
    }

    return 1;
}