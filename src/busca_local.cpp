#include <iostream>
#include <tuple>
#include <algorithm>
#include <assert.h>
#include <cstdlib>
#include "solucao.h"
#include "rota.h"
#include "lista_tabu.h"
#include "utils.h"
#include "busca_local.h"

#define MODULO(x) ((x) >= 0 ? (x) : -(x))

#define N_MOVIMENTOS 8
#define MAX_TENTATIVAS 10

using namespace std;


/* 
 * Movimento intra rota
 * Realoca k clientes dentro de uma mesma rota
 * Testa movimento em todas as rotas
 */
Solucao movimento_intra_realoacao(Solucao &s, int tam, ListaTabu* tabu)
{
    cout << "Movimento intra-rota de realocação" << endl;

    Solucao s_temp = s;

    int n_rotas = s_temp.get_n_rotas();

    for (int id_rota = 0; id_rota < n_rotas; id_rota++) {
        Rota rota_original = s_temp.get_rota(id_rota);
        Rota* r = s_temp.get_rota_ref(id_rota);

        int tamanho = r->get_tamanho();

        for (int pos_cliente = 1; pos_cliente < tamanho - tam; pos_cliente++) {

            vector<Cliente> clientes_realocados(&r->clientes.at(pos_cliente), &r->clientes.at(pos_cliente+tam));

            // tenta realocá-lo em todas as outras posições
            // começa em 1 pq não troca com depósito
            // tamanho -2 pra não trocar com o depósito na última posição && desconta posição que foi removido
            for (int i = 1; i < tamanho - tam; i++)
            {
                // não tenta realocar na mesma posição
                if (i == pos_cliente) continue;

                r->clientes.erase(r->clientes.begin() + pos_cliente, r->clientes.begin() + pos_cliente + tam);
                r->clientes.insert(r->clientes.begin()+i, clientes_realocados.begin(), clientes_realocados.end());

                s_temp.recalcula_rotulos_utilizados();

                // encontrou movimento aprimorante        
                if (s_temp.get_custo() < s.get_custo()) {

                    // se existir lista tabu para verificar...
                    if (tabu != nullptr) {
                        vector<int> lista_clientes(clientes_realocados.size());
                        for(unsigned int i = 0; i < clientes_realocados.size(); i++)
                            lista_clientes.push_back(clientes_realocados.at(i).id);
                        
                        Movimento mov = std::make_tuple(id_rota, lista_clientes);
                        tabu->adiciona(mov);

                        if (tabu->is_tabu(mov))
                        {
                            cout << "-- Movimento Tabu --" << endl;
                            s_temp = s;
                            continue;
                        }
                        else
                            tabu->adiciona(mov);
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

        int tamanho = r->get_tamanho();

        for (int i = 1; i < tamanho; i++) {
            for (int j = i + 1; j < (tamanho - 1); j++) {
    
                std::reverse(r->clientes.begin() + i, r->clientes.begin() + j);
                s_temp.recalcula_rotulos_utilizados();

                if (s_temp.get_custo() < s.get_custo()) {
                    // se existir lista tabu para verificar...
                    if (tabu != nullptr) {
                        vector<int> lista_clientes;
                        for (int t = i; t < j; t++)
                            lista_clientes.push_back(rota_original.clientes.at(t).id);

                        Movimento mov = std::make_tuple(id_rota, lista_clientes);

                        if (tabu->is_tabu(mov))
                        {
                            cout << "-- Movimento Tabu --" << endl;
                            s_temp = s;
                            continue;
                        }
                        else
                            tabu->adiciona(mov);
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
    for (unsigned int i = 0; i < movimentos.size(); i++) {
        
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



// MOVIMENTOS INTER ROTA

Solucao perturbacao_corte_cruzado(Solucao &s, unsigned int tentativa) {
    int n_rotas = s.get_n_rotas();
    if (n_rotas < 2)
        throw SEM_ROTAS;

    Solucao s_temp = s;

    int id_rota_1 = rand() % n_rotas;
    int id_rota_2 = id_rota_1;

    while (id_rota_1 == id_rota_2)
        id_rota_2 = rand() % n_rotas;

    int ponto_corte_rota_1 = rand() % (s.get_rota(id_rota_1).get_tamanho() - 1);
    int ponto_corte_rota_2 = rand() % (s.get_rota(id_rota_2).get_tamanho() - 1);

    try {
        return _perturba_corte_cruzado(s_temp, id_rota_1, id_rota_2, ponto_corte_rota_1, ponto_corte_rota_2);
    } catch (int e) {
        if (tentativa > MAX_TENTATIVAS) return s;
        return perturbacao_corte_cruzado(s, tentativa++);
    }
}

Solucao perturbacao_troca_conjuntos(Solucao &s, unsigned int tentativa) {
    int n_rotas = s.get_n_rotas();
    if (n_rotas < 2)
        throw SEM_ROTAS;

    Solucao s_temp = s;

    int id_rota_1 = rand() % n_rotas;
    int id_rota_2 = id_rota_1;

    while (id_rota_1 == id_rota_2)
        id_rota_2 = rand() % n_rotas;

    int tamanho = (rand() % 2) + 1;

    if (s.get_rota(id_rota_1).get_tamanho() <= 3 || s.get_rota(id_rota_2).get_tamanho() <= 3)
        tamanho = 1;

    int inicio_conjunto_r1 = (rand() % (s.get_rota(id_rota_1).get_tamanho() - tamanho - 1)) + 1;
    int inicio_conjunto_r2 = (rand() % (s.get_rota(id_rota_2).get_tamanho() - tamanho - 1)) + 1;

    try {
        return _perturba_troca_conjuntos(s_temp, id_rota_1, id_rota_2, inicio_conjunto_r1, inicio_conjunto_r2, tamanho);
    } catch (int e) {
        if (tentativa > MAX_TENTATIVAS) return s;
        return perturbacao_troca_conjuntos(s, tentativa++);
    }
}

Solucao perturbacao_realocacao_conjuntos(Solucao &s, unsigned int tentativa) {
    int n_rotas = s.get_n_rotas();
    if (n_rotas < 2)
        throw SEM_ROTAS;

    Solucao s_temp = s;

    int id_rota_1 = rand() % n_rotas;
    int id_rota_2 = id_rota_1;

    while (id_rota_1 == id_rota_2)
        id_rota_2 = rand() % n_rotas;

    int tamanho = (rand() % 2) + 1;
    if (s.get_rota(id_rota_1).get_tamanho() <= 3)
        tamanho = 1;

    int inicio_conjunto_r1 = (rand() % (s.get_rota(id_rota_1).get_tamanho() - tamanho - 1)) + 1;
    int posicao_r2 = (rand() % (s.get_rota(id_rota_2).get_tamanho() - 1)) + 1;

    try {
        return _perturba_realocacao_conjuntos(s_temp, id_rota_1, id_rota_2, inicio_conjunto_r1, posicao_r2, tamanho);
    } catch (int e) {
        if (tentativa > MAX_TENTATIVAS) return s;
        return perturbacao_realocacao_conjuntos(s, tentativa++);
    }
}

Solucao _perturba_corte_cruzado(Solucao &s_temp, int id_rota_1, int id_rota_2, int ponto_corte_rota_1, int ponto_corte_rota_2) {
    Rota rota_original_1 = s_temp.get_rota(id_rota_1);
    Rota rota_original_2 = s_temp.get_rota(id_rota_2);

    Rota nova_rota_1;
    Rota nova_rota_2;

    for (int i = 0; i <= ponto_corte_rota_1; i++)
        nova_rota_1.clientes.push_back(rota_original_1.clientes[i]);
    for (int i = ponto_corte_rota_2 + 1; i < rota_original_2.get_tamanho(); i++)
        nova_rota_1.clientes.push_back(rota_original_2.clientes[i]);

    if (nova_rota_1.get_carga() > s_temp.get_instancia()->get_capacidade())
    {
        cout << "-- capacidade excedida r1 --" << endl;
        throw CAPACIDADE_EXCEDIDA;
    }

    for (int i = 0; i <= ponto_corte_rota_2; i++)
        nova_rota_2.clientes.push_back(rota_original_2.clientes[i]);
    for (int i = ponto_corte_rota_1 + 1; i < rota_original_1.get_tamanho(); i++)
        nova_rota_2.clientes.push_back(rota_original_1.clientes[i]);

    if (nova_rota_2.get_carga() > s_temp.get_instancia()->get_capacidade())
    {
        cout << "-- capacidade excedida r2--" << endl;
        throw CAPACIDADE_EXCEDIDA;
    }

    s_temp.update_rota(nova_rota_1, id_rota_1);
    s_temp.update_rota(nova_rota_2, id_rota_2);

    //remove a rota caso ela tenha virado uma rota com apenas depósito [0-0]
    if (nova_rota_1.get_tamanho() < 3)
        s_temp.remove_rota(id_rota_1);

    if (nova_rota_2.get_tamanho() < 3)
        s_temp.remove_rota(id_rota_2);

    s_temp.recalcula_rotulos_utilizados();

    return s_temp;
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

    for (int ponto_corte_rota_1 = 0; s.get_n_rotas() > pos_rota_1 && ponto_corte_rota_1 < (s.get_rota(pos_rota_1).get_tamanho() - 1); ponto_corte_rota_1++) {
        for (int ponto_corte_rota_2 = 0; s.get_n_rotas() > pos_rota_2 && (ponto_corte_rota_2 < s.get_rota(pos_rota_2).get_tamanho() - 1); ponto_corte_rota_2++) {

            try {
                s_temp = _perturba_corte_cruzado(s_temp, pos_rota_1, pos_rota_2, ponto_corte_rota_1, ponto_corte_rota_2);
            } catch (int e) {
                s_temp = s;
                continue;
            }

            if (s_temp.get_custo() < s.get_custo()) {
                if (tabu != nullptr) {
                    vector<int> lista_clientes;
                    for (int i = ponto_corte_rota_1 + 1; i < s.get_rota(pos_rota_1).get_tamanho(); i++)
                        lista_clientes.push_back(s.get_rota(pos_rota_1).clientes.at(i).id);
                    Movimento mov_1 = std::make_tuple(pos_rota_1, lista_clientes);
                    
                    lista_clientes.clear();
                    for (int i = ponto_corte_rota_2 + 1; i < s.get_rota(pos_rota_2).get_tamanho(); i++)
                        lista_clientes.push_back(s.get_rota(pos_rota_2).clientes.at(i).id);
                    Movimento mov_2 = std::make_tuple(pos_rota_2, lista_clientes);

                    if (tabu->is_tabu(mov_1) || tabu->is_tabu(mov_2)) {
                        s_temp = s;
                        continue;
                    } else {
                        tabu->adiciona(mov_1);
                        tabu->adiciona(mov_2);
                    }
                }

                cout << "TROCA CORTE CRUZADO ENCONTRADO: Rotas " << pos_rota_1 << " e " << pos_rota_2 << " posicoes " << ponto_corte_rota_1 << " e " << ponto_corte_rota_2 << endl;

                return s_temp;
            } else {
                s_temp = s;
            }

        }
    }

    throw NENHUM_MOVIMENTO;
}

Solucao _perturba_troca_conjuntos(Solucao &s_temp, int id_rota_1, int id_rota_2, int inicio_conjunto_r1, int inicio_conjunto_r2, int tamanho) {
    Rota* rota_1 = s_temp.get_rota_ref(id_rota_1);
    Rota* rota_2 = s_temp.get_rota_ref(id_rota_2);

    for (int k = 0; k < tamanho; k++)
        swap(rota_1->clientes[inicio_conjunto_r1 + k], rota_2->clientes[inicio_conjunto_r2 + k]);

    if (rota_1->get_carga() > s_temp.get_instancia()->get_capacidade())
        throw CAPACIDADE_EXCEDIDA;
    
    if (rota_2->get_carga() > s_temp.get_instancia()->get_capacidade())
        throw CAPACIDADE_EXCEDIDA;

    s_temp.recalcula_rotulos_utilizados();

    return s_temp;
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
    if (s.get_rota(pos_rota_1).get_tamanho() <= 3 || s.get_rota(pos_rota_2).get_tamanho() <= 3)
        tamanho = 1;

    for (int inicio_conjunto_r1 = 1; inicio_conjunto_r1 < s.get_rota(pos_rota_1).get_tamanho() - tamanho; inicio_conjunto_r1++) {
        for (int inicio_conjunto_r2 = 1; inicio_conjunto_r2 < s.get_rota(pos_rota_2).get_tamanho() - tamanho; inicio_conjunto_r2++) {
            
            try {
                s_temp = _perturba_troca_conjuntos(s_temp, pos_rota_1, pos_rota_2, inicio_conjunto_r1, inicio_conjunto_r2, tamanho);
            } catch (int e) {
                s_temp = s;
                continue;
            }

            if (s_temp.get_custo() < s.get_custo()) {
                if (tabu != nullptr) {
                    vector<int> lista_clientes;
                    for (int i = inicio_conjunto_r1; i < inicio_conjunto_r1 + tamanho; i++)
                        lista_clientes.push_back(s.get_rota(pos_rota_1).clientes.at(i).id);
                    Movimento mov_1 = std::make_tuple(pos_rota_1, lista_clientes);
                    
                    lista_clientes.clear();
                    for (int i = inicio_conjunto_r2; i < inicio_conjunto_r2 + tamanho; i++)
                        lista_clientes.push_back(s.get_rota(pos_rota_2).clientes.at(i).id);
                    Movimento mov_2 = std::make_tuple(pos_rota_2, lista_clientes);

                    if (tabu->is_tabu(mov_1) || tabu->is_tabu(mov_2)) {
                        s_temp = s;
                        continue;
                    } else {
                        tabu->adiciona(mov_1);
                        tabu->adiciona(mov_2);
                    }
                }

                cout << endl<< "troca de conjuntos com sucesso. rotas: " << pos_rota_1 << " e " << pos_rota_2 << " tam: " << tamanho << " pos: " << inicio_conjunto_r1 << " e " << inicio_conjunto_r2 << endl;

                return s_temp;
            }

            s_temp = s;
        }
    }

    throw NENHUM_MOVIMENTO;
}

Solucao _perturba_realocacao_conjuntos(Solucao &s_temp, int id_rota_1, int id_rota_2, int inicio_conjunto_r1, int posicao_r2, int tamanho) {
    Rota* rota_1 = s_temp.get_rota_ref(id_rota_1);
    Rota* rota_2 = s_temp.get_rota_ref(id_rota_2);

    rota_2->clientes.insert(rota_2->clientes.begin() + posicao_r2, rota_1->clientes.begin() + inicio_conjunto_r1, rota_1->clientes.begin() + inicio_conjunto_r1 + tamanho);
    rota_1->clientes.erase(rota_1->clientes.begin() + inicio_conjunto_r1, rota_1->clientes.begin() + inicio_conjunto_r1 + tamanho);

    if (rota_2->get_carga() > s_temp.get_instancia()->get_capacidade())
        throw CAPACIDADE_EXCEDIDA;

    if (rota_1->get_tamanho() < 3) {
        s_temp.remove_rota(id_rota_1);
    }

    if (rota_2->get_tamanho() < 3) {
        s_temp.remove_rota(id_rota_2);
    }

    s_temp.recalcula_rotulos_utilizados();

    return s_temp;
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

    if (s.get_rota(pos_rota_1).get_tamanho() <= 3)
        tamanho = 1;

    for (int inicio_conjunto_r1 = 1; s_temp.get_n_rotas() > pos_rota_1 && inicio_conjunto_r1 < (s_temp.get_rota(pos_rota_1).get_tamanho() - tamanho); inicio_conjunto_r1++) {
        for (int posicao_r2 = 1; s_temp.get_n_rotas() > pos_rota_2 && posicao_r2 < (s_temp.get_rota(pos_rota_2).get_tamanho() - 1); posicao_r2++) {
            try {
                s_temp = _perturba_realocacao_conjuntos(s_temp, pos_rota_1, pos_rota_2, inicio_conjunto_r1, posicao_r2, tamanho);
            } catch (int e) {
                s_temp = s;
                continue;
            }

            if (s_temp.get_custo() < s.get_custo()) {
                if (tabu != nullptr) {
                    vector<int> lista_clientes;
                    for (int i = inicio_conjunto_r1; i < inicio_conjunto_r1 + tamanho; i++)
                        lista_clientes.push_back(s.get_rota(pos_rota_1).clientes.at(i).id);
                    Movimento mov = std::make_tuple(pos_rota_1, lista_clientes);

                    if (tabu->is_tabu(mov)) {
                        s_temp = s;
                        continue;
                    } else {
                        tabu->adiciona(mov);
                    }
                }

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
    for (unsigned int i = 0; i < movimentos.size(); i++) {
        
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

Solucao busca_local_aleatoria(Solucao &s, int n_iter, ListaTabu* tabu) {
    Solucao s_best = s;
    Solucao s_temp = s;

    for(int i = 0; i < n_iter; i++) {
        vector<int> movimentos_disponiveis = {0, 1, 2, 3, 4, 5, 6};

        while(movimentos_disponiveis.size()) {
            int id_sorteado = movimentos_disponiveis.at(rand() % movimentos_disponiveis.size());
            try {
                switch(id_sorteado) {
                    case 0:
                        s_temp = movimento_intra_realoacao(s_best, ((rand() % 2) + 1), tabu);
                    case 1:
                        s_temp = movimento_or_opt(s_best, tabu);
                    case 2:
                        s_temp = movimento_2_opt(s_best, tabu);
                    case 3:
                        s_temp = movimento_mix_intra(s_best, tabu);
                    case 4:
                        s_temp = movimento_corte_cruzado(s_best, tabu);
                    case 5:
                        s_temp = movimento_troca_conjuntos(s_best, tabu);
                    case 6:
                        s_temp = movimento_realocacao_conjuntos(s_best, tabu);
                }
                break;
            } catch (int e) {
                movimentos_disponiveis.erase(find(movimentos_disponiveis.begin(), movimentos_disponiveis.end(), id_sorteado));
            }
        }

        if (s_temp.get_custo() < s_best.get_custo())
            s_best = s_temp;
    }
    return s_best;
}