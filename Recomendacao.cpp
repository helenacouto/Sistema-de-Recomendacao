#include <stdio.h>
#include "Recomendacao.h"

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
        Produto item;
        item.indProduto = p;
        item.valor = 1;

        rec->R.push_back(item);
    }
}

