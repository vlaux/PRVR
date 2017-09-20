#include "heuristicas.h"
#include "vnd.h"
#include "busca_local.h" // remover

Solucao* grasp(Instancia* ins) {
    Solucao* s_best = new Solucao(ins);

    int iter = 0, iter_sem_melhora = 0;
    while (iter_sem_melhora < 1) {
        cout << "iteração " << iter << endl;
        
        Solucao* s = new Solucao(ins);
        s->cria_solucao(ins->get_clientes(), ins->get_mapa_rotulos(), ins->get_capacidade());
        s->imprime();
        
        int k = 6; // tem que vir via param de configuração
        s = movimento_perturbacao_cortes(s, ins->get_capacidade(), ins->get_mapa_rotulos());
        // s = aplica_vnd(s, ins, k);

        // transformar em função
        if (s->get_custo() < s_best->get_custo()) {
            delete s_best;
	        s_best = s;
            iter_sem_melhora = 0;
        } else {
            iter_sem_melhora++;
	        delete s;
        }

        iter++;
    }

    s_best->imprime();

    return s_best;

    // polêmico isso aqui... múltiplas construções de uma vez? 
    // TEORICAMENTE, seria melhor construir uma sol e aplicar logo uma BL (seguindo princípio GRASP)
    // pra que ela seja realmente multi-start.
    // for (int i = 0; i<10000; i++)
    // {
    //     Solucao s(ins.get_n_rotulos(), ins.get_n_clientes());
    //     s.cria_solucao(clientes, rotulos, capacidade);
    //     if (s.get_custo() < s_best.get_custo())
    //     {
    //         s_best = s;
    //         cout << "Melhor solução construída na iteração " << i << ". Custo: " << s_best.get_custo() << endl;    
    //     }
    // }

    
    // int iter_max = n_clientes * n_rotulos;
    // int iter_max = 100000;
    // int iter = 0;
    // long double iter_best = -1;
}
