#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <assert.h>
#include <math.h>
#include "solucao.h"
#include "rota.h"
#include "rotulo.h"

bool Solucao::existe_cliente_nao_atendido() {
    int n_clientes = instancia->get_n_clientes();
    for (int i = 1; i <= n_clientes; i++)
        if (!clientes_visitados[i]) return true;
    return false;

}

int Solucao::get_custo()
{
    if (rotas.size())
        return std::count_if(rotulos.begin(), rotulos.end(), [](Rotulo r) { return (r.get_frequencia() > 0);});

    return INT32_MAX;
}

int Solucao::get_custo_ponderado_rotulos()
{
    std::vector<Rotulo> rotulos_ordenados = rotulos;    
    std::sort(rotulos_ordenados.begin(), rotulos_ordenados.end(), greater<Rotulo>());
    int custo = 0;
    for (int i = 0; i < rotulos.size() && rotulos_ordenados[i].get_frequencia() > 0; i++)
        custo += rotulos_ordenados[i].get_frequencia() * rotulos.size() - i;
    
    return custo;
}

int Solucao::get_custo_ponderado()
{
    int custo = get_custo() * rotulos.size() * rotulos.size();
    return custo - get_custo_ponderado_rotulos();
}


Solucao::Solucao(Instancia* ins)
{
    this->instancia = ins;
    int n_rotulos = ins->get_n_rotulos();
    Solucao::rotas = vector<Rota>();
    clientes_visitados = vector<bool>(ins->get_n_clientes() + 1);
    fill(clientes_visitados.begin(), clientes_visitados.end(), false);

    for(int i=0; i<n_rotulos; i++) {
        rotulos.push_back(Rotulo(i));
    }
}

// Solucao::Solucao(const Solucao& s) {
//     copy(s);
// }

Solucao Solucao::operator=(const Solucao& s) 
{
    copy(s);
    return *this;
}

void Solucao::copy(const Solucao& s) 
{
    instancia = s.instancia;
    clientes_visitados = s.clientes_visitados;
    rotas = s.rotas;
    rotulos = s.rotulos;
}

void Solucao::adiciona_rota(Rota rota)
{
    rotas.push_back(rota);
}

void Solucao::update_rota(Rota rota, int id_rota)
{
    try {
        Solucao::rotas[id_rota] = rota;
    } catch(std::exception& e) {
        cerr << "Erro ao atualizar rota com índice " << id_rota << endl << e.what() << endl;
        exit(EXIT_FAILURE);
    }
}

// void Solucao::ordena_rotulos_por_uso() 
// {
//     std::sort(rotulos.begin(), rotulos.end(), greater<Rotulo>());   
// }

void Solucao::adiciona_cliente(Cliente &c, Rota &r, Matriz & mapa_rotulos)
{
    Cliente ultimo_cliente = r.clientes.back();
    r.adiciona_cliente(c);
    Solucao::clientes_visitados[c.id] = true;
    int id_rotulo = mapa_rotulos[ultimo_cliente.id][c.id];
    Solucao::usa_rotulo(id_rotulo);
}

void Solucao::usa_rotulo(int id_rotulo)
{
    rotulos[id_rotulo].frequencia++;
}

void Solucao::remove_rotulo(int id_rotulo)
{
    if (rotulos[id_rotulo].get_frequencia() > 0) 
        rotulos[id_rotulo].frequencia -= 1;
}

void Solucao::recalcula_rotulos_utilizados() 
{
    //zera utilização de todos os rótulos
    for(int i=0; i<rotulos.size(); i++)
        rotulos[i].frequencia = 0;

    //recalcula utilização   
    std::for_each(rotas.begin(), rotas.end(), [&](Rota r) {
        for (std::vector<Cliente>::iterator c = r.clientes.begin(); c != std::prev(r.clientes.end()); c++)
            get_rotulo_entre(c->id, std::next(c)->id).frequencia++;
     });
}

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
    int index = 0;
    for_each(rotas.begin(), rotas.end(), [&index](Rota r){ 
        cout << "ROTA: " << index++ << ": ";
        for_each(r.clientes.begin(), r.clientes.end(), [](Cliente c) { cout << " " << c.id;});
        cout << endl;
     });
     cout << "Custo: " << get_custo() << endl;
     cout << "Rótulos: ";
     for (int i = 0; i < instancia->get_n_rotulos(); i++)
        cout << "[" << i << "]: " << rotulos[i].get_frequencia() << " ";
}

Instancia* Solucao::get_instancia() {
    return instancia;
}

Rotulo& Solucao::get_rotulo_entre(int id_origem, int id_destino) {
    int id_rotulo = instancia->get_rotulo_entre(id_origem, id_destino).id;
    return rotulos[id_rotulo];
}