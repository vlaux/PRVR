#include <vector>
#include "validador.h"
#include <cassert>

using namespace std;

bool Validador::valida(Instancia *ins, Solucao &s) {
    vector<int> rotulos_utilizados(ins->get_n_rotulos(), 0);

    for (int i = 0; i < s.get_n_rotas(); i++) {
        Rota* r = s.get_rota_ref(i);
        for (int j = 0; j < r->get_tamanho() - 1; j++) {
            int id_rotulo = ins->get_mapa_rotulos()[r->clientes[j].id][r->clientes[j+1].id];
            rotulos_utilizados[id_rotulo]++;
        }
    }

    int acc = 0;
    for (int i = 0; i < rotulos_utilizados.size(); i++)
        if (rotulos_utilizados[i] > 0) acc++;

    assert(s.get_custo() == acc && "Validação de custo falhou");

    return true;
}