#ifndef ILS
#define ILS

#include "solucao.h"
#include "cliente.h"
#include "instancia.h"
#include "utils.h"
#include "busca_local.h"

using namespace std;

class Ils
{
    public:
        Solucao executa(Solucao &s, int max_iter);
};

#endif