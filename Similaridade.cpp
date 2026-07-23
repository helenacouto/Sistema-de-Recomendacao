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
    sim->n = lista->vetorClientes.size();
    sim->m = lista->vetorProdutos.size();

    clock_t inicio = clock();
    if (!criaMatrizCompras(lista, sim)) return 0;

    if (alg == 0) {
        if (!criaTranspostaCompras(sim)) return 0;
        if (!criaMatrizIntersecao(sim)) return 0;
    }

    if (alg == 1) {
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

void exibeTempoExecucao(Similaridade *sim, const char *nome) {
    printf("Algoritmo %s: %.6f segundos\n", nome, sim->tempo);
}

void ordenaListaCompras (ListaCompras *lista, Similaridade *sim, vector<int> &vetorCompras) {
    for (int i = 0; i < sim->n; i++) {
        vector<int> produtosCliente;

        for (int produto : lista->listaCompras[i]) {
            produtosCliente.push_back(produto);
        }

        for (int posicao = 0; posicao < (int) produtosCliente.size(); posicao++) {
            int valorAtual = produtosCliente[posicao];
            int j = posicao - 1;

            while (j >= 0 && produtosCliente[j] > valorAtual) {
                produtosCliente[j + 1] = produtosCliente[j];
                j--;
            }

            produtosCliente[j + 1] = valorAtual;
        }

        for (int produto : produtosCliente) {
            vetorCompras.push_back(produto);
        }
    }
}

int criaMatrizComprasCSR(ListaCompras *lista, Similaridade *sim, MatrizCSR *matrizDestino, vector<int> &vetorCompras) {
    matrizDestino->n_linhas = sim->n;
    matrizDestino->n_colunas = sim->m;

    matrizDestino->row_ptr.push_back(0);

    int cont = 0;

    for (int i = 0; i < sim->n; i++) {
        int qtdProdutosCliente = lista->listaCompras[i].size();

        for (int k = 0; k < qtdProdutosCliente; k++) {
            int produto = vetorCompras[cont];

            matrizDestino->col_index.push_back(produto);
            matrizDestino->values.push_back(1);

            cont++;
        }

        matrizDestino->row_ptr.push_back(cont);
    }

    return 1;
}

int criaMatrizSimilaridadeCSR (ListaCompras *lista, Similaridade *sim) {
    sim->n = lista->vetorClientes.size();
    sim->m = lista->vetorProdutos.size();

    clock_t inicio = clock();

    vector<int> vetorCompras;
    ordenaListaCompras(lista, sim, vetorCompras);

    if (!criaMatrizComprasCSR(lista, sim, &sim->comprasCSR, vetorCompras)) return 0;
    if (!criaIntersecaoCSR(&sim->comprasCSR, &sim->intersecaoCSR, sim)) return 0;
    if (!criaVetorP(lista, sim)) return 0;

    clock_t fim = clock();
    sim->tempo = double(fim - inicio) / CLOCKS_PER_SEC;

    return 1;
}

double calculaSimilaridadeCSR (Similaridade *sim, int i, int j) {
    MatrizCSR *I = &sim->intersecaoCSR;

    int inicio = I->row_ptr[i];
    int fim = I->row_ptr[i + 1];

    for (int k = inicio; k < fim; k++) {
        if (I->col_index[k] == j) {
            return 1.0 - (double) I->values[k] / sim->P[i];
        }
    }

    return 1.0;
}


void testadorExibeSimilaridadeCSR (Similaridade *sim, 
    ListaCompras *lista, int indCliente) {
    int indSimilar = -1;
    double valorSimilaridade = 0;

    for (int j = 0; j < sim->n; j++) {
        if (j == indCliente) continue;

        double similaridade = calculaSimilaridadeCSR(sim, indCliente, j);

        if (indSimilar == -1 || similaridade < valorSimilaridade) {
            indSimilar = j;
            valorSimilaridade = similaridade;
        }
    }

    cout << "\nCliente " << lista->vetorClientes[indSimilar] 
        << " eh o mais similar ao cliente " << lista->vetorClientes[indCliente];
    printf("\nSimilaridade: %.4f\n", valorSimilaridade);
}

//verificar como o professor quer que façamos!
void comparaMemoria (Similaridade *sim) {
}

void testadorComparaSimilaridade (ListaCompras *lista) {
}