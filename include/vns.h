#ifndef VNS
#define VNS

#include "solucao.h"
#include "utils.h"
#include "lista_tabu.h"

using namespace std;

class Vns {
    private:
        ListaTabu* tabu = nullptr;
        Solucao busca_local(Solucao &s);
        Solucao perturbacao(Solucao &s, int k);

    public:
        Vns();
        Vns(ListaTabu* tabu);
        Solucao executa(Solucao &s);
};

#endif