#include <assert.h>
#include <stdlib.h>
#include "grasp.h"
#include "vnd.h"
#include "vns.h"
#include "construtor.h"
#include "utils.h"

Grasp::Grasp(bool is_reativo, int n_iter, float alpha, BuscaLocal* bl)
{
    counts = vector<int>(alphas.size());
    scores = vector<double>(alphas.size());
    probs = vector<double>(alphas.size());

    this->alpha = alpha;
    this->n_iter = n_iter;
    this->iter_until_update = 10;
    this->busca_local = bl;
    this->is_reativo = is_reativo;

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

        #ifdef DEBUG
        cout << "iteração " << iter << "  usando alpha = " << alpha << endl;
        #endif

        Solucao s(ins);
        s = Construtor(ins).construcao_gulosa_aleatoria(alpha);

        #ifdef DEBUG
        s.imprime();
        #endif

        if (is_reativo) avalia_alpha(s, s_best, alpha_idx, iter);

        s = busca_local->executa(s);

        if (!iter || s.get_custo() < s_best.get_custo()) {
	        s_best = s;
            cout << "Ĩteração: " << iter << endl;
            s_best.imprime();
            iter_sem_melhora = 0;
        } else {
            iter_sem_melhora++;
        }

        iter++;
    }

    #ifdef DEBUG
    cout << "finalizando grasp" << endl;
    s_best.imprime();
    #endif

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

            #ifdef DEBUG
            cout << "Probabilidades atualizadas de escolha para alfa: " << endl;;

            for (size_t i = 0; i < alphas.size(); ++i)
                cout << alphas[i] << " = " << probs[i] << endl;
            #endif
        }
    }
}