#include <iostream>
#include <tuple>
#include <algorithm>
#include <assert.h>
#include "solucao.h"
#include "rota.h"
#include "lista_tabu.h"
#include "utils.h"
#include "busca_local.h"

#define MODULO(x) ((x) >= 0 ? (x) : -(x))

using namespace std;

/* 
 * Movimento intra rota
 * Realoca k clientes dentro de uma mesma rota
 * Testa movimento em todas as rotas
 */
Solucao movimento_intra_realoacao(Solucao &s, int tam, ListaTabu* tabu)
{
    cout << "Movimento intra-rota de realocação";

    Solucao s_temp = s;

    int n_rotas = s_temp.get_n_rotas();

    for (int id_rota = 0; id_rota < n_rotas; id_rota++) {
        Rota rota_original = s_temp.get_rota(id_rota);
        Rota* r = s_temp.get_rota_ref(id_rota);

        cout << "Rota " << id_rota;

        int tamanho = r->get_tamanho();

        for (int pos_cliente = 1; pos_cliente < tamanho - tam - 1; pos_cliente++) {

            vector<Cliente> clientes_realocados(&r->clientes.at(pos_cliente), &r->clientes.at(pos_cliente+tam));

            // tenta realocá-lo em todas as outras posições
            // começa em 1 pq não troca com depósito
            // tamanho -2 pra não trocar com o depósito na última posição && desconta posição que foi removido
            for (int i = 1; i < tamanho - tam - 1; i++)
            {
                r->clientes.erase(r->clientes.begin() + pos_cliente, r->clientes.begin() + pos_cliente + tam);

                // não tenta realocar na mesma posição
                if (i == pos_cliente) continue;

                r->clientes.insert(r->clientes.begin()+i, clientes_realocados.begin(), clientes_realocados.end());

                s_temp.recalcula_rotulos_utilizados();

                // encontrou movimento aprimorante        
                if (s_temp.get_custo() < s.get_custo()) {

                    // se existir lista tabu para verificar...
                    if (tabu != nullptr) {
                        // cout << "Verificando lista tabu..." << endl;
                        // vector<int> lista_clientes = {c.id};
                        // Movimento mov = std::make_tuple(id_rota, i, lista_clientes);

                        // // se for tabu, ignora
                        // if (tabu->is_tabu(mov))
                        // {
                        //     cout << "-- Movimento Tabu --" << endl;
                        //     continue;
                        // }
                        // else
                        //     tabu->adiciona(mov);
                    }

                    // estratégia primeiro aprimorante
                    cout << "movimento aprimorante encontrado" << endl;
                    return s_temp;
                }

                // não melhorou, restaura rota pro estado anterior ao movimento
                else {
                    s_temp.rotas[id_rota] = rota_original;
                    s_temp.recalcula_rotulos_utilizados();
                }
            }

            s_temp.rotas[id_rota] = rota_original;
            s_temp.recalcula_rotulos_utilizados();

        }
    }

    throw NENHUM_MOVIMENTO;
}

Solucao movimento_2_opt(Solucao &s, ListaTabu* tabu)
{
    cout << "Movimento 2 opt" << endl;

    Solucao s_temp = s;

    int n_rotas = s_temp.get_n_rotas();

    for (int id_rota = 0; id_rota < n_rotas; id_rota++) {
        Rota rota_original = s_temp.get_rota(id_rota);
        Rota* r = s_temp.get_rota_ref(id_rota);

        cout << "Rota " << id_rota << endl;

        int tamanho = r->get_tamanho();

        for (int i = 1; i < tamanho; i++) {
            for (int j = i + 2; j < tamanho; j++) {
    
                std::reverse(r->clientes.begin() + i, r->clientes.begin() + j);
                s_temp.recalcula_rotulos_utilizados();

                if (s_temp.get_custo() < s.get_custo()) {
                    // se existir lista tabu para verificar...
                    if (tabu != nullptr) {
                        cout << "Verificando lista tabu..." << endl;
                        vector<int> lista_clientes;
                        for (int t = i; t < j; t++)
                            lista_clientes.push_back(r->clientes.at(t).id);

                        Movimento mov = std::make_tuple(id_rota, i, lista_clientes);

                        // se for tabu, ignora
                        if (tabu->is_tabu(mov))
                        {
                            cout << "-- Movimento Tabu --" << endl;
                            continue;
                        }
                        //else
                            //tabu->adiciona(mov);
                    }

                    // estratégia primeiro aprimorante
                    cout << "movimento aprimorante encontrado" << endl;
                    return s_temp;
                }

                else {
                    s_temp.rotas[id_rota] = rota_original;
                    s_temp.recalcula_rotulos_utilizados();
                }

            }
        }
    }

    throw NENHUM_MOVIMENTO;

}

Solucao movimento_or_opt(Solucao &s, ListaTabu* tabu) {
    try {
        return movimento_intra_realoacao(s, 1, tabu);
    } catch (int e) {
        cout << "não conseguiu, indo para or opt 2" << endl;
    }

    try {
        return movimento_intra_realoacao(s, 2, tabu);
    } catch (int e) {
        cout << "não conseguiu, indo para or opt 3" << endl;
    }

    try {
        return movimento_intra_realoacao(s, 3, tabu);
    } catch (int e) {
        cout << "não conseguiu nada" << endl;
    }

    throw NENHUM_MOVIMENTO;
}

Solucao movimento_mix_intra(Solucao &s, ListaTabu* tabu) {
    Solucao s_best = s;
    vector<int> movimentos = {0, 1, 2};
    utils::random::shuffle(movimentos.begin(), movimentos.end());
    for (int i = 0; i < movimentos.size(); i++) {
        
        switch (movimentos[i])
        {
            case 0:
                try {
                    Solucao s_temp = s;
                    int tam = (rand() % 2) + 1;
                    s_temp = movimento_intra_realoacao(s_temp, tam, tabu);
                    if (s_temp.get_custo() < s_best.get_custo())
                        s_best = s_temp;
                } catch (int e) {}
                break;
        
            case 1:
                try {
                    Solucao s_temp = s;
                    s_temp = movimento_2_opt(s_temp, tabu);
                    if (s_temp.get_custo() < s_best.get_custo())
                        s_best = s_temp;
                } catch (int e) {}
                break;
            
            case 2:
                try {
                    Solucao s_temp = s;
                    s_temp = movimento_or_opt(s_temp, tabu);
                    if (s_temp.get_custo() < s_best.get_custo())
                        s_best = s_temp;
                } catch (int e) {}
                break;

            default:
                break;
        }
    }

    return s_best;
}


Solucao movimento_corte_cruzado(Solucao &s, ListaTabu* tabu) {
    int n_rotas = s.get_n_rotas();
    if (n_rotas < 2)
        throw SEM_ROTAS;

    Solucao s_temp = s;

    int pos_rota_1 = rand() % n_rotas;
    int pos_rota_2 = pos_rota_1;

    while (pos_rota_1 == pos_rota_2)
        pos_rota_2 = rand() % n_rotas;

    Rota rota_1 = s_temp.get_rota(pos_rota_1);
    Rota rota_2 = s_temp.get_rota(pos_rota_2);

    for (int ponto_corte_rota_1 = 0; ponto_corte_rota_1 < rota_1.get_tamanho() - 1; ponto_corte_rota_1++) {
        for (int ponto_corte_rota_2 = 0; ponto_corte_rota_2 < rota_2.get_tamanho() - 1; ponto_corte_rota_2++) {

            Rota nova_rota_1;
            Rota nova_rota_2;

            for (std::vector<Cliente>::iterator it = rota_1.clientes.begin(); it != rota_1.clientes.begin() + ponto_corte_rota_1 + 1; it++)
                nova_rota_1.clientes.push_back(*it);
            for (std::vector<Cliente>::iterator it = rota_2.clientes.begin() + ponto_corte_rota_2 + 1; it != rota_2.clientes.end(); it++)
                nova_rota_1.clientes.push_back(*it);

            if (nova_rota_1.get_carga() > s.get_instancia()->get_capacidade())
            {
                cout << "-- capacidade excedida r1 --" << endl;
                continue;
            }

            for (std::vector<Cliente>::iterator it = rota_2.clientes.begin(); it != rota_2.clientes.begin() + ponto_corte_rota_2 + 1; it++)
                nova_rota_2.clientes.push_back(*it);
            for (std::vector<Cliente>::iterator it = rota_1.clientes.begin() + ponto_corte_rota_1 + 1; it != rota_1.clientes.end(); it++)
                nova_rota_2.clientes.push_back(*it);

            if (nova_rota_2.get_carga() > s.get_instancia()->get_capacidade())
            {
                cout << "-- capacidade excedida r2--" << endl;
                continue;
            }

            s_temp.update_rota(nova_rota_1, pos_rota_1);
            s_temp.update_rota(nova_rota_2, pos_rota_2);

            //remove a rota caso ela tenha virado uma rota com apenas depósito [0-0]
            if (nova_rota_1.get_tamanho() < 3)
                s_temp.remove_rota(pos_rota_1);

            if (nova_rota_2.get_tamanho() < 3)
                s_temp.remove_rota(pos_rota_2);

            s_temp.recalcula_rotulos_utilizados();

            if (s_temp.get_custo() < s.get_custo()) {
                // verifica tabu

                cout << "TROCA CORTE CRUZADO ENCONTRADO: Rotas " << pos_rota_1 << " e " << pos_rota_2 << " posicoes " << ponto_corte_rota_1 << " e " << ponto_corte_rota_2 << endl;

                return s_temp;
            } else {
                s_temp = s;
            }

        }
    }

    throw NENHUM_MOVIMENTO;
}

Solucao movimento_troca_conjuntos(Solucao &s, ListaTabu* tabu) {
    int n_rotas = s.get_n_rotas();
    if (n_rotas < 2)
        throw SEM_ROTAS;

    Solucao s_temp = s;

    int pos_rota_1 = rand() % n_rotas;
    int pos_rota_2 = pos_rota_1;

    while (pos_rota_1 == pos_rota_2)
        pos_rota_2 = rand() % n_rotas;

    int tamanho = (rand() % 2) + 1;

    for (int inicio_conjunto_r1 = 1; inicio_conjunto_r1 < s.get_rota(pos_rota_1).get_tamanho() - tamanho; inicio_conjunto_r1++) {
        for (int inicio_conjunto_r2 = 1; inicio_conjunto_r2 < s.get_rota(pos_rota_2).get_tamanho() - tamanho; inicio_conjunto_r2++) {
            Rota* rota_1 = s_temp.get_rota_ref(pos_rota_1);
            Rota* rota_2 = s_temp.get_rota_ref(pos_rota_2);

            for (int k = 0; k < tamanho; k++)
                swap(rota_1->clientes[inicio_conjunto_r1 + k], rota_2->clientes[inicio_conjunto_r2 + k]);

            if (rota_1->get_carga() > s.get_instancia()->get_capacidade())
            {
                cout << "-- capacidade excedida r1 --" << endl;
                s_temp = s;
                continue;
            }

            if (rota_2->get_carga() > s.get_instancia()->get_capacidade())
            {
                cout << "-- capacidade excedida r2--" << endl;
                s_temp = s;
                continue;
            }

            s_temp.recalcula_rotulos_utilizados();

            if (s_temp.get_custo() < s.get_custo()) {
                // verifica tabu
                cout << endl<< "troca de conjuntos com sucesso. rotas: " << pos_rota_1 << " e " << pos_rota_2 << " tam: " << tamanho << " pos: " << inicio_conjunto_r1 << " e " << inicio_conjunto_r2 << endl;

                return s_temp;
            } else {
                s_temp = s;
            }

        }
    }

    throw NENHUM_MOVIMENTO;
}

Solucao movimento_realocacao_conjuntos(Solucao &s, ListaTabu* tabu) {
    int n_rotas = s.get_n_rotas();
    if (n_rotas < 2)
        throw SEM_ROTAS;

    Solucao s_temp = s;

    int pos_rota_1 = rand() % n_rotas;
    int pos_rota_2 = pos_rota_1;

    while (pos_rota_1 == pos_rota_2)
        pos_rota_2 = rand() % n_rotas;

    int tamanho = (rand() % 2) + 1;

    for (int inicio_conjunto_r1 = 1; inicio_conjunto_r1 < s_temp.get_rota(pos_rota_1).get_tamanho() - tamanho; inicio_conjunto_r1++) {
        for (int posicao_r2 = 1; posicao_r2 < s_temp.get_rota(pos_rota_2).get_tamanho() - 1; posicao_r2++) {
            Rota* rota_1 = s_temp.get_rota_ref(pos_rota_1);
            Rota* rota_2 = s_temp.get_rota_ref(pos_rota_2);

            rota_2->clientes.insert(rota_2->clientes.begin() + posicao_r2, rota_1->clientes.begin() + inicio_conjunto_r1, rota_1->clientes.begin() + inicio_conjunto_r1 + tamanho);
            rota_1->clientes.erase(rota_1->clientes.begin() + inicio_conjunto_r1, rota_1->clientes.begin() + inicio_conjunto_r1 + tamanho);

            if (rota_2->get_carga() > s.get_instancia()->get_capacidade())
            {
                cout << "-- capacidade excedida r2--" << endl;
                s_temp = s;
                continue;
            }

            if (rota_1->get_tamanho() < 3) {
                s_temp.remove_rota(pos_rota_1);
            }

            if (rota_2->get_tamanho() < 3) {
                s_temp.remove_rota(pos_rota_2);
            }

            s_temp.recalcula_rotulos_utilizados();

            if (s_temp.get_custo() < s.get_custo()) {
                // verifica tabu
                cout << endl<< "realocao de conjuntos com sucesso. rotas: " << pos_rota_1 << " e " << pos_rota_2 << " tam: " << tamanho << " pos: " << inicio_conjunto_r1 << " e " << posicao_r2 << endl;

                return s_temp;
            } else {
                s_temp = s;
            }

        }
    }

    throw NENHUM_MOVIMENTO;
}

Solucao movimento_mix_inter(Solucao &s, ListaTabu* tabu) {
    Solucao s_best = s;
    vector<int> movimentos = {0, 1, 2};
    utils::random::shuffle(movimentos.begin(), movimentos.end());
    for (int i = 0; i < movimentos.size(); i++) {
        
        switch (movimentos[i])
        {
            case 0:
                try {
                    Solucao s_temp = s;
                    s_temp = movimento_corte_cruzado(s_temp, tabu);
                    if (s_temp.get_custo() < s_best.get_custo())
                        s_best = s_temp;
                } catch (int e) {}
                break;
        
            case 1:
                try {
                    Solucao s_temp = s;
                    s_temp = movimento_troca_conjuntos(s_temp, tabu);
                    if (s_temp.get_custo() < s_best.get_custo())
                        s_best = s_temp;
                } catch (int e) {}
                break;
            
            case 2:
                try {
                    Solucao s_temp = s;
                    s_temp = movimento_realocacao_conjuntos(s_temp, tabu);
                    if (s_temp.get_custo() < s_best.get_custo())
                        s_best = s_temp;
                } catch (int e) {}
                break;

            default:
                break;
        }
    }

    return s_best;
}

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
