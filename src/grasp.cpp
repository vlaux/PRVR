#include <assert.h>
#include <stdlib.h>
#include "grasp.h"
#include "vnd.h"
#include "vns.h"
#include "construtor.h"

Grasp::Grasp(bool is_reativo, int n_iter, float alpha)
{
    counts = vector<int>(alphas.size());
    scores = vector<double>(alphas.size());
    probs = vector<double>(alphas.size());

    this->alpha = alpha;
    this->n_iter = n_iter;
    this->iter_until_update = 10;

    if (is_reativo) {
        counts = vector<int>(alphas.size());
        scores = vector<double>(alphas.size());
        probs = vector<double>(alphas.size());

        fill(counts.begin(), counts.end(), 0);        
        fill(scores.begin(), scores.end(), 0);        
        fill(probs.begin(), probs.end(), (1.0 / (double) alphas.size()));
    }
}

Solucao Grasp::executa(Instancia *ins) {
    Solucao s_best(ins);

    int iter = 0, iter_sem_melhora = 0, max_iter_grasp = this->n_iter;
    while (iter_sem_melhora < max_iter_grasp) {        
        int alpha_idx = -1;
        if (is_reativo) {
            alpha_idx = utils::random::select_index_randomly(probs);
            alpha = alphas[alpha_idx];
        }

        cout << "iteração " << iter << "  usando alpha = " << alpha << endl;
        
        Solucao s(ins);
        s = Construtor(ins).construcao_gulosa_aleatoria(alpha);
        #ifdef DEBUG
        s.imprime();
        #endif

        avalia_alpha(s, s_best, alpha_idx, iter);

        #ifdef VNS
        s = Vns().executa(s);
        #endif

        if (!iter || s.get_custo_ponderado() < s_best.get_custo_ponderado()) {
	        s_best = s;
            s_best.imprime();
            iter_sem_melhora = 0;
        } else {
            iter_sem_melhora++;
        }

        iter++;
    }

    cout << "finalizando grasp" << endl;
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