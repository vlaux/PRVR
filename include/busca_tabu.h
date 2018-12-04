#ifndef BUSCA_TABU
#define BUSCA_TABU

#include "solucao.h"
#include "cliente.h"
#include "instancia.h"
// #include "utils.h"
#include "lista_tabu.h"
#include "busca_local.h"

using namespace std;


class BuscaTabu : public BuscaLocal
{
    private:
        ListaTabu* lista_tabu;
        BuscaLocal* busca_local;
        int tamanho_lista = 100;
        int iter_until_update = 10;
        int max_iter_sem_melhora = 100;
        void avalia_tamanho_lista_tabu(Solucao s, int iter);
    public:
        BuscaTabu(int max_iter_sem_melhora, int tamanho_lista, BuscaLocal* busca_local);
        Solucao executa(Solucao &sol_inicial, ListaTabu* _ = nullptr);
};

#endif