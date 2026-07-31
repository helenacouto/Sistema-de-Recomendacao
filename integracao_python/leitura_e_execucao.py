from dataclasses import dataclass
from pathlib import Path

@dataclass
class Compra:
    cod_cliente: str
    co_produto: str
    nome_produto: str

def ler_clientes(caminho_arquivo: str):
    compras = []
    caminho = Path(caminho_arquivo)

    with caminho.open("r", encoding="utf-8") as f:
        for numero_linha, linha in enumerate(f, start=1):
            linha = linha.strip()
            if not linha or linha.startswith("#"):
                continue

            partes = linha.split(",")
            if len(partes) != 4:
                raise ValueError(
                    f"Linha {numero_linha}: esperado 4 campos, mas encontrei {len(partes)}."
                )

            compra = Compra(
                cod_cliente=str(partes[1]),
                cod_compra=str(partes[2]),
                nome_produto=str(partes[3]),
            )
            compras.append(compra)

    return compras


def converter_para_tuplas(compras):
    return [(c.cod_cliente, c.cod_produto, c.nome_produto) for c in compras]


def main():
    arquivo = "dados/dados_venda_cluster_0.csv"

    compras = ler_clientes(arquivo)
    converter_para_tuplas(compras)

    print("Compras lidas em Python:")
    for c in range(4):
        print(compras[c])