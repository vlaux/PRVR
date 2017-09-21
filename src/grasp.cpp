#include <assert.h>
#include "grasp.h"
#include "vnd.h"

Grasp::Grasp(bool is_reativo, float alpha) 
{
    counts = vector<int>(alphas.size());
    scores = vector<double>(alphas.size());
    probs = vector<double>(alphas.size());

    this->alpha = alpha;

    if (is_reativo) {
        fill(counts.begin(), counts.end(), 0);        
        fill(scores.begin(), scores.end(), 0);        
        fill(probs.begin(), probs.end(), (1.0 / (double) alphas.size()));
    }
}

Solucao Grasp::executa(Instancia &ins) {
    Solucao s_best(ins);

    int iter = 0, iter_sem_melhora = 0;
    while (iter_sem_melhora < 1) {
        
        int alpha_idx = -1;
        if (is_reativo) {
            alpha_idx = utils::random::select_index_randomly(probs);
            alpha = alphas[alpha_idx];
        }

        cout << "iteração " << iter << "  usando alpha = " << alpha << endl;
        
        Solucao s(ins);
        s = constroi_solucao(ins, alpha);
        s.imprime();

        avalia_alpha(s, s_best, alpha_idx, iter);
        
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
}

void Grasp::avalia_alpha(Solucao s, Solucao s_best, float alpha_idx, int iter)
{
    if (is_reativo) {
        counts[alpha_idx]++;
        scores[alpha_idx]+= s.get_custo();

        if (iter % iter_until_update == 0) {
            vector<double> avgs(alphas.size());

            for (size_t bk = 0; bk < alphas.size(); ++bk)
                avgs[bk] = counts[bk] > 0 ? scores[bk] / counts[bk] : INFINITY;

            vector<double> Q(alphas.size());

            for (size_t bk = 0; bk < alphas.size(); ++bk)
                Q[bk] = s_best.get_custo()/avgs[bk];

            double q = utils::lists::sum(Q, 0.0);

            for (size_t bk = 0; bk < alphas.size(); ++bk)
                probs[bk] = Q[bk]/q;

            cout << "Probabilidades atualizadas de escolha para alfa: " << endl;;

            for (size_t i = 0; i < alphas.size(); ++i)
                cout << alphas[i] << " = " << probs[i] << endl;
        }
    }
}

Solucao Grasp::constroi_solucao(Instancia &ins, float alpha)
{
    Solucao s(ins);

    Cliente deposito = ins.get_cliente(0);
    
    while(s.existe_cliente_nao_atendido()) {
        Rota r(deposito);
        
        Cliente c = escolhe_melhor_cliente(s, ins, r.clientes.back(), alpha);
        while (c.id != r.clientes.back().id && r.get_carga() + c.demanda <= ins.get_capacidade() * LIMITE_CONSTRUCAO) {
            s.adiciona_cliente(c, r, ins.get_mapa_rotulos());
            c = escolhe_melhor_cliente(s, ins, r.clientes.back(), alpha);
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