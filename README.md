# Sistema de Recomendação

Projeto da disciplina de Programação Estruturada, do curso de Ciência de Dados e Inteligência Artificial da Universidade Federal da Paraíba.

> **Nota sobre o código:** o GitHub identifica este repositório como C++ (por causa da extensão `.cpp`), mas o código é escrito majoritariamente em estilo C — `stdio.h`, `printf`/`fscanf`, structs simples, sem classes ou orientação a objetos. C++ aparece só nos containers da STL (`vector`, `map`, `list`, `string`), que são exigidos pelo enunciado da disciplina.

## Objetivo

Implementar um sistema de recomendação de produtos para clientes de lojas de varejo, usando o Método da Vizinhança (filtragem colaborativa baseada em usuário) com similaridade de Jaccard.

## Estrutura do projeto

O projeto é dividido em seis atividades incrementais, cada uma adicionando um módulo ao sistema:

| Atividade | Módulo | Descrição |
|---|---|---|
| 1 | `ListaCompras` | Leitura do CSV e montagem das estruturas de dados (vetores, mapas, lista de compras por cliente) |
| 2 | `Similaridade` | Construção da matriz de similaridade de Jaccard via multiplicação de matrizes densa |
| 3 | `Recomendacao` | Ranqueamento e seleção dos *k* produtos mais recomendados para cada cliente |
| 4 | `Multiplicação eficiente` | Otimização da multiplicação explorando a simetria de A × Aᵀ, reduzindo operações pela metade |
| 5 | `CSR` | Representação esparsa (Compressed Sparse Row) da matriz de compras e de similaridade, viabilizando bases grandes |
| 6 | `Integração Python` | Reimplementação da leitura de dados em Python e integração com os módulos C++ via pybind11 |

## Como compilar (C++)

```bash
g++ ListaCompras.cpp Similaridade.cpp Recomendacao.cpp CSR.cpp main.cpp -o sistema
```

## Como usar (C++)

```bash
./sistema <arquivo.csv> <entrega> <algoritmo> <k>
```

| Argumento | Descrição |
|---|---|
| `arquivo.csv` | base de compras a ser lida (ex: `dados/dados_venda_cluster_1.csv`) |
| `entrega` | qual testador rodar: `1` = ListaCompras, `2` = Similaridade, `3` = Recomendação, <br> `4` = Multiplicação Eficiente, `5` = Matrizes Esparsas (CSR) |
| `algoritmo` | qual algoritmo usar: `0` = Padrão, `1` = Adaptado, `2` = CSR |
| `k` | quantidade de produtos recomendados (usado só na entrega `3`) |

Exemplos:
```bash
./sistema dados/dados_venda_cluster_1.csv 1 0 0   # testador da Atividade 1
./sistema dados/dados_venda_cluster_1.csv 2 0 0   # testador da Atividade 2
./sistema dados/dados_venda_cluster_1.csv 3 1 10  # testador da Atividade 3
./sistema dados/dados_venda_cluster_1.csv 4 0 0   # testador da Atividade 4
./sistema dados/dados_venda_cluster_1.csv 5 0 0   # testador da Atividade 5
```

## Integração Python (Atividade 6)

A Atividade 6 reimplementa a leitura de dados em Python e integra os módulos C++ de similaridade e recomendação via [pybind11](https://pybind11.readthedocs.io/). O módulo `ListaCompras` não entra na compilação C++, sua lógica é refeita em Python puro, e os dados resultantes são passados para os módulos C++ através de wrappers.

### Dependências

```bash
pip install pybind11 setuptools wheel
```

Também é necessário um compilador C++17. No Windows, instalar o **Microsoft C++ Build Tools** com a opção *Desktop development with C++*.

### Como compilar o módulo Python

Dentro da pasta `integracao_python/`:

```bash
cd integracao_python
python setup.py build_ext --inplace
```

Isso gera o arquivo `sistema_recomendacao*.pyd` (Windows) ou `sistema_recomendacao*.so` (Linux/macOS), que é importado automaticamente pelo `main.py`.

### Como usar (Python)

```bash
python main.py <arquivo.csv> <entrega> <algoritmo> <k>
```

Os argumentos são os mesmos da versão C++, exceto que a entrega `1` roda o testador em Python puro (sem chamar o módulo compilado) e as entregas `2` a `5` chamam os módulos C++ via pybind11.

Exemplos:
```bash
python main.py dados/dados_venda_cluster_1.csv 1 0 0   # testador da Atividade 1 (Python puro)
python main.py dados/dados_venda_cluster_1.csv 2 1 0   # testador da Atividade 2
python main.py dados/dados_venda_cluster_1.csv 3 2 10  # testador da Atividade 3
python main.py dados/dados_venda_cluster_1.csv 4 0 0   # testador da Atividade 4
python main.py dados/dados_venda_cluster_1.csv 5 0 0   # testador da Atividade 5
```

## Integrantes
- Helena Couto dos Santos
- Mariana Esthefany Xavier dos Santos