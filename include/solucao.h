#ifndef SOLUCAO
#define SOLUCAO

#include <vector>
#include <numeric>
#include "instancia.h"
#include "solucao.h"
#include "rota.h"
#include "rotulo.h"
#include "utils.h"

using namespace std;

class Solucao
{
  private:
    Instancia &instancia;
    std::vector<bool> clientes_visitados;
    std::vector<Rota> rotas;
    std::vector<Rotulo> rotulos;
    Cliente escolhe_melhor_cliente(const vector<Cliente> &clientes, const Cliente& origem, Matriz &mapa_rotulos);
    Cliente escolhe_melhor_cliente_grasp(const vector<Cliente> &clientes, const Cliente& origem, Matriz &mapa_rotulos, float alpha);
    void copy(const Solucao& s);      
  public:
    Solucao(Instancia &ins);
    Solucao(const Solucao& s);
    Solucao& operator=(const Solucao& s);      
    Rota* get_rota_ref(int id_rota) { return &rotas[id_rota]; };
    Rota get_rota(int id_rota) { return rotas[id_rota]; };
    void update_rota(Rota rota, int id_rota);
    int get_custo();
    int get_n_rotas();
    void adiciona_rota(Rota r);
    void ordena_rotulos_por_uso();
    void recalcula_rotulos_utilizados(Matriz &mapa_rotulos);
    void cria_solucao(const std::vector<Cliente> &clientes, Matriz &mapa_rotulos, int capacidade);
    void adiciona_cliente(Cliente &c, Rota &r, Matriz &mapa_rotulos);
    void usa_rotulo(int id_rotulo);
    void remove_rotulo(int id_rotulo);
    void remove_rota(int pos_rota);
    bool existe_cliente_nao_atendido();
    void imprime();
};

#endif
