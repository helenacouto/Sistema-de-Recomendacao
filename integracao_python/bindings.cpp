#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "Similaridade.h"
#include "Recomendacao.h"
#include "CSR.h"

namespace py = pybind11;

PYBIND11_MODULE(sisema_recomendacao, m) {
    m.doc() = "Pacote de Recomendação";

    m.def("cria_matriz_similaridade", &criaMatrizSimilaridade, "Funcao que cria a matriz de similaridade");

    m.def("cria_matriz_similaridade_CSR", &criaMatrizSimilaridadeCSR, "Funcao que cria a matriz de similaridade em CSR");

    m.def("exibe_similaridade", &testadorExibeSimilaridade, "Funcao que exibe a similaridade");

    m.def("exibe_similaridade_CSR", &testadorExibeSimilaridadeCSR, "Funcao que exibe a similaridade em CSR");

    m.def("exibe_recomendados", &testadorExibeRecomendados, "Funcao que exibe recomendados");

    m.def("exibe_recomendados_CSR", &testadorExibeRecomendadosCSR, "Funcao que exibe recomendados em CSR");

    m.def("exibe_tempo_execucao", &exibeTempoExecucao, "Funcao que exibe o tempo de execucao do algoritmo escolhido");
}