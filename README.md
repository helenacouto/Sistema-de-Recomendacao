# Sistema de Recomendação

Projeto da disciplina de Programação Estruturada, do curso de Ciência de Dados e Inteligência Artificial da Universidade Federal da Paraíba.

> **Nota sobre o código:** o GitHub identifica este repositório como C++ (por causa da extensão `.cpp`), mas o código é escrito majoritariamente em estilo C — `stdio.h`, `printf`/`fscanf`, structs simples, sem classes ou orientação a objetos. C++ aparece só nos containers da STL (`vector`, `map`, `list`, `string`), que são exigidos pelo enunciado da disciplina.

## Objetivo

Implementar um sistema de recomendação de produtos para clientes de lojas de varejo, usando o Método da Vizinhança (filtragem colaborativa baseada em usuário) com similaridade de Jaccard.

## Como compilar

```bash
g++ ListaCompras.cpp Similaridade.cpp Recomendacao.cpp CSR.cpp main.cpp -o sistema
```

## Como usar

```bash
./sistema <arquivo.csv> <entrega> <algoritmo> <k>
```

| Argumento | Descrição |
|---|---|
| `arquivo.csv` | base de compras a ser lida (ex: `dados/dados_venda_cluster_1.csv`) |
| `entrega` | qual testador rodar: `1` = ListaCompras, `2` = Similaridade, `3` = Recomendação, <br> `4` = Multiplicação Eficiente |
| `algoritmo` | qual algoritmo usar: `0` = Padrão, `1` = Adaptado | 
| `k` | quantidade de produtos recomendados (usado só na entrega `3`) |

Exemplos:
```bash
./sistema dados/dados_venda_cluster_1.csv 1 0 0   # testador da Atividade 1
./sistema dados/dados_venda_cluster_1.csv 2 0 0   # testador da Atividade 2
./sistema dados/dados_venda_cluster_1.csv 3 1 10  # testador da Atividade 3
./sistema dados/dados_venda_cluster_1.csv 4 0 0   # testador da Atividade 4
./sistema dados/dados_venda_cluster_1.csv 5 0 0   # testador da Atividade 5
```

##  Integrantes
- Helena Couto dos Santos
- Mariana Esthefany Xavier dos Santos