#ifndef LISTA_TABU
#define LISTA_TABU

#include <list>
#include <tuple>
#include "utils.h"

using namespace std;

class ListaTabu {
    private:
        std::list<Movimento> lista_tabu;
        int tamanho_maximo;
        int tamanho_minimo;
        int tamanho;
    public:
        ListaTabu(int tamanho);
        void adiciona(Movimento movimento);
        bool is_tabu(Movimento movimento);
        void aumenta_lista();
        void diminui_lista();
};

#endif
