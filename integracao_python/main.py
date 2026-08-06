import sys
import leitura_compras as lc
import sistema_recomendacao as sr


def main():
    if len(sys.argv) < 5:
        print(f"Erro! Uso correto: python {sys.argv[0]} <ARQUIVO_CSV> <ENTREGA> <ALGORITMO> <K>")
        print("ENTREGA:   1 = ListaCompras | 2 = Similaridade | 3 = Recomendacao | 4 = Multiplicao Eficiente | 5 = CSR")
        print("ALGORITMO: 0 = PADRAO | 1 = ADAPTADO | 2 = CSR")
        print(f"Exemplo: python {sys.argv[0]} dados/dados_venda_cluster_0.csv 3 1 10")
        sys.exit(1)

    arquivo = sys.argv[1]
    entrega = int(sys.argv[2])
    algoritmo = int(sys.argv[3])
    k = int(sys.argv[4])

    if entrega < 1 or entrega > 5:
        print("ENTREGA invalida. Use 1, 2, 3, 4 ou 5.")
        sys.exit(1)

    if algoritmo < 0 or algoritmo > 2:
        print("ALGORITMO invalido. Use 0, 1 ou 2.")
        sys.exit(1)

    lista = lc.ler_arquivo(f"../{arquivo}")
    n = len(lista.vetor_produtos)

    if entrega == 1:
        lc.testador_exibe_produtos(lista)

    elif entrega == 2:
        if algoritmo != 2:
            nova_sim = sr.cria_similaridade(lista.lista_compras, lista.vetor_clientes, n, algoritmo)
            sr.exibe_similaridade(lista.lista_compras, lista.vetor_clientes, n, nova_sim, 1, 4)
        else:
            sr.exibe_similaridade_csr(lista.lista_compras, lista.vetor_clientes, n, 1, 4)

    elif entrega == 3:
        if algoritmo != 2:
            nova_sim = sr.cria_similaridade(lista.lista_compras, lista.vetor_clientes, n, algoritmo)
            sr.exibe_recomendados(
                lista.lista_compras, lista.vetor_clientes,
                lista.mapa_clientes, lista.nome_produtos,
                n, nova_sim, k
            )
        else:
            sr.exibe_recomendados_csr(
                lista.lista_compras, lista.vetor_clientes,
                lista.mapa_clientes, lista.nome_produtos,
                n, k
            )

    elif entrega == 4:
        sr.compara_tempos(lista.lista_compras, lista.vetor_clientes, n)

    elif entrega == 5:
        sr.compara_tempos_memoria(lista.lista_compras, lista.vetor_clientes, n)


if __name__ == "__main__":
    main()