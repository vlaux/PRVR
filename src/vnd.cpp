#include "vnd.h"
#include <iostream>
#include "solucao.h"
#include "busca_local.h"

Solucao aplica_vnd(const Solucao &s, int n_clientes, int capacidade, int **mapa_rotulos, int k_max)
{
    int k = 1;

    Solucao s_temp = s;
    Solucao s_best = s;
    
    while (k < k_max)
    {
        cout << "K = " << k << endl;

        s_temp = encontra_melhor_solucao_vizinhanca(s_best, n_clientes, capacidade, mapa_rotulos, k);
        
        if (s_temp.get_custo() < s_best.get_custo())
        {
            cout << "ENCONTROU. K=1" << endl;
            s_best = s_temp;
            k = 1;
        }
        else 
        {
            s_temp = s_best;
            k++;
            cout << "NÃO ENCONTROU. K = " << k << endl;
        }
    }

    return s_best;
}

Solucao encontra_melhor_solucao_vizinhanca(const Solucao &s, int n_clientes, int capacidade, int **mapa_rotulos, int k)
{
    cout << "Procurando melhor vizinho em k = " << k << endl;

    int max_busca_em_k = 1000000;
    Solucao s_temp = s;
    Solucao s_best = s;

    for (int i = 0; i < max_busca_em_k; i++)
    {
        switch (rand() % N_MOVIMENTOS)
        {
        case 0:
            s_temp = movimento_intra_rota(s_temp, mapa_rotulos, k);
            break;
        case 1:
            s_temp = movimento_intra_rota_n_rotas(s_temp, mapa_rotulos, k);
            break;
        case 2:
            s_temp = movimento_inter_move_n(s_temp, capacidade, mapa_rotulos, k);
            break;
        case 3:
            s_temp = movimento_intra_2_opt(s_temp, mapa_rotulos, k);
            break;
        default:
            cerr << "Que movimento é esse???" << endl;
            exit(EXIT_FAILURE);
        }
        if (s_temp.get_custo() < s_best.get_custo())
        {
            s_best = s_temp;
            cout << "Encontrou melhora em k " << k << endl;
        }
        else
        {
            s_temp = s_best;
            i++;
        }
    }

    cout << "Encerrando busca em k=" << k << endl;

    return s_best;
}
