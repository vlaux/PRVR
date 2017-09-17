#include "instancia.h"

Instancia::Instancia(int n_clientes, int n_rotulos) {
    Instancia::n_clientes = n_clientes;
    Instancia::n_rotulos = n_rotulos;

    // Instancia matriz com mapa de rótulos. n_clientes + 1 por causa do depósito
    Instancia::mapa_rotulos = new int *[n_clientes + 1];
    for (int i = 0; i <= n_clientes; i++)
        Instancia::mapa_rotulos[i] = new int[n_clientes + 1];
}

Instancia::~Instancia() {
    for (int i = 0; i <= n_clientes; i++)
        delete [] Instancia::mapa_rotulos[i];

    delete [] Instancia::mapa_rotulos;
}

int Instancia::get_capacidade() {
    return capacidade;
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
    try {
        Instancia::mapa_rotulos[id_origem][id_destino] = rotulo;
    } catch (std::exception& e) {
        std::cerr << "Erro ao adicionar rótulo ao mapa. Possível índice errado? (" << id_origem << ", " << id_destino << ") " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

Cliente Instancia::get_cliente(int id_cliente) {
    try {
        return Instancia::clientes.at(id_cliente);
    } catch (std::exception& e) {
        std::cerr << "Erro ao buscar rótulo. Possível índice errado? " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}
