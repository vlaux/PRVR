#ifndef VND
#define VND

#include "solucao.h"

Solucao aplica_vnd(const Solucao &s, int n_clientes, int capacidade, int **mapa_rotulos, int k_max);
Solucao encontra_melhor_solucao_vizinhanca(const Solucao &s, int n_clientes, int capacidade, int **mapa_rotulos, int k);

#endif