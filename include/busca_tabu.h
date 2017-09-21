#ifndef BUSCA_TABU
#define BUSCA_TABU

#include "solucao.h"
#include "cliente.h"
#include "instancia.h"
#include "utils.h"

using namespace std;


class BuscaTabu
{
    public:
        Solucao executa(Instancia &ins);
};

#endif