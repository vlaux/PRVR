#ifndef GRASP
#define GRASP

#include "solucao.h"
#include "busca_local.h"
#include "cliente.h"
#include "instancia.h"
#include "utils.h"

using namespace std;

class Grasp 
{
    private:
        BuscaLocal* busca_local;
        bool is_reativo = false;
        float alpha = 1;
        int iter_until_update;
        int n_iter = 1;
        vector<float> alphas = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
        vector<int> counts;
        vector<double> scores;
        vector<double> probs;
        void avalia_alpha(Solucao s, Solucao s_best, float alpha_idx, int iter);
    public:
        Grasp(bool is_reativo, int n_iter, float alpha, BuscaLocal* bl);
        Solucao executa(Instancia *ins);
};

#endif