from dataclasses import dataclass
from pathlib import Path

@dataclass
class ListaCompras:
    vetor_clientes = []
    mapa_clientes = {}

    vetor_produtos = []
    mapa_produtos = {}

    nome_produtos = []
    lista_compras = []


def ler_arquivo(caminho_arquivo: str):
    lista = ListaCompras()
    caminho = Path(caminho_arquivo)

    with caminho.open("r", encoding="utf-8") as f:
        for numero_linha, linha in enumerate(f, start=1):
            linha = linha.strip()
            if not linha or numero_linha == 1:
                continue

            partes = linha.split(",")
            if len(partes) < 4:
                continue

            cod_cliente = partes[1]
            cod_produto = partes[2]
            nome_produto = partes[3]

            if cod_cliente not in lista.mapa_clientes:
                lista.mapa_clientes[cod_cliente] = len(lista.vetor_clientes)
                lista.vetor_clientes.append(cod_cliente)
                lista.lista_compras.append([])

            if cod_produto not in lista.mapa_produtos:
                lista.mapa_produtos[cod_produto] = len(lista.vetor_produtos)
                lista.vetor_produtos.append(cod_produto)
                lista.nome_produtos.append(nome_produto)

    with caminho.open("r", encoding="utf-8") as f:
        for numero_linha, linha in enumerate(f, start=1):
            linha = linha.strip()
            if not linha or numero_linha == 1:
                continue

            partes = linha.split(",")
            if len(partes) < 4:
                continue
            
            cod_cliente = partes[1]
            cod_produto = partes[2]
            nome_produto = partes[3]

            ind_cliente = lista.mapa_clientes[cod_cliente]
            ind_produto = lista.mapa_produtos[cod_produto]

            if ind_produto not in lista.lista_compras[ind_cliente]:
                lista.lista_compras[ind_cliente].append(ind_produto)

    return lista


def mostrar_produtos_cliente(lista: ListaCompras, cod_cliente):
    if cod_cliente not in lista.mapa_clientes:
        print(f"Cliente {cod_cliente} não está registrado.")
        return

    ind_cliente = lista.mapa_clientes[cod_cliente]
    print(f"\nProdutos que o cliente {cod_cliente} comprou:")
    for ind_produto in lista.lista_compras[ind_cliente]:
        print(f"- {lista.nome_produtos[ind_produto]}")


def testador_exibe_produtos(lista: ListaCompras):
    for cod in ["99DIQV01", "99KQAA01", "99FT8Z01"]:
        mostrar_produtos_cliente(lista, cod)