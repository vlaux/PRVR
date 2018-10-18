#include <vector>
#include "validador.h"
#include <cassert>

using namespace std;

bool Validador::valida(Instancia *ins, Solucao &s) {
    vector<int> rotulos_utilizados(ins->get_n_rotulos(), 0);
    vector<int> vezes_visitado(ins->get_n_clientes() + 1, 0);

    for (int i = 0; i < s.get_n_rotas(); i++) {
        Rota* r = s.get_rota_ref(i);

        assert(r->get_carga() <= ins->get_capacidade() && "capacidade excedida!");

        assert(r->clientes.front().id == 0 && r->clientes.back().id == 0 && "depósito no começo e no final");

        for (int j = 0; j < r->get_tamanho() - 1; j++) {
            int id_rotulo = ins->get_mapa_rotulos()[r->clientes[j].id][r->clientes[j+1].id];
            vezes_visitado[r->clientes[j].id] += 1;
            rotulos_utilizados[id_rotulo]++;
        }
    }

    int acc = 0;
    for (unsigned int i = 0; i < rotulos_utilizados.size(); i++)
        if (rotulos_utilizados[i] > 0) acc++;

    for (unsigned int i = 1; i < vezes_visitado.size(); i++)
        assert(vezes_visitado[i] == 1 && "cliente visitado mais de uma vez");

    assert(s.get_custo() == acc && "Validação de custo falhou");

    return true;
}