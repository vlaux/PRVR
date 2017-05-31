#ifndef BUSCA_LOCAL
#define BUSCA_LOCAL

#define N_MOVIMENTOS 4

#include "solucao.h"
#include "tabu.h"

Solucao movimento_intra_rota(Solucao s, int **mapa_rotulos, int k);
Solucao movimento_intra_rota_n_rotas(Solucao s, int **mapa_rotulos, int k);
Solucao movimento_inter_move_n(Solucao s, int capacidade, int **mapa_rotulos, int k);
Solucao movimento_intra_2_opt(Solucao s, int **mapa_rotulos, int k);
// TODO Falta um movimento de swap, talvez
Solucao movimento_perturbacao_cortes(Solucao s, int capacidade, int **mapa_rotulos);

#endif