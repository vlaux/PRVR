#include "vns.h"

#include <iostream>
#include <cstdlib>

#include "solucao.h"
#include "instancia.h"
#include "busca_local.h"
#include "vnd.h"

#define K_MAX 4

Vns::Vns() {};

Vns::Vns(int max_iter) {
    Vns::max_iter = max_iter;
};

Solucao Vns::executa(Solucao &s, ListaTabu* lista_tabu)
{
    int k = 1;

    Solucao s_temp = s;
    Solucao s_best = s;

    int iter = 0, iter_sem_melhora = 0, custo = INT32_MAX;

    while(iter_sem_melhora < max_iter)
    {
        k = 1;
        iter++;

        while (k <= K_MAX)
        {
            #ifdef DEBUG
            cout << "k = " << k << endl;
            #endif

            s_temp = perturbacao(s_best, k);
            s_temp = Vnd().executa(s_temp, lista_tabu);
            
            if (s_temp.get_custo_ponderado() < s_best.get_custo_ponderado())
            {
                #ifdef DEBUG
                cout << "ENCONTROU. K=1" << endl;
                #endif

                k = 1;
                s_best = s_temp;
            }
            else 
            {
                #ifdef DEBUG
                cout << "NÃO ENCONTROU. K = " << k << endl;
                #endif

                k++;
            }
        }

        if (s_best.get_custo() < custo) {
            custo = s_best.get_custo();
            if (logger != nullptr) logger->salva_resultado_parcial(iter, custo);
            iter_sem_melhora = 0;
        }
        else iter_sem_melhora++;
    }

    return s_best;
}

Solucao Vns::perturbacao(Solucao &s, int k) {
    switch(k) {
        case 1:
            return perturbacao_troca_conjuntos(s);
        case 2:
            return perturbacao_realocacao_conjuntos(s);
        case 3:
            return perturbacao_corte_cruzado(s);
        case 4: { // mix
            int k_rand = rand() % 4;
            return perturbacao(s, k_rand);
        }
        default:
            return s;
            
    }
}