#include <stdlib.h>
#include "Similaridade.h"

using namespace std;

int criaMatrizCompras(ListaCompras *lista, Similaridade *sim) {
    sim->n = lista->vetorClientes.size();
    sim->m = lista->vetorProdutos.size();

    sim->A = (double**) malloc(sim->n * sizeof(double*));
    if (sim->A == NULL) return false;

    for (int i = 0; i < sim->n; i++) {
        sim->A[i] = (double*) malloc(sim->m * sizeof(double));
        if (sim->A[i] == NULL) return false;
    }

    for (int i = 0; i < sim->n; i++) {
        for (int j = 0; j < sim->m; j++) {
            sim->A[i][j] = 0;
        } 
    }

    for (int i = 0; i < sim->n; i++) {
        for (int produto : lista->listaCompras[i]) {
            sim->A[i][produto] = 1;
        }
    }
    return true;
}

int criaTranspostaCompras(Similaridade *sim) {
    sim->At = (double**) malloc(sim->m * sizeof(double*));
    if (sim->At == NULL) return false;

    for (int i = 0; i < sim->m; i++) {
        sim->At[i] = (double*) malloc(sim->n * sizeof(double));
        if (sim->At[i] == NULL) return false;
    }

    for (int i = 0; i < sim->n; i++) {
        for (int j = 0; j < sim->m; j++) {
            sim->At[j][i] = sim->A[i][j];

        }
    }
    return true;
}

void liberaMatrizDouble(double **matriz, int linhas) {
    if (matriz == NULL) return;

    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
        matriz[i] = NULL;
    }
    free(matriz);
}

int criaMatrizIntersecao(Similaridade *sim) {
    sim->I = (double**) malloc(sim->n * sizeof(double*));
    if (sim->I == NULL) return false;

    for (int i = 0; i < sim->n; i++) {
        sim->I[i] = (double*) malloc(sim->n * sizeof(double));
        if (sim->I[i] == NULL) return false;
    }

    for (int i = 0; i < sim->n; i++) {
        for (int j = 0; j < sim->n; j++) {
            sim->I[i][j] = 0;
            for (int k = 0; k < sim->m; k++) {
                sim->I[i][j] += sim->A[i][k] * sim->At[k][j];
            }
        }
    }

    liberaMatrizDouble(sim->A, sim->n);
    sim->A = NULL;
    liberaMatrizDouble(sim->At, sim->m);
    sim->At = NULL;

    return true;
}

int criaVetorP(ListaCompras *lista, Similaridade *sim) {
    sim->P = (int*) malloc(sim->n * sizeof(int));
    if (sim->P == NULL) return false;

    for (int i = 0; i < sim->n; i++) {
        sim->P[i] = lista->listaCompras[i].size();
    }
    return true;
}

int criaMatrizSimilaridade(Similaridade *sim) {
    sim->S = (double**) malloc(sim->n * sizeof(double*));
    if (sim->S == NULL) return false;

    for (int i = 0; i < sim->n; i++) {
        sim->S[i] = (double*) malloc(sim->n * sizeof(double));
        if (sim->S[i] == NULL) return false;
    }

    for (int i = 0; i < sim->n; i++) {
        for (int j = 0; j < sim->n; j++) {
            sim->S[i][j] = 1 - sim->I[i][j]/sim->P[i];
        }
    }

    liberaMatrizDouble(sim->I, sim->n);
    sim->I = NULL;
    free(sim->P);
    sim->P = NULL;

    return true;
}

double calculaSimilaridade(Similaridade *sim, int i, int j) {
    return sim->S[i][j];
}

void testadorExibeSimilaridade (Similaridade *sim, ListaCompras *lista, int indCliente) {
    int indSimilar = -1;
    double valorSimilaridade = 0;

    for (int j = 0; j < sim->n; j++) {
        if (j == indCliente) continue;

        double similaridade = calculaSimilaridade(sim, indCliente, j);

        if (indSimilar == -1 || similaridade < valorSimilaridade) {
            indSimilar = j;
            valorSimilaridade = similaridade;
        }
    }

    cout << "\nCliente " << lista->vetorClientes[indSimilar] 
        << " eh o mais similar ao cliente " << lista->vetorClientes[indCliente];
    printf("\nSimilaridade: %.4f\n", valorSimilaridade);
}