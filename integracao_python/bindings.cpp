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
    lista.vetorProdutos.resize(n_produtos);

    lista.listaCompras.resize(lista_compras.size());
    for (int i = 0; i < (int)lista_compras.size(); i++) {
        for (int p : lista_compras[i]) {
            lista.listaCompras[i].push_back(p);
        }
    }

    return lista;
}

vector<vector<double>> converte_similaridade_vector(Similaridade *sim) {
    vector<vector<double>> nova_sim;

    for (int i = 0; i < sim->n; i++) {
        vector<double> valores;
        for (int j = 0; j < sim->n; j++) {
            valores.push_back(sim->S[i][j]);
        }
        nova_sim.push_back(valores);
    }

    return nova_sim;
}

vector<vector<double>> cria_similaridade(
    vector<vector<int>> lista_compras,
    vector<string> vetor_clientes,
    int n_produtos, int algoritmo
) {
    ListaCompras lista = converte_lista(lista_compras, vetor_clientes, {}, {}, n_produtos);
    Similaridade sim;

    criaMatrizSimilaridade(&lista, &sim, algoritmo);
    vector<vector<double>> nova_sim = converte_similaridade_vector(&sim);

    liberaMatrizDouble(sim.S, sim.n);
    sim.S = NULL;

    return nova_sim;
}

Similaridade converte_vector_similaridade(const vector<vector<double>> &nova_sim) {
    Similaridade sim;
    sim.n = nova_sim.size();

    sim.S = criaMatrizDouble(sim.n, sim.n);
    for (int i = 0; i < sim.n; i++) {
        for (int j = 0; j < sim.n; j++) {
            sim.S[i][j] = nova_sim[i][j];
        }
    }

    return sim;
}

void exibe_similaridade(
    vector<vector<int>> lista_compras,
    vector<string> vetor_clientes,
    int n_produtos,
    const vector<vector<double>> &nova_sim,
    int ind1, int ind2
) {
    ListaCompras lista = converte_lista(lista_compras, vetor_clientes, {}, {}, n_produtos);
    Similaridade sim = converte_vector_similaridade(nova_sim);

    testadorExibeSimilaridade(&sim, &lista, ind1);
    testadorExibeSimilaridade(&sim, &lista, ind2);

    liberaMatrizDouble(sim.S, sim.n);
    sim.S = NULL;
}

void exibe_similaridade_csr(
    vector<vector<int>> lista_compras,
    vector<string> vetor_clientes,
    int n_produtos, int ind1, int ind2
) {
    ListaCompras lista = converte_lista(lista_compras, vetor_clientes, {}, {}, n_produtos);
    SimilaridadeCSR simCSR;

    criaMatrizSimilaridadeCSR(&lista, &simCSR);

    testadorExibeSimilaridadeCSR(&lista, &simCSR, ind1);
    testadorExibeSimilaridadeCSR(&lista, &simCSR, ind2);
}

void exibe_recomendados(
    vector<vector<int>> lista_compras,
    vector<string> vetor_clientes,
    map<string, int> mapa_clientes,
    vector<string> nome_produtos,
    int n_produtos,
    const vector<vector<double>> &nova_sim,
    int k
) {
    ListaCompras lista = converte_lista(lista_compras, vetor_clientes, mapa_clientes, nome_produtos, n_produtos);
    Similaridade sim = converte_vector_similaridade(nova_sim);

    testadorExibeRecomendados(&lista, &sim, k);

    liberaMatrizDouble(sim.S, sim.n);
    sim.S = NULL;
}

void exibe_recomendados_csr(
    vector<vector<int>> lista_compras,
    vector<string> vetor_clientes,
    map<string, int> mapa_clientes,
    vector<string> nome_produtos,
    int n_produtos, int k
) {
    ListaCompras lista = converte_lista(lista_compras, vetor_clientes, mapa_clientes, nome_produtos, n_produtos);
    SimilaridadeCSR simCSR;

    criaMatrizSimilaridadeCSR(&lista, &simCSR);
    testadorExibeRecomendadosCSR(&lista, &simCSR, k);
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

    m.def("cria_similaridade", &cria_similaridade,
        "Constroi a matriz de similaridade (algoritmo 0 ou 1) e devolve como lista de listas.");

    m.def("exibe_similaridade", &exibe_similaridade,
        "Reconstroi a matriz de similaridade a partir da lista de listas e exibe a comparacao entre dois clientes.");

    m.def("exibe_similaridade_csr", &exibe_similaridade_csr,
        "Constroi a matriz de similaridade em CSR e exibe a comparacao entre dois clientes.");

    m.def("exibe_recomendados", &exibe_recomendados,
        "Reconstroi a matriz de similaridade a partir da lista de listas e exibe as recomendacoes.");

    m.def("exibe_recomendados_csr", &exibe_recomendados_csr,
        "Constroi a matriz de similaridade em CSR e exibe as recomendacoes.");

    m.def("compara_tempos", &compara_tempos,
        "Compara tempo de execucao do algoritmo padrao com o algoritmo adaptado.");

    m.def("compara_tempos_memoria", &compara_tempos_memoria,
        "Compara tempo de execucao e uso da memoria entre os algoritmos padrao, adaptado e CSR.");
}