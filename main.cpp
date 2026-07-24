#include <stdio.h>
#include <stdlib.h>
#include "ListaCompras.h"
#include "Similaridade.h"
#include "Recomendacao.h"
#include "CSR.h"

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Erro! Uso correto: %s <ARQUIVO_CSV> <ENTREGA> <ALGORITMO> <K>\n", argv[0]);
        printf("ENTREGA: 1 = ListaCompras | 2 = Similaridade | 3 = Recomendacao\n");
        printf("ALGORITMO: 0 = PADRAO | 1 = ADAPTADO\n");
        printf("Exemplo: %s dados/dados_venda_cluster_1.csv 3 2 10\n", argv[0]);
        return 1;
    }

    char *nome_arquivo = argv[1];
    int entrega = atoi(argv[2]);
    int algoritmo = atoi(argv[3]);
    int k = atoi(argv[4]);

    if (entrega > 5 || entrega < 1) {
        printf("ENTREGA invalida. Use 1, 2, 3, 4 ou 5.\n");
        return 1;
    }    

    ListaCompras lista;
    if (!criaListaCompras(nome_arquivo, &lista)) return 1;

    if (entrega == 1) {
        testadorExibeProdutos(&lista);
        return 0;
    }

    Similaridade sim;
    SimilaridadeCSR simCSR;
    if (entrega == 2) {
        if (algoritmo != 2) {
            if (!criaMatrizSimilaridade(&lista, &sim, algoritmo)) return 1;

            testadorExibeSimilaridade(&sim, &lista, 1);
            testadorExibeSimilaridade(&sim, &lista, 4);

            liberaMatrizDouble(sim.S, sim.n);
            sim.S = NULL;
        } else {
            criaMatrizSimilaridadeCSR(&lista, &simCSR);

            testadorExibeSimilaridadeCSR(&lista, &simCSR, 1);
            testadorExibeSimilaridadeCSR(&lista, &simCSR, 4);
        }
        return 0;
    }

    if (entrega == 3) {
        if (algoritmo != 2) {
            if (!criaMatrizSimilaridade(&lista, &sim, algoritmo)) return 1;
            testadorExibeRecomendados(&lista, &sim, k);
        } else {
            criaMatrizSimilaridadeCSR(&lista, &simCSR);
            testadorExibeRecomendadosCSR(&lista, &simCSR, k);
        return 0;
        }
    }

    if (entrega == 4) {
        if (!criaMatrizSimilaridade(&lista, &sim, 0)) return 1;
        exibeTempoExecucao(&sim, "padrao");

        liberaMatrizDouble(sim.S, sim.n);
        sim.S = NULL;

        if (!criaMatrizSimilaridade(&lista, &sim, 1)) return 1;
        exibeTempoExecucao(&sim, "adaptado");

        liberaMatrizDouble(sim.S, sim.n);
        sim.S = NULL;

        return 0;
    }

    if (entrega == 5) {
        if (!criaMatrizSimilaridade(&lista, &sim, 1)) return 1;
        exibeTempoExecucao(&sim, "adaptado");

        liberaMatrizDouble(sim.S, sim.n);
        sim.S = NULL;

        criaMatrizSimilaridadeCSR(&lista, &simCSR);
        printf("Algoritmo CSR: %.6f segundos\n", simCSR.tempo);
        return 0;
    }
}