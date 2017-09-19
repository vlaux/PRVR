#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <assert.h>
#include <math.h>
#include "solucao.h"
#include "rota.h"
#include "rotulo.h"

#define LIMITE_CONSTRUCAO 0.8

bool Solucao::existe_cliente_nao_atendido() {
    int n_clientes = Solucao::instancia->get_n_clientes();
    for (int i = 1; i <= n_clientes; i++)
        if (!clientes_visitados[i]) return true;
    return false;
}

int Solucao::get_custo()
{
    if (rotas.size())
        return std::count_if(rotulos.begin(), rotulos.end(), [](Rotulo r) { return (r.vezes_utilizado > 0);});

    return INFINITY;
}

Solucao::Solucao(Instancia* ins)
{
    Solucao::instancia = ins;
    int n_rotulos = ins->get_n_rotulos();
    rotulos = vector<Rotulo>(n_rotulos);
    clientes_visitados = vector<bool>(ins->get_n_clientes());
    fill(clientes_visitados.begin(), clientes_visitados.end(), false);

    for(int i=0; i<n_rotulos; i++) {
        rotulos[i].id = i;
        rotulos[i].vezes_utilizado = 0;
    }
}

void Solucao::adiciona_rota(Rota rota)
{
    rotas.push_back(rota);
}

void Solucao::ordena_rotulos_por_uso() 
{
    std::sort(rotulos.begin(), rotulos.end(), greater<Rotulo>());   
}

Cliente Solucao::escolhe_melhor_cliente(const vector<Cliente> &clientes, const Cliente& origem, int** mapa_rotulos)
{
    std::vector<Cliente> clientes_viaveis(clientes.size());

    if(!existe_cliente_nao_atendido())
        return origem;

    for(int i=0; i<rotulos.size(); i++){

        clientes_viaveis.erase(clientes_viaveis.begin(), clientes_viaveis.end());
        for_each(clientes.begin()+1, clientes.end(), [&](const Cliente c) {
            if(!clientes_visitados[c.id] && mapa_rotulos[origem.id][c.id] == rotulos[i].id)
                clientes_viaveis.push_back(c);
         });


        if (!clientes_viaveis.empty()) {
            int n_clientes_viaveis = clientes_viaveis.size();
            return clientes_viaveis[rand()%n_clientes_viaveis];
        }
    }
    
    cerr << "Não era pra ter chegado aqui... como assim não tinha nenhum cliente com nenhum rótulo?" << endl;
    fflush(stdout);
    exit(EXIT_FAILURE);

    //return origem;
}

Cliente Solucao::escolhe_melhor_cliente_grasp(const vector<Cliente> &clientes, const Cliente& origem, int** mapa_rotulos, float alpha)
{
    int max = -1, min = 999999;
    vector<int> avaliacao(clientes.size());

    for_each(clientes.begin()+1, clientes.end(), [&](const Cliente c) {
            if(c.id != origem.id && !clientes_visitados[c.id])
            {
                avaliacao[c.id] = rotulos[mapa_rotulos[origem.id][c.id]].vezes_utilizado;

                if (avaliacao[c.id] > max) max = avaliacao[c.id];
                if (avaliacao[c.id] < min) min = avaliacao[c.id];
            }
            else 
                avaliacao[c.id] = -1;
         });

    vector<Cliente> lrc;

    for_each(clientes.begin()+1, clientes.end(), [&](const Cliente c) {
        if (avaliacao[c.id] >= min + alpha*(max-min))
            lrc.push_back(c);
    });

    assert(!lrc.empty());

    return lrc[rand()%lrc.size()];
}

void Solucao::cria_solucao(const std::vector<Cliente> &clientes, int** mapa_rotulos, int capacidade_veiculo)
{
    Cliente deposito = clientes[0];

    while(existe_cliente_nao_atendido()) {
        Rota r(deposito);
        
        Cliente c = escolhe_melhor_cliente_grasp(clientes, r.clientes.back(), mapa_rotulos, 1);
        while (c.id != r.clientes.back().id && r.get_carga() + c.demanda <= capacidade_veiculo * LIMITE_CONSTRUCAO) {
            Solucao::adiciona_cliente(c, r, mapa_rotulos);
            c = escolhe_melhor_cliente_grasp(clientes, r.clientes.back(), mapa_rotulos, 1);
        }

        Solucao::adiciona_cliente(deposito, r, mapa_rotulos);
        adiciona_rota(r);
    }
}

void Solucao::adiciona_cliente(Cliente &c, Rota &r, int** mapa_rotulos)
{
    Cliente ultimo_cliente = r.clientes.back();
    r.adiciona_cliente(c);
    Solucao::clientes_visitados[c.id] = true;
    int id_rotulo = mapa_rotulos[ultimo_cliente.id][c.id];
    Solucao::usa_rotulo(id_rotulo);
}

void Solucao::usa_rotulo(int id_rotulo)
{
    rotulos[id_rotulo].vezes_utilizado += 1;
}

void Solucao::remove_rotulo(int id_rotulo)
{
    if (rotulos[id_rotulo].vezes_utilizado > 0) 
        rotulos[id_rotulo].vezes_utilizado--;
}

// TODO: remover?
// void Solucao::recalcula_rotulos_utilizados(int** mapa_rotulos) 
// {
//     //zera utilização de todos os rótulos
//     for(int i=0; i<rotulos.size(); i++) {
//         rotulos[i].id = i;
//         rotulos[i].vezes_utilizado = 0;
//     }

//     //recalcula utilização   
//     std::for_each(rotas.begin(), rotas.end(), [&](Rota r) {
//         int id_rotulo;
//         for (std::vector<Cliente>::iterator c = r.clientes.begin(); c != std::prev(r.clientes.end()); c++)
//         {
//             id_rotulo = mapa_rotulos[c->id][std::next(c)->id];
//             rotulos[id_rotulo].vezes_utilizado++;
//         }
//      });     

// }

void Solucao::remove_rota(int pos_rota)
{
    rotas.erase(rotas.begin() + pos_rota);
}

int Solucao::get_n_rotas()
{
    return rotas.size();
}

void Solucao::imprime()
{
    cout << "Solucao" << endl;
    for_each(rotas.begin(), rotas.end(), [](Rota r){ 
        cout << "ROTA: ";
        for_each(r.clientes.begin(), r.clientes.end(), [](Cliente c) { cout << " " << c.id;});
        cout << endl;
     });
}
