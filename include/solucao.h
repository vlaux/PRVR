#ifndef SOLUCAO
#define SOLUCAO

#include <vector>
#include <numeric>
#include "solucao.h"
#include "rota.h"
#include "rotulo.h"

using namespace std;

class Solucao
{
  private:
    int n_clientes;
    bool* clientes_visitados;  
    Cliente escolhe_melhor_cliente(const vector<Cliente> &clientes, const Cliente& origem, int** mapa_rotulos);
  public:
    Solucao(int n_rotulos, int n_clientes);
    std::vector<Rota> rotas;
    vector<Rotulo> rotulos;
    int get_custo();
    void adiciona_rota(Rota r);
    void ordena_rotulos_por_uso();
    void cria_solucao(const std::vector<Cliente> &clientes, int** mapa_rotulos, int capacidade);
    void adiciona_cliente(Cliente &c, Rota &r, int** mapa_rotulos);
    void usa_rotulo(int id_rotulo);
    bool existe_cliente_nao_atendido();
    void imprime();
};

#endif
