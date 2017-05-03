#ifndef TABU
#define TABU

#include <list>
#include <tuple>

using namespace std;

class ListaTabu {
    private:
        std::list<std::tuple<int, int, int> > lista_tabu;
        int tamanho_maximo;
        int tamanho;
    public:
        ListaTabu(int tamanho_maximo);
        void adiciona(std::tuple<int, int, int> movimento);
        bool is_tabu(std::tuple<int, int, int> movimento);
        void aumenta_lista();
        void diminui_lista();
};

#endif
