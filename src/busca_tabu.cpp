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

Solucao BuscaTabu::executa(Instancia &ins, Solucao sol_inicial, int max_iter, char* argv[])
{
    Solucao s_best(ins);
    Solucao s(ins);
    s = sol_inicial;

    int iter_sem_melhora = 0, iter = 0;
    while(iter_sem_melhora < max_iter)
    {
        // s = vnd.executa(s, ins, k_max);
        char* tipo_busca = argv[4];
        s = busca_local(s, ins, tipo_busca, argv);

        if (s.get_custo() < s_best.get_custo()) 
        {
            s_best = s;
            iter_sem_melhora = 0;
        }
        else
        {
            s = s_best;
            iter_sem_melhora++;
        }

        iter++;
        avalia_tamanho_lista_tabu(s_best, iter);
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

Solucao BuscaTabu::busca_local(Solucao s, Instancia ins, char *tipo_busca, char* argv[]) {
    if (strcmp(tipo_busca, "VND") == 0) {
        int k_max = atoi(argv[5]);
        return Vnd(lista_tabu).executa(s, ins, k_max);
    }
    else if (strcmp(tipo_busca, "VNS") == 0) {
        cout << "vns para tabu não implementado" << endl;
        abort();
    }
    else {
        cout << "deveria executar movs aleatórios no ILS: não implementado yet" << endl;
        abort();
    }
    return s;
}