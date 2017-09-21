#include "vnd.h"
#include <iostream>
#include "solucao.h"
#include "instancia.h"
#include "busca_local.h"


Vnd::Vnd() {};

Vnd::Vnd(ListaTabu* lista_tabu)
{
    tabu = lista_tabu;
}

Solucao Vnd::executa(Solucao s, Instancia& ins, int k_max)
{
    int k = 1;

    Solucao s_temp = s;
    Solucao s_best = s;
    
    while (k < k_max)
    {
        cout << "K = " << k << endl;

        s_temp = busca_melhor_vizinho(s_best, ins, k);
        
        if (s_temp.get_custo() < s_best.get_custo())
        {
            cout << "ENCONTROU. K=1" << endl;
            s_best = s_temp;
            k = 1;
        }
        else 
        {
            k++;
            cout << "NÃO ENCONTROU. K = " << k << endl;
        }
    }

    return s_best;
}

Solucao Vnd::busca_melhor_vizinho(Solucao s, Instancia& ins, int k)
{
    cout << "Procurando melhor vizinho em k = " << k << endl;

    // int max_busca_em_k = ins.get_n_rotulos(); // bom valor?
    int max_busca_em_k = 1; // bom valor?    
    Solucao s_temp = s;
    Solucao s_best = s;

    for (int i = 0; i < max_busca_em_k; i++)
    {
        switch (rand() % N_MOVIMENTOS)
        {
        case 0:
            s_temp = movimento_intra_rota(s_temp, ins.get_mapa_rotulos(), k, tabu);
            break;
        case 1:
            s_temp = movimento_intra_rota_n_rotas(s_temp, ins.get_mapa_rotulos(), k);
            break;
        case 2:
            s_temp = movimento_inter_move_n(s_temp, ins.get_capacidade(), ins.get_mapa_rotulos(), k);
            break;
        case 3:
            s_temp = movimento_intra_2_opt(s_temp, ins.get_mapa_rotulos(), k);
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
