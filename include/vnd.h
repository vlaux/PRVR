#ifndef VND
#define VND

#include "solucao.h"

Solucao* aplica_vnd(Solucao* s, Instancia* ins, int k_max);
Solucao* encontra_melhor_solucao_vizinhanca(Solucao *s, Instancia* ins, int k);

#endif