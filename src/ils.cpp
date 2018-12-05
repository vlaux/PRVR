#include "ils.h"
#include "vnd.h"


Ils::Ils(int max_iter, BuscaLocal* bl)
{
    Ils::max_iter = max_iter;
    Ils::bl = bl;
}

Solucao Ils::executa(Solucao &s, ListaTabu* tabu)
{
    Solucao s_best = s;
    Solucao s_temp = s;

    s_temp = bl->executa(s_temp, tabu);

    int iter_sem_melhora = 0;

    while (iter_sem_melhora < max_iter)
    {
        s_temp = perturbacao(s_temp);

        s_temp = bl->executa(s_temp, tabu);

        if (s_temp.get_custo_ponderado() < s_best.get_custo_ponderado())
        {
            iter_sem_melhora = 0;
            s_best = s_temp;
        }
        else 
        {
            s_temp = s_best;
            iter_sem_melhora++;
        }

    }

    return s_best;
}

Solucao Ils::perturbacao(Solucao &s)
{
    int opcao = rand() % 3;

    switch (opcao)
    {
    case 0:
        return perturbacao_troca_conjuntos(s);
    case 1:
        return perturbacao_realocacao_conjuntos(s);
    case 2:
        return perturbacao_corte_cruzado(s);
    default:
        return s;
    }
}