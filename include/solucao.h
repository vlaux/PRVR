#ifndef SOLUCAO
#define SOLUCAO

#include <vector>
#include <numeric>
#include <assert.h>
#include "instancia.h"
#include "solucao.h"
#include "rota.h"
#include "rotulo.h"
#include "utils.h"

using namespace std;

class Solucao
{
  private:
    Instancia *instancia;
    std::vector<bool> clientes_visitados;
    void copy(const Solucao& s);
    int get_custo_ponderado_rotulos();
  public:
    Solucao(Instancia *ins);
    // Solucao();
    // Solucao(const Solucao& s);
    Solucao operator=(const Solucao& s);
    std::vector<Rota> rotas; // era pra ser private, mas na correria foi pra public pra contornar problema
    std::vector<Rotulo> rotulos; // idem acima
    Rota* get_rota_ref(int id_rota) { 
      assert(id_rota < rotas.size() && "índice maior que no. de rotas");
      return &rotas[id_rota]; 
    };
    Rota get_rota(int id_rota) { return rotas[id_rota]; };
    void update_rota(Rota rota, int id_rota);
    int get_custo();
    int get_custo_ponderado();
    int get_n_rotas();
    Rotulo& get_rotulo(int id_rotulo) { return rotulos[id_rotulo]; };
    Rotulo& get_rotulo_entre(int id_origem, int id_destino);
    void adiciona_rota(Rota r);
    bool is_cliente_visitado(int id_cliente) { return clientes_visitados[id_cliente]; };
    void recalcula_rotulos_utilizados();
    void adiciona_cliente(Cliente &c, Rota &r, Matriz &mapa_rotulos);
    void usa_rotulo(int id_rotulo);
    void remove_rotulo(int id_rotulo);
    void remove_rota(int pos_rota);
    bool existe_cliente_nao_atendido();
    void imprime();
    Instancia* get_instancia();
};

#endif
