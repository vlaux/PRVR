#include <iostream>
#include <list>
#include <tuple>
#include <algorithm>
#include "lista_tabu.h"

using namespace std;

void ListaTabu::adiciona(const Movimento movimento)
{
    // cout << "Tamanho: " << lista_tabu.size() << "/" << tamanho << endl;
    if (lista_tabu.size() == tamanho)
        lista_tabu.pop_front();
    
    lista_tabu.push_back(movimento);
}

bool ListaTabu::is_tabu(const Movimento movimento)
{
    return (std::find(lista_tabu.begin(), lista_tabu.end(), movimento) != lista_tabu.end());
}

// TODO melhorar isso
void ListaTabu::aumenta_lista()
{
    tamanho = tamanho_maximo;
}

void ListaTabu::diminui_lista()
{
    tamanho = tamanho_maximo / 2;
}

ListaTabu::ListaTabu(int tamanho_maximo)
{
    ListaTabu::tamanho_maximo = tamanho_maximo;
    ListaTabu::tamanho = tamanho_maximo / 2;
}