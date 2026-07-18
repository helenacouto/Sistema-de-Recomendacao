#include <stdio.h>
#include "Recomendacao.h"
#include <algorithm>

using namespace std;

void criaClientesSimilares(Similaridade *sim, Recomendacao *rec, int c) {
    for (int j = 0; j < sim->n; j++) {
        if (j == c) continue;

        double similaridade = calculaSimilaridade(sim, c, j);
        if (similaridade >= 1) continue;

        rec->L.push_back(j);
    }
}

void criaVetorRanqueamento(Similaridade *sim, Recomendacao *rec) {
    for (int p = 0; p < sim->m; p++) {
        Rank item;
        item.indProduto = p;
        item.valor = 1;

        rec->R.push_back(item);
    }
}

static int jaComprou(ListaCompras *lista, int c, int p) {
    for (int compra : lista->listaCompras[c]) {
        if (compra == p) return true;
    }
    return false;
}

void calculaRanqueamento(Similaridade *sim, ListaCompras *lista, Recomendacao *rec, int c) {
    for (int s : rec->L) {
        double similaridade = calculaSimilaridade(sim, c, s);

        for (int p : lista->listaCompras[s]) {
            if (jaComprou(lista, c, p)) continue;
                
            rec->R[p].valor *= similaridade;
        }
    }
}

