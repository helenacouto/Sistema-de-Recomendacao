#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "Similaridade.h"
#include "Recomendacao.h"
#include "CSR.h"

using namespace std;
namespace py = pybind11;

ListaCompras converte_lista(
    vector<vector<int>> lista_compras,
    vector<string> vetor_clientes,
    map<string, int> mapa_clientes,
    vector<string> nome_produtos,
    int n_produtos
) {
    ListaCompras lista;
    lista.vetorClientes = vetor_clientes;
    lista.mapaClientes = mapa_clientes;
    lista.nomeProdutos = nome_produtos;
    lista.vetorProdutos.resize(n_produtos); // nao precisa preencher o vetor pois só o usam pra saber o tamanho (sim->m)
    
    // o mesmo para o mapa dos produtos, ele só é usado na leitura em C, n faz sentido trazer pra cá

    lista.listaCompras.resize(lista_compras.size());
    for (int i = 0; i < (int)lista_compras.size(); i++) {
        for (int p : lista_compras[i]) {
            lista.listaCompras[i].push_back(p);
        }
    }

    return lista;
}

void testa_similaridade(
    vector<vector<int>> lista_compras,
    vector<string> vetor_clientes,
    int n_produtos, int algoritmo,
    int ind1, int ind2
) {
    ListaCompras lista = converte_lista(lista_compras, vetor_clientes, {}, {}, n_produtos);

    if (algoritmo != 2) {
        Similaridade sim;
        criaMatrizSimilaridade(&lista, &sim, algoritmo);
        testadorExibeSimilaridade(&sim, &lista, ind1);
        testadorExibeSimilaridade(&sim, &lista, ind2);
        liberaMatrizDouble(sim.S, sim.n);
        sim.S = NULL;
    } else {
        SimilaridadeCSR simCSR;
        criaMatrizSimilaridadeCSR(&lista, &simCSR);
        testadorExibeSimilaridadeCSR(&lista, &simCSR, ind1);
        testadorExibeSimilaridadeCSR(&lista, &simCSR, ind2);
    }
}

void testa_recomendados(
    vector<vector<int>> lista_compras,
    vector<string> vetor_clientes,
    map<string, int> mapa_clientes,
    vector<string> nome_produtos,
    int n_produtos, int algoritmo, int k
) {
    ListaCompras lista = converte_lista(lista_compras, vetor_clientes, mapa_clientes, nome_produtos, n_produtos);

    if (algoritmo != 2) {
        Similaridade sim;
        criaMatrizSimilaridade(&lista, &sim, algoritmo);
        testadorExibeRecomendados(&lista, &sim, k);
        liberaMatrizDouble(sim.S, sim.n);
        sim.S = NULL;
    } else {
        SimilaridadeCSR simCSR;
        criaMatrizSimilaridadeCSR(&lista, &simCSR);
        testadorExibeRecomendadosCSR(&lista, &simCSR, k);
    }
}

void compara_tempos(
    vector<vector<int>> lista_compras,
    vector<string> vetor_clientes,
    int n_produtos
) {
    ListaCompras lista = converte_lista(lista_compras, vetor_clientes, {}, {}, n_produtos);
    Similaridade sim;

    criaMatrizSimilaridade(&lista, &sim, 0);
    exibeTempoExecucao(sim.tempo, "padrao");
    liberaMatrizDouble(sim.S, sim.n);
    sim.S = NULL;

    criaMatrizSimilaridade(&lista, &sim, 1);
    exibeTempoExecucao(sim.tempo, "adaptado");
    liberaMatrizDouble(sim.S, sim.n);
    sim.S = NULL;
}

void compara_tempos_memoria(
    vector<vector<int>> lista_compras,
    vector<string> vetor_clientes,
    int n_produtos
) {
    ListaCompras lista = converte_lista(lista_compras, vetor_clientes, {}, {}, n_produtos);
    Similaridade sim;

    criaMatrizSimilaridade(&lista, &sim, 0);
    exibeTempoExecucao(sim.tempo, "padrao");
    liberaMatrizDouble(sim.S, sim.n);
    sim.S = NULL;

    criaMatrizSimilaridade(&lista, &sim, 1);
    exibeTempoExecucao(sim.tempo, "adaptado");
    liberaMatrizDouble(sim.S, sim.n);
    sim.S = NULL;

    SimilaridadeCSR simCSR;
    criaMatrizSimilaridadeCSR(&lista, &simCSR);
    exibeTempoExecucao(simCSR.tempo, "CSR");
    comparaMemoria(&simCSR);
}


PYBIND11_MODULE(sistema_recomendacao, m) {
    m.doc() = "Sistema de Recomendacao - integracao Python-C++";

    m.def("testa_similaridade", &testa_similaridade);
    m.def("testa_recomendados", &testa_recomendados);
    m.def("compara_tempos", &compara_tempos);
    m.def("compara_tempos_memoria", &compara_tempos_memoria);
}