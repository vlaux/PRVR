#include <iostream>
#include <tuple>
#include "solucao.h"
#include "rota.h"
#include "tabu.h"

#define MODULO(x) ((x)>=0?(x):-(x))

using namespace std;

Solucao movimento_1(Solucao s, ListaTabu &tabu, int** mapa_rotulos)
{
    cout << "Movimento 1 - Intra rota ";
    int n_rotas = s.get_n_rotas();
    if (n_rotas < 1)
        return s;
    
    int id_rota = rand()%n_rotas;
    Rota *r = &s.rotas[id_rota];

    cout << "Rota " << id_rota;

    int tamanho = r->get_tamanho();

    if (tamanho < 4) // pelo menos dois clientes são necessários: depósito, cliente 1, cliente 2, depósito 
        return s;

    int pos_1 = rand()%(tamanho -2) + 1;
    int pos_2 = pos_1;

    while (pos_2 == pos_1) 
    {
        pos_2 = rand()%(tamanho -2) + 1;
    }

    cout << ", posições " << pos_1 << " e " << pos_2 << endl;

    std::tuple<int, int, int> mov_1 = std::make_tuple(r->clientes[pos_1].id, id_rota, pos_2);
    std::tuple<int, int, int> mov_2 = std::make_tuple(r->clientes[pos_2].id, id_rota, pos_1);

    if (tabu.is_tabu(mov_1) || tabu.is_tabu(mov_2))
    {
        cout << "-- movimento tabu --" << endl;
        return s;
    }
    else 
    {
        tabu.adiciona(mov_1);
        tabu.adiciona(mov_2);        
    }

    //Efetua a troca
    std::swap(r->clientes[pos_1], r->clientes[pos_2]);

    s.recalcula_rotulos_utilizados(mapa_rotulos);

    cout << "Trocado! Custo agora é " << s.get_custo() << endl;

    return s;
}

Solucao movimento_2(Solucao s, int capacidade, ListaTabu &tabu, int** mapa_rotulos)
{
    cout << "Movimento 2 - Realocação - ";
    int n_rotas = s.get_n_rotas();
    if (n_rotas < 2)
        return s;
    
    int id_rota_1 = rand()%n_rotas;
    int id_rota_2 = id_rota_1;

    while (id_rota_1 == id_rota_2)
        id_rota_2 = rand()%n_rotas;

    Rota *rota_1 = &s.rotas[id_rota_1];
    Rota *rota_2 = &s.rotas[id_rota_2];
    
    int posicao_origem = rand()%(rota_1->get_tamanho() - 2) + 1;
    int posicao_destino = rand()%(rota_2->get_tamanho() - 2) + 1;

    Cliente cliente_movido = rota_1->clientes[posicao_origem];
    cout << "cliente " << cliente_movido.id << " na pos " << posicao_origem << " da rota " << id_rota_1 << " para pos " << posicao_destino << " da rota " << id_rota_2;

    if (rota_2->get_carga() + cliente_movido.demanda > capacidade) // capacidade excedida // throw(?)
    {
        cout << "-- capacidade excedida --" << endl;
        return s; 
    }

    rota_2->clientes.insert(rota_2->clientes.begin() + posicao_destino, cliente_movido);
    rota_1->clientes.erase(rota_1->clientes.begin() + posicao_origem);

    s.recalcula_rotulos_utilizados(mapa_rotulos);

    cout << "Realocado! Custo agora é " << s.get_custo() << endl;

    return s;
}

Solucao movimento_3(Solucao s, int capacidade, ListaTabu &tabu, int** mapa_rotulos) 
{
    cout << "Movimento 3: 2-opt. ";
    int n_rotas = s.get_n_rotas();
    if (n_rotas < 2)
        return s;
    
    int id_rota_1 = rand()%n_rotas;
    int id_rota_2 = id_rota_1;

    while (id_rota_1 == id_rota_2)
        id_rota_2 = rand()%n_rotas;

    Rota rota_1 = s.rotas[id_rota_1];
    Rota rota_2 = s.rotas[id_rota_2];
    
    int ponto_corte_rota_1 = rand()%(rota_1.get_tamanho() - 2) + 1;
    int ponto_corte_rota_2 = rand()%(rota_2.get_tamanho() - 2) + 1;

    cout << "Rota " << id_rota_1 << ":ponto de corte " << ponto_corte_rota_1;
    cout << " e Rota " << id_rota_2 << ":ponto de corte " << ponto_corte_rota_2 << endl;

    Rota nova_rota_1;
    Rota nova_rota_2;

    for (std::vector<Cliente>::iterator it = rota_1.clientes.begin(); it != rota_1.clientes.begin() + ponto_corte_rota_1; it++)
        nova_rota_1.clientes.push_back(*it);
    for (std::vector<Cliente>::iterator it = rota_2.clientes.begin() + ponto_corte_rota_2; it != rota_2.clientes.end(); it++)
        nova_rota_1.clientes.push_back(*it);

    if (nova_rota_1.get_carga() > capacidade)
    {
        cout << "-- capacidade excedida r1 --" << endl;
        return s;
    }

    for (std::vector<Cliente>::iterator it = rota_2.clientes.begin(); it != rota_2.clientes.begin() + ponto_corte_rota_2; it++)
        nova_rota_2.clientes.push_back(*it);
    for (std::vector<Cliente>::iterator it = rota_1.clientes.begin() + ponto_corte_rota_1; it != rota_1.clientes.end(); it++)
        nova_rota_2.clientes.push_back(*it);


    if (nova_rota_2.get_carga() > capacidade)
    {
        cout << "-- capacidade excedida r2--" << endl;
        return s;
    }
    
    s.rotas[id_rota_1] = nova_rota_1;
    s.rotas[id_rota_2] = nova_rota_2;

    s.recalcula_rotulos_utilizados(mapa_rotulos);

    cout << "Movimento 2opt aplicado! Novo custo é " << s.get_custo() << endl;

    return s;    
}