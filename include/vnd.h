#ifndef VND
#define VND

#include "solucao.h"
#include "utils.h"

Solucao aplica_vnd(Solucao s, Instancia& ins, int k_max);
Solucao busca_melhor_vizinho(Solucao s, Instancia& ins, int k);

#endif