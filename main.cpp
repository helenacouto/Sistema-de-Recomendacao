#include <stdio.h>
#include "ListaCompras.h"
#include "Similaridade.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Erro! Uso correto: %s <ARQUIVO_PARA_LEITURA>\n", argv[0]);
        printf("Exemplo: %s dados/dados_venda_cluster_0.csv\n", argv[0]);
        return 1;
    }

    char *nome_arquivo = argv[1];
    ListaCompras lista;

    if (!criaListaCompras(nome_arquivo, &lista)) return 1;
    testadorExibeProdutos(&lista);

    Similaridade sim;
    if (!criaMatrizCompras(&lista, &sim)) return 1;
    if (!criaTranspostaCompras(&sim)) return 1;
    if (!criaMatrizIntersecao(&sim)) return 1;
    if (!criaVetorP(&lista, &sim)) return 1;
    if (!criaMatrizSimilaridade(&sim)) return 1;

    testadorExibeSimilaridade(&sim, &lista, 0);
    testadorExibeSimilaridade(&sim, &lista, 1);

    return 0;
}
