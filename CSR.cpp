#include <stdio.h>
#include "Similaridade.h"

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