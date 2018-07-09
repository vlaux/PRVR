#include "instancia.h"

Instancia::Instancia(int n_clientes, int n_rotulos) {
    this->n_clientes = n_clientes;
    this->n_rotulos = n_rotulos;
    
    for (int i = 0; i < n_rotulos; i++)
        rotulos.push_back(Rotulo(i));

    mapa_rotulos = std::vector<std::vector<int>>(n_clientes + 1);
    for (int i = 0; i <= n_clientes; i++)
        mapa_rotulos[i] = std::vector<int>(n_clientes + 1);
}

Instancia::Instancia(){}

void Instancia::set_capacidade(int capacidade) {
    Instancia::capacidade = capacidade;
}

// int Instancia::get_rotulo_entre(int id_origem, int id_destino) {
//     try {
//         return Instancia::mapa_rotulos[id_origem][id_destino];
//     } catch (std::exception& e) {
//         std::cerr << "Erro ao buscar rótulo. Possível índice errado? " << e.what() << std::endl;
//         exit(EXIT_FAILURE);
//     }
// }

void Instancia::add_cliente(Cliente c) {
    Instancia::clientes.push_back(c);
}

void Instancia::add_rotulo(int id_origem, int id_destino, int rotulo) {
    Instancia::mapa_rotulos[id_origem][id_destino] = rotulo;
}

void Instancia::set_iter_grasp(int n_iter) {
    Instancia::n_iter_grasp = n_iter;
}

Cliente& Instancia::get_cliente(int id_cliente) {
    try {
        return Instancia::clientes.at(id_cliente);
    } catch (std::exception& e) {
        std::cerr << "Erro ao buscar cliente. Possível índice errado? " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

Cliente& Instancia::get_deposito() {
    return Instancia::clientes.at(0);
}

void Instancia::add_frequencia(int id_rotulo, int frequencia) {
    get_rotulo(id_rotulo).frequencia++;
}

Rotulo& Instancia::get_rotulo_entre(Cliente &cliente_origem, Cliente &cliente_destino) {
    try {
        int id_rotulo = this->mapa_rotulos[cliente_origem.id][cliente_destino.id];
        return get_rotulo(id_rotulo);
    } catch (std::exception& e) {
        std::cerr << "Erro ao buscar rótulo entre clientes. Ids de clientes inválidos? " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

Rotulo& Instancia::get_rotulo_entre(int id_origem, int id_destino) {
    try {
        int id_rotulo = mapa_rotulos[id_origem][id_destino];
        return get_rotulo(id_rotulo);
    } catch (std::exception& e) {
        std::cerr << "Erro ao buscar rótulo entre clientes. Ids de clientes inválidos? " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

Rotulo& Instancia::get_rotulo(int id_rotulo) {
    try {
        return rotulos.at(id_rotulo);
    } catch (std::exception& e) {
        std::cerr << "Erro ao buscar rótulo. Id de rótulo inválido? " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

int Instancia::get_frequencia(int id_rotulo) {
    return get_rotulo(id_rotulo).get_frequencia();
}