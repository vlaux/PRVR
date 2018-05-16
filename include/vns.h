#ifndef VNS
#define VNS

#include "solucao.h"
#include "utils.h"
#include "lista_tabu.h"

using namespace std;

class Vns {
    private:
        ListaTabu* tabu = nullptr;
        Solucao busca_local(Solucao s, Instancia ins, char* tipo_busca, char* argv[]);         

    public:
        Vns();
        Vns(ListaTabu* tabu);
        Solucao executa(Solucao s, Instancia& ins, char* argv[], int k_max = 1);
};

#endif