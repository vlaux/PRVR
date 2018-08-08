// #include <iostream>
// #include <tuple>
// #include <algorithm>
// #include <assert.h>
// #include "solucao.h"
// #include "rota.h"
// #include "lista_tabu.h"

// #define MODULO(x) ((x) >= 0 ? (x) : -(x))

// using namespace std;

// /* 
//  * Movimento intra rota
//  * Troca k clientes de posição dentro de uma mesma rota
//  */
// Solucao movimento_intra_rota(Solucao s, Matriz &mapa_rotulos, int k, ListaTabu* tabu)
// {
//     cout << "Movimento #1 - Intra-rota ";
//     int n_rotas = s.get_n_rotas();
//     if (n_rotas < 1)
//         return s;

//     int id_rota = rand() % n_rotas;
//     Rota *r = s.get_rota_ref(id_rota);

//     cout << "Rota " << id_rota;

//     int tamanho = r->get_tamanho();

//     if (tamanho < 4) // pelo menos dois clientes são necessários: depósito, cliente 1, cliente 2, depósito
//         return s;

//     int pos_origem, pos_destino;

//     for (int i = 0; i < k; i++)
//     {
//         pos_origem = rand() % (tamanho - 2) + 1;
//         pos_destino = pos_origem;

//         while (pos_destino == pos_origem)
//         {
//             pos_destino = rand() % (tamanho - 2) + 1;
//         }

//         if (tabu != nullptr) {
//             cout << "Verificando lista tabu..." << endl;
//             Movimento mov_1 = std::make_tuple(r->clientes[pos_origem].id, id_rota, pos_destino);
//             Movimento mov_2 = std::make_tuple(r->clientes[pos_destino].id, id_rota, pos_origem);

//             if (tabu->is_tabu(mov_1) || tabu->is_tabu(mov_2))
//             {
//                 cout << "-- Movimento Tabu --" << endl;
//                 return s;
//             }
//             else
//             {
//                 tabu->adiciona(mov_1);
//                 tabu->adiciona(mov_2);
//             }
//         }

//         //Efetua a troca
//         std::swap(r->clientes[pos_origem], r->clientes[pos_destino]);

//         s.recalcula_rotulos_utilizados(mapa_rotulos);
//     }

//     cout << "Trocado! Custo agora é " << s.get_custo() << endl;
//     s.imprime();

//     return s;
// }

// /* 
//  * Movimento intra rota para múltiplas rotas
//  * Troca clientes de posição k vezes, em quaisquer rotas da solução
//  */
// Solucao movimento_intra_rota_n_rotas(Solucao s, Matriz &mapa_rotulos, int k, ListaTabu* tabu)
// {
//     cout << "Movimento 1 - Intra rota múltiplas rotas";
//     int n_rotas = s.get_n_rotas();
//     if (n_rotas < 1)
//         return s;

//     int id_rota, tamanho;
//     Rota *r;

//     for (int i = 0; i < k; i++)
//     {
//         id_rota = rand() % n_rotas;
//         r = s.get_rota_ref(id_rota);

//         cout << "Rota " << id_rota;

//         tamanho = r->get_tamanho();

//         if (tamanho < 4) // pelo menos dois clientes são necessários: depósito, cliente 1, cliente 2, depósito
//             return s;

//         int pos_origem = rand() % (tamanho - 2) + 1;
//         int pos_destino = pos_origem;

//         while (pos_destino == pos_origem)
//         {
//             pos_destino = rand() % (tamanho - 2) + 1;
//         }

//         if (tabu != nullptr) {
//             Movimento mov_1 = std::make_tuple(r->clientes[pos_origem].id, id_rota, pos_destino);
//             Movimento mov_2 = std::make_tuple(r->clientes[pos_destino].id, id_rota, pos_origem);

//             if (tabu->is_tabu(mov_1) || tabu->is_tabu(mov_2))
//             {
//                 cout << "-- Movimento Tabu --" << endl;
//                 return s;
//             }
//             else
//             {
//                 tabu->adiciona(mov_1);
//                 tabu->adiciona(mov_2);
//             }
//         }

//         //Efetua a troca
//         std::swap(r->clientes[pos_origem], r->clientes[pos_destino]);

//         s.recalcula_rotulos_utilizados(mapa_rotulos);
//     }

//     s.imprime();
//     cout << "Trocado! Custo agora é " << s.get_custo() << endl;

//     return s;
// }

// /* 
//  * Movimento inter rota
//  * Move k clientes de uma rota para outras rotas
//  */
// Solucao movimento_inter_move_n(Solucao s, int capacidade, Matriz &mapa_rotulos, int k, ListaTabu* tabu)
// {
//     cout << "Movimento 2 - Realocação - ";
//     int n_rotas = s.get_n_rotas();
//     if (n_rotas < 2)
//         return s;

//     int pos_rota_1 = rand() % n_rotas;
//     int pos_rota_2;
//     int posicao_origem, posicao_destino;
//     Rota *rota_1, *rota_2;

//     for (int i = 0; i < k; i++)
//     {
//         pos_rota_2 = pos_rota_1;

//         while (pos_rota_1 == pos_rota_2)
//             pos_rota_2 = rand() % n_rotas;

//         rota_1 = s.get_rota_ref(pos_rota_1);
//         rota_2 = s.get_rota_ref(pos_rota_2);

//         posicao_origem = rand() % (rota_1->get_tamanho() - 2) + 1;
//         posicao_destino = rand() % (rota_2->get_tamanho() - 2) + 1;

//         Cliente cliente_movido = rota_1->clientes[posicao_origem];
//         cout << "cliente " << cliente_movido.id << " na pos " << posicao_origem << " da rota " << pos_rota_1;
//         cout << " para pos " << posicao_destino << " da rota " << pos_rota_2 << endl;

//         if (rota_2->get_carga() + cliente_movido.demanda > capacidade) // capacidade excedida // throw(?)
//         {
//             cout << "-- capacidade excedida --" << endl;
//             return s;
//         }

//         if (tabu != nullptr) {
//             Movimento mov = std::make_tuple(cliente_movido.id, posicao_destino, pos_rota_2);

//             if (tabu->is_tabu(mov)) {
//                 cout << "Movimento tabu!" << endl;
//                 return s;
//             }
//             else {
//                 tabu->adiciona(mov);
//             }
//         }

//         rota_2->clientes.insert(rota_2->clientes.begin() + posicao_destino, cliente_movido);
//         rota_1->clientes.erase(rota_1->clientes.begin() + posicao_origem);

//         if (rota_1->get_tamanho() < 3) // só sobrou o depósito (no início e no fim)
//         {
//             cout << "Só sobrou depósito; remove rota " << pos_rota_1 << endl;
//             s.remove_rota(pos_rota_1);
//             return s;
//         }

//         cout << "Realocado! Custo agora é " << s.get_custo() << endl;
//         s.imprime();
//     }

//     s.recalcula_rotulos_utilizados(mapa_rotulos);

//     return s;
// }

// /* 
//  * Movimento de troca 2-opt
//  * Escolhe dois pontos na rota e inverte todos os clientes entre eles
//  * TODO adicionar movimento Tabu
//  */
// Solucao movimento_intra_2_opt(Solucao s, Matriz &mapa_rotulos, int k, ListaTabu* tabu)
// {
//     cout << "Movimento intra rota 2-opt" << endl;

//     int n_rotas = s.get_n_rotas();
//     if (n_rotas < 1)
//         return s;

//     int pos_rota = rand() % n_rotas;
//     Rota *r = s.get_rota_ref(pos_rota);

//     cout << "Rota " << pos_rota;

//     int tamanho = r->get_tamanho();

//     if (tamanho < (2+k+1)) // pelo menos 2 (depósito no início e fim) + (k+1) clientes necessários
//         return s;

//     int teto_inicio = (tamanho - 2 - k);
//     assert(teto_inicio > 0);

//     int pos_inicio = (rand() % teto_inicio) + 1;
//     assert(pos_inicio > 0);

//     cout << " - revertendo clientes de " << pos_inicio << " até " << pos_inicio + k << endl;

//     if (tabu != nullptr) {
//         std::vector<Movimento> movimentos;

//         for (int i = pos_inicio; i < pos_inicio + k; i++) 
//         {
//             Movimento mov = std::make_tuple(r->clientes[i].id, pos_rota, pos_inicio + k - i);
//             if (tabu->is_tabu(mov)) 
//             {
//                 cout << "Movimento Tabu" << endl;
//                 return s;
//             } 
//             else 
//             {
//                 movimentos.push_back(mov);
//             }
//         }
//         for_each(movimentos.begin(), movimentos.end(), [tabu](Movimento mov) { tabu->adiciona(mov); });
//     }

//     std::reverse(r->clientes.begin() + pos_inicio, r->clientes.begin() + pos_inicio + k + 1);

//     s.recalcula_rotulos_utilizados(mapa_rotulos);
//     s.imprime();

//     return s;
// }

// /* 
//  * Movimento de perturbação
//  * Cria k pontos de corte em um par de rotas e alterna entre esses pontos
//  * TODO: atualmente, só funciona para k = 1 (um ponto de corte em cada rota)
//  */
// Solucao movimento_perturbacao_cortes(Solucao s, int capacidade, Matriz &mapa_rotulos)
// {
//     cout << "Movimento de perturbação ";
//     int n_rotas = s.get_n_rotas();
//     if (n_rotas < 2)
//         return s;

//     int pos_rota_1 = rand() % n_rotas;
//     int pos_rota_2 = pos_rota_1;

//     while (pos_rota_1 == pos_rota_2)
//         pos_rota_2 = rand() % n_rotas;

//     Rota rota_1 = s.get_rota(pos_rota_1);
//     Rota rota_2 = s.get_rota(pos_rota_2);

//     int ponto_corte_rota_1 = rand() % (rota_1.get_tamanho() - 2) + 1;
//     int ponto_corte_rota_2 = rand() % (rota_2.get_tamanho() - 2) + 1;

//     cout << "Rota " << pos_rota_1 << ":ponto de corte " << ponto_corte_rota_1;
//     cout << " e Rota " << pos_rota_2 << ":ponto de corte " << ponto_corte_rota_2 << endl;

//     Rota nova_rota_1;
//     Rota nova_rota_2;

//     for (std::vector<Cliente>::iterator it = rota_1.clientes.begin(); it != rota_1.clientes.begin() + ponto_corte_rota_1; it++)
//         nova_rota_1.clientes.push_back(*it);
//     for (std::vector<Cliente>::iterator it = rota_2.clientes.begin() + ponto_corte_rota_2; it != rota_2.clientes.end(); it++)
//         nova_rota_1.clientes.push_back(*it);

//     if (nova_rota_1.get_carga() > capacidade)
//     {
//         cout << "-- capacidade excedida r1 --" << endl;
//         return s;
//     }

//     for (std::vector<Cliente>::iterator it = rota_2.clientes.begin(); it != rota_2.clientes.begin() + ponto_corte_rota_2; it++)
//         nova_rota_2.clientes.push_back(*it);
//     for (std::vector<Cliente>::iterator it = rota_1.clientes.begin() + ponto_corte_rota_1; it != rota_1.clientes.end(); it++)
//         nova_rota_2.clientes.push_back(*it);

//     if (nova_rota_2.get_carga() > capacidade)
//     {
//         cout << "-- capacidade excedida r2--" << endl;
//         return s;
//     }

//     s.update_rota(nova_rota_1, pos_rota_1);
//     s.update_rota(nova_rota_2, pos_rota_2);

//     //remove a rota caso ela tenha virado uma rota com apenas depósito [0-0]
//     if (nova_rota_1.get_tamanho() < 3)
//         s.remove_rota(pos_rota_1);

//     if (nova_rota_2.get_tamanho() < 3)
//         s.remove_rota(pos_rota_2);

//     s.recalcula_rotulos_utilizados(mapa_rotulos);
//     s.imprime();

//     cout << "Movimento 2opt aplicado! Novo custo é " << s.get_custo() << endl;

//     return s;
// }
