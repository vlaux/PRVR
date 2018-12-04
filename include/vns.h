#ifndef VNS
#define VNS

#include "solucao.h"
#include "utils.h"
#include "lista_tabu.h"
#include "busca_local.h"

using namespace std;

class Vns: public BuscaLocal {
    private:
        Solucao busca_local(Solucao &s);
        Solucao perturbacao(Solucao &s, int k);
        int max_iter = 1;
    public:
        Vns();
        Vns(int max_iter);
        Solucao executa(Solucao &s, ListaTabu* lista_tabu = nullptr);
};

#endif