#include "vnd.h"
#include <iostream>
#include <cstdlib>
#include "solucao.h"
#include "instancia.h"
#include "busca_local.h"

#define L_MAX 4

Vnd::Vnd() { };

Solucao Vnd::executa(Solucao &s, ListaTabu* tabu)
{
    int l = 1;

    Solucao s_temp = s;
    Solucao s_best = s;
    
    while (l <= L_MAX)
    {
        #ifdef DEBUG
        cout << "[VND] l = " << l << endl;
        #endif

        try {
            s_temp = busca_local(s_best, l, tabu);
        } catch (int e) {
            l++;
            continue;
        }
        
        if (s_temp.get_custo() < s_best.get_custo())
        {
            #ifdef DEBUG
            cout << "[VND] ENCONTROU. l=1" << endl;
            #endif
            s_best = s_temp;
            l = 1;
        }
        else 
        {
            l++;
            #ifdef DEBUG
            cout << "[VND] NÃO ENCONTROU. l = " << k << endl;
            #endif
        }
    }

    return s_best;
}

Solucao Vnd::busca_local(Solucao &s, int l, ListaTabu* tabu) {
    switch(l) {
        case 1: {
            int tam = (rand() % 2) + 1;
            return movimento_intra_realoacao(s, tam, tabu);
        }
        case 2: 
            return movimento_2_opt(s, tabu);
        case 3:
            return movimento_or_opt(s, tabu);
        case 4:
            return movimento_mix_intra(s, tabu);
        default:
            return s;
    }
}