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

bool compararRanqueamento(const Rank &a, const Rank &b) {
    return a.valor < b.valor;
}

void ordenaRanqueamento(Recomendacao *rec) {
    sort(rec->R.begin(), rec->R.end(), compararRanqueamento);
}

void recomendaProdutos(ListaCompras *lista, Similaridade *sim, char *codigo, int k) {
    int c;

    if (lista->mapaClientes.find(codigo) == lista->mapaClientes.end()) {
        printf("Cliente %s nao esta registrado.\n", codigo);
        return;
    }
    c = lista->mapaClientes[codigo];

    Recomendacao rec;

    criaClientesSimilares(sim, &rec, c);
    criaVetorRanqueamento(sim, &rec);
    calculaRanqueamento(sim, lista, &rec, c);
    ordenaRanqueamento(&rec);

    cout << "Os "<< k << " produtos mais recomendados para o cliente " 
        << lista->vetorClientes[c] << ":" << endl;
    for (int i = 0; i < k; i++) {
        int produto = rec.R[i].indProduto;
        cout << "- " << lista->nomeProdutos[produto] << endl;
    }

    printf("\n");
}

void testadorExibeRecomendados(ListaCompras *lista, Similaridade *sim, int k) {
    char cliente1[] = "99DL9N01";
    char cliente2[] = "77602301";
    char cliente3[] = "22047601";

    recomendaProdutos(lista, sim, cliente1, k);
    recomendaProdutos(lista, sim, cliente2, k);
    recomendaProdutos(lista, sim, cliente3, k);

    liberaMatrizDouble(sim->S, sim->n);
    sim->S = NULL;
}
