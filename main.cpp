#include <stdio.h>
#include <stdlib.h>
#include "ListaCompras.h"
#include "Similaridade.h"
#include "Recomendacao.h"

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Erro! Uso correto: %s <ARQUIVO_CSV> <ENTREGA> <ALGORITMO> <K>\n", argv[0]);
        printf("ENTREGA: 1 = ListaCompras | 2 = Similaridade | 3 = Recomendacao\n");
        printf("ALGORITMO: 1 = PADRAO | 2 = ADAPTADO");
        printf("Exemplo: %s dados/dados_venda_cluster_0.csv 2 1 5\n", argv[0]);
        return 1;
    }

    char *nome_arquivo = argv[1];
    int entrega = atoi(argv[2]);
    int algoritmo = atoi(argv[3]);
    int k = atoi(argv[4]);

    if (entrega > 4 || entrega < 1) {
        printf("ENTREGA invalida. Use 1, 2 ou 3.\n");
        return 1;
    }    

    ListaCompras lista;
    if (!criaListaCompras(nome_arquivo, &lista)) return 1;

    if (entrega == 1) {
        testadorExibeProdutos(&lista);
        return 0;
    }

    Similaridade sim;
    if (!criaMatrizSimilaridade(&lista, &sim, algoritmo)) return 1;

    if (entrega == 2) {
        testadorExibeSimilaridade(&sim, &lista, 1);
        testadorExibeSimilaridade(&sim, &lista, 4);
        liberaMatrizDouble(sim.S, sim.n);
        sim.S = NULL;
        return 0;
    }

    if (entrega == 3) {
        testadorExibeRecomendados(&lista, &sim, k);
        return 0;
    }

    if (entrega == 4) {
        exibeTempoExecucao(&sim);
        return 0;
    }
}