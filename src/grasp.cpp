#include "heuristicas.h"

Solucao* grasp(Instancia* ins) {
    Solucao* s_best = new Solucao(ins);

    int iter = 0, iter_sem_melhora = 0;
    while (iter_sem_melhora < 500) {
        iter++;
        cout << "iteração " << iter;
        
        Solucao* s = new Solucao(ins);
        s->cria_solucao(ins->get_clientes(), ins->get_mapa_rotulos(), ins->get_capacidade());
        cout << "custo da solucao criada: " << s->get_custo() << endl;
        
        // s = aplica_vnd(s, n_clientes, capacidade, rotulos, 6);

        // transformar em função
        if (s->get_custo() < s_best->get_custo()) {
            s_best = s;
            iter_sem_melhora = 0;
        } else {
            iter_sem_melhora++;
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