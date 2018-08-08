#ifndef VND
#define VND

#include "solucao.h"
#include "utils.h"
#include "lista_tabu.h"

using namespace std;

class Vnd {
    private:
        ListaTabu* tabu = nullptr;
        Solucao busca_local(Solucao &s, int l);

    public:
        Vnd();
        Vnd(ListaTabu* tabu);
        Solucao executa(Solucao &s);
};

#endif