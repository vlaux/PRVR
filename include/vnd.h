#ifndef VND
#define VND

#include "solucao.h"
#include "utils.h"
#include "lista_tabu.h"

using namespace std;

class Vnd {
    private:
        ListaTabu* tabu = nullptr;
        Solucao busca_melhor_vizinho(Solucao s, Instancia& ins, int k);        

    public:
        Vnd();
        Vnd(ListaTabu* tabu);
        Solucao executa(Solucao s, Instancia& ins, int k_max);
};

#endif