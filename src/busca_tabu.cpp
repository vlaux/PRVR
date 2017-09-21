#include <iostream>
#include "busca_tabu.h"
#include "instancia.h"
#include "grasp.h"
#include "lista_tabu.h"
#include "vnd.h"

using namespace std;

Solucao BuscaTabu::executa(Instancia &ins)
{
    Solucao s(ins);
    s = Grasp(false).constroi_solucao(ins, 1);

    int tam_max_lista = ins.get_n_clientes() * 2;
    ListaTabu* lista_tabu = new ListaTabu(tam_max_lista);

    Vnd vnd(lista_tabu);
    int k_max = 5;
    vnd.executa(s, ins, k_max);

    delete lista_tabu;

    return s;
}