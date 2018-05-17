#ifndef VNS
#define VNS

#include "solucao.h"
#include "utils.h"
#include "lista_tabu.h"

using namespace std;

class Vns {
    private:
        ListaTabu* tabu = nullptr;
        char *tipo_busca_local;
        int k_max_bl;
        Solucao busca_local(Solucao s, Instancia ins);

    public:
        Vns();
        Vns(char* tipo_busca, int k_max_bl, ListaTabu* tabu = nullptr);
        Solucao executa(Solucao s, Instancia& ins, int k_max = 1);
};

#endif