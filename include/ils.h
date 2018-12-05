#ifndef ILS
#define ILS

#include "solucao.h"
#include "cliente.h"
#include "instancia.h"
#include "utils.h"
#include "busca_local.h"

using namespace std;

class Ils : public BuscaLocal
{
    private:
        int max_iter;
        BuscaLocal* bl;
        Solucao perturbacao(Solucao &s);
    public:
        Ils(int max_iter, BuscaLocal* bl);
        Solucao executa(Solucao &s, ListaTabu* tabu = nullptr);
};

#endif