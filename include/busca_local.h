#ifndef BUSCA_LOCAL
#define BUSCA_LOCAL

#include "solucao.h"
#include "instancia.h"
#include "lista_tabu.h"

// #define N_MOVIMENTOS 4

// Movimentos INTRA-rota
Solucao movimento_intra_realoacao(Solucao &s, int tam = 1, ListaTabu* tabu = nullptr);
Solucao movimento_2_opt(Solucao &s, ListaTabu* tabu = nullptr);
Solucao movimento_or_opt(Solucao &s, ListaTabu* tabu = nullptr);
Solucao movimento_mix_intra(Solucao &s, ListaTabu* tabu = nullptr);

// Movimentos INTER-rota
Solucao movimento_corte_cruzado(Solucao &s, ListaTabu* tabu = nullptr);
Solucao movimento_troca_conjuntos(Solucao &s, ListaTabu* tabu = nullptr);
Solucao movimento_realocacao_conjuntos(Solucao &s, ListaTabu* tabu = nullptr);
Solucao movimento_mix_inter(Solucao &s, ListaTabu* tabu = nullptr);

// perturbações isoladas, usando movimentos inter-rota
Solucao perturbacao_realocacao_conjuntos(Solucao &s);
Solucao perturbacao_corte_cruzado(Solucao &s);
Solucao perturbacao_troca_conjuntos(Solucao &s);

// qualquer movimento (sem mix inter - normalmente usado como perturbacao)
Solucao busca_local_aleatoria(Solucao &s, int n_iter, ListaTabu* tabu = nullptr);

// helpers
Solucao _perturba_corte_cruzado(Solucao &s_temp, int id_rota_1, int id_rota_2, int ponto_corte_rota_1, int ponto_corte_rota2);
Solucao _perturba_realocacao_conjuntos(Solucao &s_temp, int id_rota_1, int id_rota_2, int inicio_conjunto_r1, int posicao_r2, int tamanho);
Solucao _perturba_troca_conjuntos(Solucao &s_temp, int id_rota_1, int id_rota_2, int inicio_conjunto_r1, int inicio_conjunto_r2, int tamanho);

#endif