#include <iostream>
#include <list>
#include <tuple>
#include <algorithm>
#include <cmath>
#include "lista_tabu.h"

using namespace std;

#define TAXA_AUMENTO 0.1
#define TAXA_DECRESCIMO 0.1

void ListaTabu::adiciona(const Movimento movimento)
{
    if (lista_tabu.size() == tamanho)
        lista_tabu.pop_front();
    
    lista_tabu.push_back(movimento);
}

bool ListaTabu::is_tabu(const Movimento movimento)
{
    return std::find(lista_tabu.begin(), lista_tabu.end(), movimento) != lista_tabu.end();
}

void ListaTabu::aumenta_lista()
{
    tamanho = (int) min(tamanho_maximo, (int) (tamanho + (tamanho * TAXA_AUMENTO)));
}

void ListaTabu::diminui_lista()
{
    tamanho = (int) max(tamanho_minimo, (int) (tamanho - (tamanho * TAXA_DECRESCIMO)));
    while (lista_tabu.size() > tamanho)
        lista_tabu.pop_front();
}

ListaTabu::ListaTabu(int tamanho)
{
    ListaTabu::tamanho_maximo = (int) std::ceil(tamanho * 2);
    ListaTabu::tamanho_minimo = (int) std::ceil(tamanho / 2);
    ListaTabu::tamanho = tamanho;
}
