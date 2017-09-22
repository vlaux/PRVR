#include "ils.h"
#include "vnd.h"
#include "grasp.h"
#include "busca_local.h"

Solucao Ils::executa(Instancia &ins)
{
    Solucao sol_inicial(ins);
    sol_inicial = Grasp(false).constroi_solucao(ins);
    
    Solucao s_best(ins);
    s_best = Vnd().executa(sol_inicial, ins);

    int iter = 0, iter_sem_melhora = 0, max_iter = ins.get_n_rotulos(); // valor melhor?

    while(iter_sem_melhora < max_iter)
    {
        Solucao s_temp = movimento_perturbacao_cortes(s_best, ins.get_capacidade(), ins.get_mapa_rotulos());

        int k_max = 5; // definir via algum param
        s_temp = Vnd().executa(s_temp, ins, k_max);

        if (s_temp.get_custo() < s_best.get_custo())
        {
            iter_sem_melhora = 0;
            s_best = s_temp;
        }

        iter++;
    }

    return s_best;
}