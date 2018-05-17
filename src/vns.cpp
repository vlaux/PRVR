#include "vns.h"
#include <iostream>
#include "solucao.h"
#include "instancia.h"
#include "busca_local.h"
#include "vnd.h"

Vns::Vns() {};

Vns::Vns(char* tipo_busca, int k_max_bl, ListaTabu* lista_tabu)
{
    this->tabu = lista_tabu;
    this->tipo_busca_local = tipo_busca;
    this->k_max_bl = k_max_bl;
}

Solucao Vns::executa(Solucao s, Instancia& ins, int k_max)
{
    int k = 1;

    Solucao s_temp = s;
    Solucao s_best = s;
    
    while (k <= k_max)
    {
        #ifdef DEBUG
        cout << "K = " << k << endl;
        #endif

        s_temp = movimento_perturbacao_cortes(s_best, ins.get_capacidade(), ins.get_mapa_rotulos());
        s_temp = busca_local(s_temp, ins);
        
        if (s_temp.get_custo() < s_best.get_custo())
        {
            #ifdef DEBUG
            cout << "ENCONTROU. K=1" << endl;
            #endif
            s_best = s_temp;
            k = 1;
        }
        else 
        {
            k++;
            #ifdef DEBUG
            cout << "NÃO ENCONTROU. K = " << k << endl;
            #endif
        }
    }

    return s_best;
}

Solucao Vns::busca_local(Solucao s, Instancia ins) {
    if (strcmp(tipo_busca_local, "VND") == 0) {
        return Vnd().executa(s, ins, k_max_bl);
    }
    else if (strcmp(tipo_busca_local, "TABU") == 0) {
        cout << "tabu para ILS ainda não suportado" << endl;
        abort();
    }
    else {
        cout << "deveria executar movs aleatórios no ILS: não implementado yet" << endl;
        abort();
    }
    return s;
}