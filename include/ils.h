#ifndef ILS
#define ILS

#include "solucao.h"
#include "cliente.h"
#include "instancia.h"
#include "utils.h"

using namespace std;

class Ils
{
    private:
        
    public:
        Solucao executa(Instancia &ins);
};

#endif