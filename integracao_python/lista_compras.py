from dataclasses import dataclass, field
from pathlib import Path

@dataclass
class ListaCompras:
    vetor_clientes: list = field(default_factory=list)   # list[str]
    mapa_clientes:  dict = field(default_factory=dict)   # dict[str, int]

    vetor_produtos: list = field(default_factory=list)   # list[str]
    mapa_produtos:  dict = field(default_factory=dict)   # dict[str, int]

    nome_produtos:  list = field(default_factory=list)   # list[str]
    lista_compras:  list = field(default_factory=list)   # list[list[int]]


def cria_lista_compras(caminho_arquivo: str) -> ListaCompras:
    lista = ListaCompras()
    caminho = Path(caminho_arquivo)

    # Passagem 1: popula vetores e mapas de clientes e produtos
    with caminho.open("r", encoding="utf-8") as f:
        for numero_linha, linha in enumerate(f, start=1):
            linha = linha.strip()
            if not linha:
                continue

            if numero_linha == 1: # pula primeira linha (cabeçalho do csv)
                continue

            partes = linha.split(",")
            if len(partes) < 4:
                continue

            cod_cliente  = partes[1]
            cod_produto  = partes[2]
            nome_produto = partes[3]

            if cod_cliente not in lista.mapa_clientes:
                lista.mapa_clientes[cod_cliente] = len(lista.vetor_clientes)
                lista.vetor_clientes.append(cod_cliente)
                lista.lista_compras.append([])   # lista vazia para esse cliente

            if cod_produto not in lista.mapa_produtos:
                lista.mapa_produtos[cod_produto] = len(lista.vetor_produtos)
                lista.vetor_produtos.append(cod_produto)
                lista.nome_produtos.append(nome_produto)

    # Passagem 2: preenche lista_compras
    with caminho.open("r", encoding="utf-8") as f:
        for numero_linha, linha in enumerate(f, start=1):
            linha = linha.strip()
            if not linha:
                continue
            if numero_linha == 1 and linha.startswith("DATA_COMPRA"):
                continue

            partes = linha.split(",")
            if len(partes) < 4:
                continue
            
            cod_cliente  = partes[1]
            cod_produto  = partes[2]
            nome_produto = partes[3]

            ind_cliente = lista.mapa_clientes[cod_cliente]
            ind_produto  = lista.mapa_produtos[cod_produto]

            # evita duplicatas (equivalente ao jaComprou do C++)
            if ind_produto not in lista.lista_compras[ind_cliente]:
                lista.lista_compras[ind_cliente].append(ind_produto)

    return lista


def mostrar_produtos_cliente(lista: ListaCompras, cod_cliente) -> None:
    if cod_cliente not in lista.mapa_clientes:
        print(f"Cliente {cod_cliente} não está registrado.")
        return

    ind_cliente = lista.mapa_clientes[cod_cliente]
    print(f"\nProdutos que o cliente {cod_cliente} comprou:")
    for ind_produto in lista.lista_compras[ind_cliente]:
        print(f"- {lista.nome_produtos[ind_produto]}")


def testador_exibe_produtos(lista: ListaCompras) -> None:
    for cod in ["99DIQV01", "99KQAA01", "99FT8Z01"]:
        mostrar_produtos_cliente(lista, cod)