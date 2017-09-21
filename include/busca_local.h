#ifndef BUSCA_LOCAL
#define BUSCA_LOCAL

#define N_MOVIMENTOS 4

#include "solucao.h"
#include "utils.h"

Solucao movimento_intra_rota(Solucao s, Matriz &mapa_rotulos, int k=1, ListaTabu* tabu = nullptr);
Solucao movimento_intra_rota_n_rotas(Solucao s, Matriz &mapa_rotulos, int k=1, ListaTabu* tabu = nullptr);
Solucao movimento_inter_move_n(Solucao s, int capacidade, Matriz &mapa_rotulos, int k=1, ListaTabu* tabu = nullptr);
Solucao movimento_intra_2_opt(Solucao s, Matriz &mapa_rotulos, int k=1, ListaTabu* tabu = nullptr);
// TODO Falta um movimento de swap, talvez
Solucao movimento_perturbacao_cortes(Solucao s, int capacidade, Matriz &mapa_rotulos);

#endif