#ifndef VND
#define VND

#include "solucao.h"
#include "utils.h"
#include "lista_tabu.h"
#include "busca_local.h"

using namespace std;

class Vnd : public BuscaLocal {
    private:
        Solucao busca_local(Solucao &s, int l, ListaTabu* tabu);
    public:
        Vnd();
        Solucao executa(Solucao &s, ListaTabu* tabu = nullptr);
};

#endif