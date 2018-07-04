#include "instancia.h"

Instancia::Instancia(int n_clientes, int n_rotulos) {
    this->n_clientes = n_clientes;
    this->n_rotulos = n_rotulos;
    this->frequencias = std::vector<int>(n_rotulos, 0);

    mapa_rotulos = std::vector<std::vector<int>>(n_clientes + 1);
    for (int i = 0; i <= n_clientes; i++)
        mapa_rotulos[i] = std::vector<int>(n_clientes + 1);
}

void Instancia::set_capacidade(int capacidade) {
    Instancia::capacidade = capacidade;
}

int Instancia::get_rotulo(int id_origem, int id_destino) {
    try {
        return Instancia::mapa_rotulos[id_origem][id_destino];
    } catch (std::exception& e) {
        std::cerr << "Erro ao buscar rótulo. Possível índice errado? " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Instancia::add_cliente(Cliente c) {
    Instancia::clientes.push_back(c);
}

void Instancia::add_rotulo(int id_origem, int id_destino, int rotulo) {
    Instancia::mapa_rotulos[id_origem][id_destino] = rotulo;
}

void Instancia::set_iter_grasp(int n_iter) {
    Instancia::n_iter_grasp = n_iter;
}

Cliente Instancia::get_cliente(int id_cliente) {
    try {
        return Instancia::clientes.at(id_cliente);
    } catch (std::exception& e) {
        std::cerr << "Erro ao buscar rótulo. Possível índice errado? " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Instancia::add_frequencia(int rotulo, int frequencia) {
    frequencias[rotulo] += frequencia;
}
