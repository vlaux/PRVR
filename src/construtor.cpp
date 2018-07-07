#include "construtor.h"
#include <cstdlib>
#include <cassert>
#include <map>

using namespace std;

Construtor::Construtor(Instancia &ins) {
    this->instancia = ins;
}

Construtor::Construtor() {}

Solucao Construtor::construcao_aleatoria() {
    Solucao s(this->instancia);

    Cliente deposito = instancia.get_deposito();
    
    while(s.existe_cliente_nao_atendido()) {
        Rota r(deposito);
        
        Cliente c = get_cliente_aleatorio(s);
        while (c.id != 0 && r.get_carga() + c.demanda <= instancia.get_capacidade() * LIMITE_CONSTRUCAO) {
            s.adiciona_cliente(c, r, instancia.get_mapa_rotulos());
            c = get_cliente_aleatorio(s);
        }

        s.adiciona_cliente(deposito, r, instancia.get_mapa_rotulos());
        s.adiciona_rota(r);
    }

    return s;
}

Solucao Construtor::construcao_gulosa() {
    Solucao s(this->instancia);

    Cliente deposito = instancia.get_deposito();
    
    while(s.existe_cliente_nao_atendido()) {
        Rota r(deposito);
        
        Cliente c = get_cliente_guloso(s, r.clientes.back());
        while (c.id != 0 && r.get_carga() + c.demanda <= instancia.get_capacidade() * LIMITE_CONSTRUCAO) {
            s.adiciona_cliente(c, r, instancia.get_mapa_rotulos());
            c = get_cliente_guloso(s, r.clientes.back());
        }

        s.adiciona_cliente(deposito, r, instancia.get_mapa_rotulos());
        s.adiciona_rota(r);
    }

    return s;
}

Solucao Construtor::construcao_gulosa_aleatoria(float alpha) {
    Solucao s(this->instancia);

    Cliente deposito = instancia.get_deposito();
    
    while(s.existe_cliente_nao_atendido()) {
        Rota r(deposito);
        
        Cliente c = get_cliente_guloso_aleatorio(s, r.clientes.back(), alpha);
        while (c.id != 0 && r.get_carga() + c.demanda <= instancia.get_capacidade() * LIMITE_CONSTRUCAO) {
            s.adiciona_cliente(c, r, instancia.get_mapa_rotulos());
            c = get_cliente_guloso_aleatorio(s, r.clientes.back(), alpha);
        }

        s.adiciona_cliente(deposito, r, instancia.get_mapa_rotulos());
        s.adiciona_rota(r);
    }

    return s;
}

Cliente Construtor::get_cliente_aleatorio(Solucao &s) {
    vector<int> id_clientes_nao_visitados;

    for (int i = 1; i <= this->instancia.get_n_clientes(); i++)
        if (!s.is_cliente_visitado(i)) 
            id_clientes_nao_visitados.push_back(i);

    // retorna depósito caso não haja mais cliente não-visitado
    int id_cliente_sorteado = 0;
    if (id_clientes_nao_visitados.size()) {
        int rand_pos = rand() % id_clientes_nao_visitados.size();
        id_cliente_sorteado = id_clientes_nao_visitados.at(rand_pos);
    }

    return this->instancia.get_cliente(id_cliente_sorteado);
}

Cliente Construtor::get_cliente_guloso(Solucao &s, Cliente &origem) {
    vector<int> id_clientes_nao_visitados;

    for (int i = 1; i <= this->instancia.get_n_clientes(); i++)
        if (!s.is_cliente_visitado(i)) 
            id_clientes_nao_visitados.push_back(i);

    if (id_clientes_nao_visitados.size() == 0)
        return instancia.get_deposito();

    vector<int> avaliacoes(id_clientes_nao_visitados.size(), 0);

    for (int i=0; i<id_clientes_nao_visitados.size(); i++)
        avaliacoes.at(i) = s.get_rotulo_entre(id_clientes_nao_visitados.at(i), origem.id).get_frequencia();

    int max_pos = -1, max_freq = 0;
    for (int i=0; i<id_clientes_nao_visitados.size(); i++) {
        if (avaliacoes.at(i) > max_freq) {
            max_pos = i;
            max_freq = avaliacoes.at(i);
        }
    }
    
    if (max_freq > 0)
        return instancia.get_cliente(id_clientes_nao_visitados[max_pos]);

    for (int i=0; i<id_clientes_nao_visitados.size(); i++)
        avaliacoes.at(i) = instancia.get_frequencia(s.get_rotulo_entre(id_clientes_nao_visitados.at(i), origem.id).id);

    max_pos = -1;
    max_freq = -1;
    for (int i=0; i<id_clientes_nao_visitados.size(); i++) {
        if (avaliacoes.at(i) > max_freq) {
            max_pos = i;
            max_freq = avaliacoes.at(i);
        }
    }
    
    return instancia.get_cliente(id_clientes_nao_visitados[max_pos]);
}

Cliente Construtor::get_cliente_guloso_aleatorio(Solucao &s, Cliente &origem, float alpha) {
    vector<int> id_clientes_nao_visitados;

    for (int i = 1; i <= this->instancia.get_n_clientes(); i++)
        if (!s.is_cliente_visitado(i)) 
            id_clientes_nao_visitados.push_back(i);

    if (id_clientes_nao_visitados.size() == 0)
        return instancia.get_deposito();

    vector<int> avaliacoes(id_clientes_nao_visitados.size(), 0);

    for (int i=0; i<id_clientes_nao_visitados.size(); i++)
        avaliacoes.at(i) = s.get_rotulo_entre(id_clientes_nao_visitados.at(i), origem.id).get_frequencia();

    int min_freq = INT32_MAX, max_freq = -1;
    for (int i=0; i<id_clientes_nao_visitados.size(); i++) {
        int avaliacao_corrente = avaliacoes.at(i);
        if (avaliacao_corrente > max_freq)
            max_freq = avaliacao_corrente;
        if (avaliacao_corrente < min_freq)
            min_freq = avaliacao_corrente;
    }
    
    if (max_freq == 0) {
        min_freq = INT32_MAX; max_freq = -1;
        for (int i=0; i<id_clientes_nao_visitados.size(); i++)
            avaliacoes.at(i) = instancia.get_frequencia(s.get_rotulo_entre(id_clientes_nao_visitados.at(i), origem.id).id);
    }

    for (int i=0; i<id_clientes_nao_visitados.size(); i++) {
        int avaliacao_corrente = avaliacoes.at(i);
        if (avaliacao_corrente > max_freq)
            max_freq = avaliacao_corrente;
        if (avaliacao_corrente < min_freq)
            min_freq = avaliacao_corrente;
    }

    vector<int> lrc;

    for (int i=0; i<id_clientes_nao_visitados.size(); i++)
        if (avaliacoes[i] >= (min_freq + alpha*(max_freq-min_freq)))
            lrc.push_back(i);

    assert(!lrc.empty());
    
    int pos_sorteada = lrc[rand()%lrc.size()];
    int id_cliente_sorteado = id_clientes_nao_visitados.at(pos_sorteada);

    return instancia.get_cliente(id_cliente_sorteado);
}
