#include "rota.h"
#include <vector>
#include <numeric>
#include "cliente.h"

using namespace std;

int Rota::get_carga()
{
    return std::accumulate(clientes.begin(), clientes.end(), 0, [](int sum, Cliente c) { return sum + c.demanda; });
}

void Rota::adiciona_cliente(Cliente &c)
{
    clientes.push_back(c);
}

Rota::Rota(Cliente &base)
{
    Rota::adiciona_cliente(base);
}

Rota::Rota() {}

int Rota::get_tamanho()
{
    return clientes.size();
}