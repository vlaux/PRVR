#ifndef ROTA
#define ROTA

#include <vector>
#include <numeric>
#include "cliente.h"

using namespace std;

class Rota
{
  public:
    Rota(Cliente &base);
    std::vector<Cliente> clientes;
    int get_carga();
    void adiciona_cliente(Cliente &c);
    int get_tamanho();
};

#endif