#ifndef GRASP
#define GRASP

#include "solucao.h"
#include "cliente.h"
#include "instancia.h"
#include "utils.h"

using namespace std;

#define LIMITE_CONSTRUCAO 0.8

class Grasp 
{
    private:
        bool is_reativo;
        float alpha;
        int iter_until_update = 20; // receber via param
        vector<float> alphas = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
        vector<int> counts;
        vector<double> scores;
        vector<double> probs;

        void avalia_alpha(Solucao s, Solucao s_best, float alpha_idx, int iter);        
        Cliente escolhe_melhor_cliente(Solucao s, Instancia &ins, Cliente origem, float alpha);
    public:
        Grasp(bool is_reativo = false, float alpha = 1);        
        Solucao constroi_solucao(Instancia &ins, float alpha = 1);
        Solucao executa(Instancia &ins);
};

#endif