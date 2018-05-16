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
        Solucao busca_local(Solucao s, Instancia ins, char* tipo_busca, char* argv[]);
        
    public:
        Solucao executa(Instancia &ins, Solucao sol_inicial, int max_iter, char* argv[]);
};

#endif