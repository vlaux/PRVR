#include <iostream>
#include <tuple>
#include <assert.h>
#include "solucao.h"
#include "rota.h"
// #include "tabu.h"

#define MODULO(x) ((x) >= 0 ? (x) : -(x))

using namespace std;

/* 
 * Movimento intra rota
 * Troca k clientes de posição dentro de uma mesma rota
 */
Solucao movimento_intra_rota(Solucao s, int **mapa_rotulos, int k)
{
    //count << "Movimento #1 - Intra-rota ";
    int n_rotas = s.get_n_rotas();
    if (n_rotas < 1)
        return s;

    int id_rota = rand() % n_rotas;
    Rota *r = &s.rotas[id_rota];

    //count << "Rota " << id_rota;

    int tamanho = r->get_tamanho();

    if (tamanho < 4) // pelo menos dois clientes são necessários: depósito, cliente 1, cliente 2, depósito
        return s;

    int pos_origem, pos_destino;

    for (int i = 0; i < k; i++)
    {
        pos_origem = rand() % (tamanho - 2) + 1;
        pos_destino = pos_origem;

        while (pos_destino == pos_origem)
        {
            pos_destino = rand() % (tamanho - 2) + 1;
        }

        //count << ", posições " << pos_origem << " e " << pos_destino << endl;

        // std::tuple<int, int, int> mov_1 = std::make_tuple(r->clientes[pos_origem].id, id_rota, pos_destino);
        // std::tuple<int, int, int> mov_2 = std::make_tuple(r->clientes[pos_destino].id, id_rota, pos_origem);

        // if (tabu.is_tabu(mov_1) || tabu.is_tabu(mov_2))
        // {
        //     //count << "-- movimento tabu --" << endl;
        //     return s;
        // }
        // else
        // {
        //     tabu.adiciona(mov_1);
        //     tabu.adiciona(mov_2);
        // }

        //Efetua a troca
        std::swap(r->clientes[pos_origem], r->clientes[pos_destino]);

        s.recalcula_rotulos_utilizados(mapa_rotulos);
    }

    //count << "Trocado! Custo agora é " << s.get_custo() << endl;

    return s;
}

/* 
 * Movimento intra rota para múltiplas rotas
 * Troca clientes de posição k vezes, em quaisquer rotas da solução
 */
Solucao movimento_intra_rota_n_rotas(Solucao s, int **mapa_rotulos, int k)
{
    //count << "Movimento 1 - Intra rota múltiplas rotas";
    int n_rotas = s.get_n_rotas();
    if (n_rotas < 1)
        return s;

    int id_rota, tamanho;
    Rota *r;

    for (int i = 0; i < k; i++)
    {
        id_rota = rand() % n_rotas;
        r = &s.rotas[id_rota];

        //count << "Rota " << id_rota;

        tamanho = r->get_tamanho();

        if (tamanho < 4) // pelo menos dois clientes são necessários: depósito, cliente 1, cliente 2, depósito
            return s;

        int pos_origem = rand() % (tamanho - 2) + 1;
        int pos_destino = pos_origem;

        while (pos_destino == pos_origem)
        {
            pos_destino = rand() % (tamanho - 2) + 1;
        }

        //count << ", posições " << pos_origem << " e " << pos_destino << endl;

        // std::tuple<int, int, int> mov_1 = std::make_tuple(r->clientes[pos_origem].id, id_rota, pos_destino);
        // std::tuple<int, int, int> mov_2 = std::make_tuple(r->clientes[pos_destino].id, id_rota, pos_origem);

        // if (tabu.is_tabu(mov_1) || tabu.is_tabu(mov_2))
        // {
        //     //count << "-- movimento tabu --" << endl;
        //     return s;
        // }
        // else
        // {
        //     tabu.adiciona(mov_1);
        //     tabu.adiciona(mov_2);
        // }

        //Efetua a troca
        std::swap(r->clientes[pos_origem], r->clientes[pos_destino]);

        s.recalcula_rotulos_utilizados(mapa_rotulos);
    }

    //count << "Trocado! Custo agora é " << s.get_custo() << endl;

    return s;
}

/* 
 * Movimento inter rota
 * Move k clientes de uma rota para outras rotas
 */
Solucao movimento_inter_move_n(Solucao s, int capacidade, int **mapa_rotulos, int k)
{
    //count << "Movimento 2 - Realocação - ";
    int n_rotas = s.get_n_rotas();
    if (n_rotas < 2)
        return s;

    int pos_rota_1 = rand() % n_rotas;
    int pos_rota_2;
    int posicao_origem, posicao_destino;
    Rota *rota_1, *rota_2;

    for (int i = 0; i < k; i++)
    {
        pos_rota_2 = pos_rota_1;

        while (pos_rota_1 == pos_rota_2)
            pos_rota_2 = rand() % n_rotas;

        rota_1 = &s.rotas[pos_rota_1];
        rota_2 = &s.rotas[pos_rota_2];

        posicao_origem = rand() % (rota_1->get_tamanho() - 2) + 1;
        posicao_destino = rand() % (rota_2->get_tamanho() - 2) + 1;

        Cliente cliente_movido = rota_1->clientes[posicao_origem];
        //count << "cliente " << cliente_movido.id << " na pos " << posicao_origem << " da rota " << pos_rota_1;
        //count << " para pos " << posicao_destino << " da rota " << pos_rota_2 << endl;

        if (rota_2->get_carga() + cliente_movido.demanda > capacidade) // capacidade excedida // throw(?)
        {
            //count << "-- capacidade excedida --" << endl;
            return s;
        }

        rota_2->clientes.insert(rota_2->clientes.begin() + posicao_destino, cliente_movido);
        rota_1->clientes.erase(rota_1->clientes.begin() + posicao_origem);

        if (rota_1->get_tamanho() < 3) // só sobrou o depósito (no início e no fim)
        {
            s.remove_rota(pos_rota_1);
            return s;
        }

        //count << "Realocado! Custo agora é " << s.get_custo() << endl;
    }

    s.recalcula_rotulos_utilizados(mapa_rotulos);

    return s;
}

/* 
 * Movimento de troca 2-opt
 * Escolhe dois pontos na rota e inverte todos os clientes entre eles
 * TODO adicionar movimento Tabu
 */
Solucao movimento_intra_2_opt(Solucao s, int **mapa_rotulos, int k)
{
    //count << "Movimento intra rota 2-opt" << endl;

    int n_rotas = s.get_n_rotas();
    if (n_rotas < 1)
        return s;

    int pos_rota = rand() % n_rotas;
    Rota *r = &s.rotas[pos_rota];

    //count << "Rota " << pos_rota;

    int tamanho = r->get_tamanho();

    if (tamanho < (2+k+1)) // pelo menos 2 (depósito no início e fim) + (k+1) clientes necessários
        return s;

    int teto_inicio = (tamanho - 2 - k);
    assert(teto_inicio > 0);

    int pos_inicio = (rand() % teto_inicio) + 1;
    assert(pos_inicio > 0);

    //count << " - revertendo clientes de " << pos_inicio << " até " << pos_inicio + k << endl;

    reverse(r->clientes.begin() + pos_inicio, r->clientes.begin() + pos_inicio + k);

    s.recalcula_rotulos_utilizados(mapa_rotulos);

    return s;    
}

/* 
 * Movimento de perturbação
 * Cria k pontos de corte em um par de rotas e alterna entre esses pontos
 * TODO: atualmente, só funciona para k = 1 (um ponto de corte em cada rota)
 */
Solucao movimento_perturbacao_cortes(Solucao s, int capacidade, int **mapa_rotulos)
{
    //count << "Movimento de perturbação ";
    int n_rotas = s.get_n_rotas();
    if (n_rotas < 2)
        return s;

    int pos_rota_1 = rand() % n_rotas;
    int pos_rota_2 = pos_rota_1;

    while (pos_rota_1 == pos_rota_2)
        pos_rota_2 = rand() % n_rotas;

    Rota rota_1 = s.rotas[pos_rota_1];
    Rota rota_2 = s.rotas[pos_rota_2];

    int ponto_corte_rota_1 = rand() % (rota_1.get_tamanho() - 2) + 1;
    int ponto_corte_rota_2 = rand() % (rota_2.get_tamanho() - 2) + 1;

    //count << "Rota " << pos_rota_1 << ":ponto de corte " << ponto_corte_rota_1;
    //count << " e Rota " << pos_rota_2 << ":ponto de corte " << ponto_corte_rota_2 << endl;

    Rota nova_rota_1;
    Rota nova_rota_2;

    for (std::vector<Cliente>::iterator it = rota_1.clientes.begin(); it != rota_1.clientes.begin() + ponto_corte_rota_1; it++)
        nova_rota_1.clientes.push_back(*it);
    for (std::vector<Cliente>::iterator it = rota_2.clientes.begin() + ponto_corte_rota_2; it != rota_2.clientes.end(); it++)
        nova_rota_1.clientes.push_back(*it);

    if (nova_rota_1.get_carga() > capacidade)
    {
        //count << "-- capacidade excedida r1 --" << endl;
        return s;
    }

    for (std::vector<Cliente>::iterator it = rota_2.clientes.begin(); it != rota_2.clientes.begin() + ponto_corte_rota_2; it++)
        nova_rota_2.clientes.push_back(*it);
    for (std::vector<Cliente>::iterator it = rota_1.clientes.begin() + ponto_corte_rota_1; it != rota_1.clientes.end(); it++)
        nova_rota_2.clientes.push_back(*it);

    if (nova_rota_2.get_carga() > capacidade)
    {
        //count << "-- capacidade excedida r2--" << endl;
        return s;
    }

    s.rotas[pos_rota_1] = nova_rota_1;
    s.rotas[pos_rota_2] = nova_rota_2;

    //remove a rota caso ela tenha virado uma rota com apenas depósito [0-0]
    if (nova_rota_1.get_tamanho() < 3)
        s.remove_rota(pos_rota_1);

    if (nova_rota_2.get_tamanho() < 3)
        s.remove_rota(pos_rota_2);

    s.recalcula_rotulos_utilizados(mapa_rotulos);

    //count << "Movimento 2opt aplicado! Novo custo é " << s.get_custo() << endl;

    return s;
}