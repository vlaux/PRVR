#ifndef BUSCA_LOCAL
#define BUSCA_LOCAL

#define N_MOVIMENTOS 3

#include "solucao.h"
#include "tabu.h"

Solucao movimento_1(Solucao s, ListaTabu &tabu, int** mapa_rotulos);
Solucao movimento_2(Solucao s, int capacidade, ListaTabu &tabu, int** mapa_rotulos);
Solucao movimento_3(Solucao s, int capacidade, ListaTabu &tabu, int** mapa_rotulos);

#endif