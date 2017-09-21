#ifndef INSTANCIA
#define INSTANCIA

#include <iostream>
#include <vector>
#include "instancia.h"
#include "cliente.h"
#include "utils.h"

using namespace std;

class Instancia {
    private:
        int capacidade;
        int n_clientes;
        int n_rotulos;
        Matriz mapa_rotulos;
        std::vector<Cliente> clientes;
    public:
        Instancia(int n_clientes, int n_rotulos);
        int get_capacidade() { return capacidade; };
        int get_n_clientes() { return n_clientes; };
        int get_n_rotulos() { return n_rotulos; };
        std::vector<Cliente> get_clientes() { return clientes; }; // DEPRECATED
        Matriz& get_mapa_rotulos() { return mapa_rotulos; } // DEPRECATED
        int get_rotulo(int id_origem, int id_destino);
        Cliente get_cliente(int id_cliente);
        void set_capacidade(int capacidade);
        void add_cliente(Cliente c);
        void add_rotulo(int id_origem, int id_destino, int rotulo);
};

#endif