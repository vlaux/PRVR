#ifndef BUSCA_TABU
#define BUSCA_TABU

#include "solucao.h"
#include "cliente.h"
#include "instancia.h"
#include "utils.h"
#include "lista_tabu.h"

using namespace std;


class BuscaTabu
{
    private:
        ListaTabu* lista_tabu;
        int tam_maximo_lista = 100; // receber via param; tem que ser dinâmico!
        int ultimo_custo_avaliado = INT32_MAX;
        int iter_until_update = 20; //receber via param de conf
        void avalia_tamanho_lista_tabu(Solucao s_best, int iter);
    public:
        BuscaTabu();
        Solucao executa(Instancia &ins);
};

#endif