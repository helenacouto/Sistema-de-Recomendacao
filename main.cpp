#include <stdio.h>
#include <stdlib.h>
#include "ListaCompras.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Erro! Uso correto: %s <ARQUIVO_PARA_LEITURA>\n", argv[0]);
        printf("Exemplo: %s dados_venda_cluster_0.csv\n", argv[0]);
        return 1;
    }

    char *nome_arquivo = argv[1];
    ListaCompras lista;

    if (not criaListaCompras(nome_arquivo, &lista)) return 1;

    return 0;
}
