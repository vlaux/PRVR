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
        // vector<float>   alphas = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
        // vector<int>     counts(alphas.size());
        // vector<double>  scores(alphas.size());
        // vector<double>   probs(alphas.size());

        Solucao constroi_solucao(Instancia &ins);
        Cliente escolhe_melhor_cliente(Solucao s, Instancia &ins, Cliente origem, float alpha);
    public:
        Solucao executa(Instancia &ins);
};

#endif