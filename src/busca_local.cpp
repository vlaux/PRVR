#include <iostream>
#include "solucao.h"
#include "rota.h"

#define MODULO(x) ((x)>=0?(x):-(x))

using namespace std;

Solucao movimento_1(Solucao s, int** mapa_rotulos)
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
    //Efetua a troca
    std::swap(r->clientes[pos_1], r->clientes[pos_2]);

    s.recalcula_rotulos_utilizados(mapa_rotulos);

    cout << "Trocado! Custo agora é " << s.get_custo() << endl;

    return s;
}

// Solucao movimento_2(Solucao s, int capacidade, int** mapa_rotulos)
// {
//     cout << "Movimento 2 - Realocação - ";
//     int n_rotas = s.get_n_rotas();
//     if (n_rotas < 2)
//         return s;
    
//     int id_rota_1 = rand()%n_rotas;
//     int id_rota_2 = id_rota_1;

//     while (id_rota_1 == id_rota_2)
//         id_rota_2 = rand()%n_rotas;

//     Rota rota_1 = s.rotas[id_rota_1];
//     Rota rota_2 = s.rotas[id_rota_2];
    
//     int posicao_origem = rand()%(rota_1.get_tamanho() - 2) + 1;
//     int posicao_destino = rand()%(rota_2.get_tamanho() - 2) + 1;

//     Cliente cliente_movido = rota_1.clientes[posicao_origem];

//     if (rota_2.get_carga() + cliente_movido.demanda > capacidade) // capacidade excedida // throw(?)
//         return s; 

//     rota_2.clientes.insert(rota_2.clientes.begin() + posicao_destino, cliente_movido);
//     rota_1.clientes.erase(posicao_origem);

//     s.recalcula_rotulos_utilizados(mapa_rotulos);

//     Rota r = s.rotas[id_rota];   
// }