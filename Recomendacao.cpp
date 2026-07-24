#include <stdio.h>
#include <algorithm>
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

void criaVetorRanqueamento(int qntdProdutos, Recomendacao *rec) {
    for (int p = 0; p < qntdProdutos; p++) {
        Rank item;
        item.indProduto = p;
        item.valor = 1;

        rec->R.push_back(item);
    }
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
    if (lista->mapaClientes.find(codigo) == lista->mapaClientes.end()) {
        printf("Cliente %s nao esta registrado.\n", codigo);
        return;
    }

    int c = lista->mapaClientes[codigo];

    Recomendacao rec;
    criaClientesSimilares(sim, &rec, c);
    criaVetorRanqueamento(sim->m, &rec);
    calculaRanqueamento(sim, lista, &rec, c);
    ordenaRanqueamento(&rec);

    int produto;
    printf("\nOs %d produtos mais recomendados para o cliente %s:\n", k, codigo);
    for (int i = 0; i < k; i++) {
        produto = rec.R[i].indProduto;
        cout << "- " << lista->nomeProdutos[produto] << endl;
    }
}

void testadorExibeRecomendados(ListaCompras *lista, Similaridade *sim, int k) {
    char cliente1[] = "99DIQV01";
    char cliente2[] = "99KQAA01";
    char cliente3[] = "99FT8Z01";

    recomendaProdutos(lista, sim, cliente1, k);
    recomendaProdutos(lista, sim, cliente2, k);
    recomendaProdutos(lista, sim, cliente3, k);

    liberaMatrizDouble(sim->S, sim->n);
    sim->S = NULL;
}

void calculaRanqueamentoCSR(SimilaridadeCSR *simCSR, ListaCompras *lista, Recomendacao *rec, int c) {
    for (int k = simCSR->similaridadeCSR.row_ptr[c]; k < simCSR->similaridadeCSR.row_ptr[c + 1]; k++) {
        int s = simCSR->similaridadeCSR.col_index[k];
        double similaridade = simCSR->similaridadeCSR.values[k];

        for (int p : lista->listaCompras[s]) {
            if (jaComprou(lista, c, p)) continue;
            rec->R[p].valor *= similaridade;
        }
    }
}

void recomendaProdutosCSR(ListaCompras *lista, SimilaridadeCSR *simCSR, char *codigo, int k) {
    if (lista->mapaClientes.find(codigo) == lista->mapaClientes.end()) {
        printf("Cliente %s nao esta registrado.\n", codigo);
        return;
    }

    int c = lista->mapaClientes[codigo];

    Recomendacao rec;
    criaVetorRanqueamento(simCSR->m, &rec);
    calculaRanqueamentoCSR(simCSR, lista, &rec, c);
    ordenaRanqueamento(&rec);

    int produto;
    printf("\nOs %d produtos mais recomendados para o cliente %s:\n", k, codigo);
    for (int i = 0; i < k; i++) {
        produto = rec.R[i].indProduto;
        cout << "- " << lista->nomeProdutos[produto] << endl;
    }
}

void testadorExibeRecomendadosCSR(ListaCompras *lista, SimilaridadeCSR *simCSR, int k) {
    char cliente1[] = "99DIQV01";
    char cliente2[] = "99KQAA01";
    char cliente3[] = "99FT8Z01";

    recomendaProdutosCSR(lista, simCSR, cliente1, k);
    recomendaProdutosCSR(lista, simCSR, cliente2, k);
    recomendaProdutosCSR(lista, simCSR, cliente3, k);
}