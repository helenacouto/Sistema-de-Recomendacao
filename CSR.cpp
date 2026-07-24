#include <stdio.h>
#include <ctime>
#include "CSR.h"

using namespace std;

void criaMatrizComprasCSR (ListaCompras *lista, MatrizCSR *A, SimilaridadeCSR *simCSR) {
    simCSR->n = lista->vetorClientes.size();
    simCSR->m = lista->vetorProdutos.size();

    A->values.clear();
    A->col_index.clear();
    
    A->row_ptr.clear();
    for (int i =0; i <= simCSR->n; i++) {
        A->row_ptr.push_back(0);
    }

    simCSR->P.clear();
    int cont = 0;

    for (int i = 0; i < simCSR->n; i++) {
        lista->listaCompras[i].sort();

        for (int produto : lista->listaCompras[i]) {
            A->col_index.push_back(produto);
            A->values.push_back(1);
            cont++;
        }

        A->row_ptr[i + 1] = cont;
        simCSR->P.push_back(A->row_ptr[i + 1] - A->row_ptr[i]); // Cria o vetor P no caminho !
    }

    return;
}

void criaIntersecaoCSR(MatrizCSR *A, MatrizCSR *C, SimilaridadeCSR *simCSR) {
    C->values.clear();
    C->col_index.clear();

    C->row_ptr.clear();
    for (int i = 0; i <= simCSR->n; i++) {
        C->row_ptr.push_back(0);
    }

    int cont = 0;

    for (int i =0; i < simCSR->n; i++) {
        if (A->row_ptr[i] == A->row_ptr[i+1]) {
            C->row_ptr[i + 1] = cont;
            continue;
        }

        for ( int j = 0; j < simCSR->n; j++) {
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

void criaMatrizSimilaridadeCSR (ListaCompras *lista, SimilaridadeCSR *simCSR) {
    clock_t inicio = clock();

    MatrizCSR A;
    criaMatrizComprasCSR(lista, &A, simCSR);
    criaIntersecaoCSR(&A, &simCSR->intersecaoCSR, simCSR);
    
    MatrizCSR *C = &simCSR->intersecaoCSR;
    MatrizCSR *S = &simCSR->similaridadeCSR;

    S->row_ptr.push_back(0);
    int cont = 0;

    for (int i = 0; i < simCSR->n; i++) {
        for (int k = C->row_ptr[i]; k < C->row_ptr[i + 1]; k++) {
            int j = C->col_index[k];

            S->values.push_back(1.0 - C->values[k] / simCSR->P[i]);
            S->col_index.push_back(j);
            cont++;
        }
        S->row_ptr.push_back(cont);
    } // Cria a matriz de similaridade necessaria pra recomendação de produtos!

    clock_t fim = clock();
    simCSR->tempo = double(fim - inicio) / CLOCKS_PER_SEC;
}

double calculaSimilaridadeCSR (SimilaridadeCSR *simCSR, int i, int j) {
    MatrizCSR *C = &simCSR->intersecaoCSR;

    int inicio = C->row_ptr[i];
    int fim = C->row_ptr[i + 1];

    for (int k = inicio; k < fim; k++) {
        if (C->col_index[k] == j) {
            return 1.0 - (double) C->values[k] / simCSR->P[i];
        }
    }

    return 1.0;
}

void testadorExibeSimilaridadeCSR (ListaCompras *lista, SimilaridadeCSR *simCSR, int indCliente) {
    int indSimilar = -1;
    double valorSimilaridade = 0;

    for (int j = 0; j < simCSR->n; j++) {
        if (j == indCliente) continue;

        double similaridade = calculaSimilaridadeCSR(simCSR, indCliente, j);

        if (indSimilar == -1 || similaridade < valorSimilaridade) {
            indSimilar = j;
            valorSimilaridade = similaridade;
        }
    }

    cout << "\nCliente " << lista->vetorClientes[indSimilar] 
        << " eh o mais similar ao cliente " << lista->vetorClientes[indCliente];
    printf("\nSimilaridade: %.4f\n", valorSimilaridade);
}

// void comparaMemoria(SimilaridadeCSR *simCSR)