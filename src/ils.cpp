#include "ils.h"
#include "vnd.h"

Solucao Ils::executa(Solucao &s, int max_iter)
{   
    Solucao s_best = s;
    Solucao s_temp = s;

    s_temp = busca_local_aleatoria(s_temp, 100);

    int iter_sem_melhora = 0;

    while(iter_sem_melhora < max_iter)
    {
        s_temp = movimento_mix_inter(s);

        s_temp = busca_local_aleatoria(s_temp, 100); // talvez testar com VNS?

        if (s_temp.get_custo_ponderado() < s_best.get_custo_ponderado())
        {
            iter_sem_melhora = 0;
            s_best = s_temp;
        }

        iter_sem_melhora++;
    }

    return s_best;
}