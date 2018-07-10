#ifndef BUSCA_LOCAL
#define BUSCA_LOCAL

#include "solucao.h"
#include "instancia.h"

// #define N_MOVIMENTOS 4

// Movimentos INTRA-rota
Solucao movimento_intra_realoacao(Solucao &s, int tam = 1, ListaTabu* tabu = nullptr);
Solucao movimento_2_opt(Solucao &s, ListaTabu* tabu = nullptr);
Solucao movimento_or_opt(Solucao &s, ListaTabu* tabu = nullptr);
Solucao movimento_mix_intra(Solucao &s, ListaTabu* tabu = nullptr);

// Movimentos INTER-rota
Solucao movimento_corte_cruzado(Solucao &s, ListaTabu* tabu = nullptr);

// Solucao movimento_intra_rota(Solucao s, Matriz &mapa_rotulos, int k=1, ListaTabu* tabu = nullptr);
// Solucao movimento_intra_rota_n_rotas(Solucao s, Matriz &mapa_rotulos, int k=1, ListaTabu* tabu = nullptr);
// Solucao movimento_inter_move_n(Solucao s, int capacidade, Matriz &mapa_rotulos, int k=1, ListaTabu* tabu = nullptr);
// Solucao movimento_intra_2_opt(Solucao s, Matriz &mapa_rotulos, int k=1, ListaTabu* tabu = nullptr);
// // TODO Falta um movimento de swap, talvez
// Solucao movimento_perturbacao_cortes(Solucao s, int capacidade, Matriz &mapa_rotulos);

#endif