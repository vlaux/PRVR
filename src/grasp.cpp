#include <assert.h>
#include "grasp.h"
#include "vnd.h"

Solucao Grasp::executa(Instancia &ins) {
    Solucao s_best(ins);

    int iter = 0, iter_sem_melhora = 0;
    while (iter_sem_melhora < 1) {
        cout << "iteração " << iter << endl;
        
        Solucao s(ins);
        s = constroi_solucao(ins);
        s.imprime();
        
        int k = 6; // tem que vir via param de configuração
        s = aplica_vnd(s, ins, k);

        // transformar em função
        if (s.get_custo() < s_best.get_custo()) {
	        s_best = s;
            iter_sem_melhora = 0;
        } else {
            iter_sem_melhora++;
        }

        iter++;
    }

    s_best.imprime();

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

Solucao Grasp::constroi_solucao(Instancia &ins)
{
    Solucao s(ins);

    Cliente deposito = ins.get_cliente(0);
    
    while(s.existe_cliente_nao_atendido()) {
        Rota r(deposito);
        
        Cliente c = escolhe_melhor_cliente(s, ins, r.clientes.back(), 1);
        while (c.id != r.clientes.back().id && r.get_carga() + c.demanda <= ins.get_capacidade() * LIMITE_CONSTRUCAO) {
            s.adiciona_cliente(c, r, ins.get_mapa_rotulos());
            c = escolhe_melhor_cliente(s, ins, r.clientes.back(), 1);
        }

        s.adiciona_cliente(deposito, r, ins.get_mapa_rotulos());
        s.adiciona_rota(r);
    }

    return s;
}

Cliente Grasp::escolhe_melhor_cliente(Solucao s, Instancia &ins, Cliente origem, float alpha)
{
    if (!s.existe_cliente_nao_atendido())
        return origem;

    int max = -1, min = INT32_MAX;
    int n_clientes = ins.get_n_clientes();
    vector<int> avaliacao(n_clientes + 1);

    for(int i = 1; i <= n_clientes; i++)
    {
        if(i != origem.id && !s.is_cliente_visitado(i))
        {
            avaliacao[i] = s.get_rotulo(ins.get_rotulo(origem.id, i)).vezes_utilizado;
            
            if (avaliacao[i] > max) max = avaliacao[i];
            if (avaliacao[i] < min) min = avaliacao[i];
        }
        else 
            avaliacao[i] = -1;
    }

    vector<Cliente> lrc;

    for(int i = 1; i <= n_clientes; i++)
    {
        if (avaliacao[i] >= (min + alpha*(max-min))) 
        {
            Cliente c = ins.get_cliente(i);        
            lrc.push_back(c);            
        }
    }

    assert(!lrc.empty());
    
    return lrc[rand()%lrc.size()];
}