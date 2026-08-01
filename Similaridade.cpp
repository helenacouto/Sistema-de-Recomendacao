#include <stdlib.h>
#include <ctime>
#include "Similaridade.h"

using namespace std;

double **criaMatrizDouble(int linhas, int colunas) {
    double **matriz = (double **) malloc(linhas * sizeof(double *));

    if (matriz == NULL)
        return NULL;

    for (int i = 0; i < linhas; i++) {
        matriz[i] = (double *) malloc(colunas * sizeof(double));

        if (matriz[i] == NULL) {
            for (int j = 0; j < i; j++) free(matriz[j]);

            free(matriz);
            return NULL;
        }
    }

    return matriz;
}

void liberaMatrizDouble(double **matriz, int linhas) {
    if (matriz == NULL) return;

    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
        matriz[i] = NULL;
    }
    free(matriz);
}

int criaMatrizCompras(ListaCompras *lista, Similaridade *sim) {
    sim->n = lista->vetorClientes.size();
    sim->m = lista->vetorProdutos.size();

    sim->A = criaMatrizDouble(sim->n, sim->m);
    if (sim->A == NULL) return 0;

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
    return 1;
}

int criaTranspostaCompras(Similaridade *sim) {
    sim->At = criaMatrizDouble(sim->m, sim->n);
    if (sim->At == NULL) return 0;

    for (int i = 0; i < sim->n; i++) {
        for (int j = 0; j < sim->m; j++) {
            sim->At[j][i] = sim->A[i][j];
        }
    }
    return 1;
}

int criaMatrizIntersecao(Similaridade *sim) {
    sim->I = criaMatrizDouble(sim->n, sim->n);
    if (sim->I == NULL) return 0;

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

    return 1;
}

int criaIntersecaoEficiente(Similaridade *sim) {
    sim->I = criaMatrizDouble(sim->n, sim->n);
    if (sim->I == NULL) return 0;

    for (int i = 0; i < sim->n; i++) {
        for (int j = i; j < sim->n; j++) {
            sim->I[i][j] = 0;

            for (int k = 0; k < sim->m; k++) {
                sim->I[i][j] += sim->A[i][k] * sim->A[j][k];
            }

            sim->I[j][i] = sim->I[i][j];
        }
    }

    liberaMatrizDouble(sim->A, sim->n);
    sim->A = NULL;

    return 1;
}

int criaVetorP(ListaCompras *lista, Similaridade *sim) {
    sim->P = (int*) malloc(sim->n * sizeof(int));
    if (sim->P == NULL) return 0;

    for (int i = 0; i < sim->n; i++) {
        sim->P[i] = lista->listaCompras[i].size();
    }
    return 1;
}

int criaMatrizSimilaridade(ListaCompras *lista, Similaridade *sim, int alg) {
    clock_t inicio = clock();

    if (alg == 0) {
        if (!criaMatrizCompras(lista, sim)) return 0;
        if (!criaTranspostaCompras(sim)) return 0;
        if (!criaMatrizIntersecao(sim)) return 0;
    }

    if (alg == 1) {
        if (!criaMatrizCompras(lista, sim)) return 0;
        if (!criaIntersecaoEficiente(sim)) return 0;
    }

    if(!criaVetorP(lista, sim)) return 0;

    sim->S = criaMatrizDouble(sim->n, sim->n);
    if (sim->S == NULL) return 0;

    for (int i = 0; i < sim->n; i++) {
        for (int j = 0; j < sim->n; j++) {
            sim->S[i][j] = 1 - sim->I[i][j]/sim->P[i];
        }
    }

    liberaMatrizDouble(sim->I, sim->n);
    sim->I = NULL;
    free(sim->P);
    sim->P = NULL;

    clock_t fim = clock();
    sim->tempo = double(fim - inicio) / CLOCKS_PER_SEC;

    return 1;
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

void exibeTempoExecucao(double tempo, const char *nome) {
    printf("Algoritmo %s: %.6f segundos\n", nome, tempo);
}