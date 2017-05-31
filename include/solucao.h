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
    std::vector<bool> clientes_visitados;
    Cliente escolhe_melhor_cliente(const vector<Cliente> &clientes, const Cliente& origem, int** mapa_rotulos);
    Cliente escolhe_melhor_cliente_grasp(const vector<Cliente> &clientes, const Cliente& origem, int** mapa_rotulos, float alpha);    
  public:
    Solucao(int n_rotulos, int n_clientes);
    std::vector<Rota> rotas;
    std::vector<Rotulo> rotulos;
    int get_custo();
    int get_n_rotas();
    void adiciona_rota(Rota r);
    void ordena_rotulos_por_uso();
    void recalcula_rotulos_utilizados(int** mapa_rotulos);
    void cria_solucao(const std::vector<Cliente> &clientes, int** mapa_rotulos, int capacidade);
    void adiciona_cliente(Cliente &c, Rota &r, int** mapa_rotulos);
    void usa_rotulo(int id_rotulo);
    void remove_rotulo(int id_rotulo);
    void remove_rota(int pos_rota);
    bool existe_cliente_nao_atendido();
    void imprime();
};

#endif
