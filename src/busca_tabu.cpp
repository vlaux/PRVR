#include <iostream>
#include "instancia.h"
#include "lista_tabu.h"
#include "busca_local.h"
#include "busca_tabu.h"

using namespace std;

BuscaTabu::BuscaTabu(int max_iter_sem_melhora, int tamanho_lista, BuscaLocal* busca_local)
{
    this->busca_local = busca_local;
    this->tamanho_lista = tamanho_lista;
    this->max_iter_sem_melhora = max_iter_sem_melhora;
}

Solucao BuscaTabu::executa(Solucao &sol_inicial, ListaTabu* _)
{
    Solucao s_best = sol_inicial;
    Solucao s = sol_inicial;

    lista_tabu = new ListaTabu(tamanho_lista); // vai ser definido por algum param da instância    

    int iter_sem_melhora = 0, iter = 0;
    while(iter_sem_melhora < max_iter_sem_melhora)
    {
        s = busca_local->executa(s, lista_tabu);

        if (s.get_custo() < s_best.get_custo()) 
        {
            s_best = s;
            iter_sem_melhora = 0;
        }
        else
            iter_sem_melhora++;

        avalia_tamanho_lista_tabu(s, iter);
        iter++;
    }

    delete lista_tabu;

    return s_best;
}

void BuscaTabu::avalia_tamanho_lista_tabu(Solucao s, int iter)
{   
    static int ultimo_custo_avaliado;
    if (iter == 0)
        ultimo_custo_avaliado = s.get_custo();

    if (iter % iter_until_update == 0) 
    {
        if (s.get_custo() < ultimo_custo_avaliado)
            lista_tabu->diminui_lista();

        else if (s.get_custo() > ultimo_custo_avaliado)
            lista_tabu->aumenta_lista();
    }

    ultimo_custo_avaliado = s.get_custo();
}