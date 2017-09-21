#include <iostream>
#include "busca_tabu.h"
#include "instancia.h"
#include "grasp.h"
#include "lista_tabu.h"
#include "vnd.h"

using namespace std;

BuscaTabu::BuscaTabu()
{
    lista_tabu = new ListaTabu(tam_maximo_lista);
}

Solucao BuscaTabu::executa(Instancia &ins)
{
    Solucao s_best(ins);
    Solucao s(ins);
    s = Grasp(false).constroi_solucao(ins, 1);

    Vnd vnd(lista_tabu);
    int k_max = 5;

    int iter = 0, iter_sem_melhora = ins.get_n_clientes();
    while(iter < iter_sem_melhora)
    {
        s = vnd.executa(s, ins, k_max);
        if (s.get_custo() < s_best.get_custo()) 
        {
            s_best = s;
            iter_sem_melhora = 0;
        }
        else
        {
            s = s_best;
        }

        avalia_tamanho_lista_tabu(s_best, iter);

        iter++;
    }

    delete lista_tabu;

    return s;
}

void BuscaTabu::avalia_tamanho_lista_tabu(Solucao s_best, int iter)
{
    if (iter % iter_until_update == 0) 
    {
        if (s_best.get_custo() < ultimo_custo_avaliado)
            lista_tabu->diminui_lista();

        else
            lista_tabu->aumenta_lista();
    }
}
