#ifndef INSTANCIA
#define INSTANCIA

#include <iostream>
#include <vector>
#include "instancia.h"
#include "cliente.h"
#include "utils.h"
#include "rotulo.h"

using namespace std;

class Instancia {
    private:
        int capacidade;
        int n_clientes;
        int n_rotulos;
        Matriz mapa_rotulos;
        std::vector<Cliente> clientes;
        int n_iter_grasp;
        vector<Rotulo> rotulos;
    public:
        Instancia();
        Instancia(int n_clientes, int n_rotulos);
        int get_capacidade() { return capacidade; };
        int get_n_clientes() { return n_clientes; };
        int get_n_rotulos() { return n_rotulos; };
        std::vector<Cliente> get_clientes() { return clientes; }; // DEPRECATED
        Matriz& get_mapa_rotulos() { return mapa_rotulos; } // DEPRECATED
        Cliente get_cliente(int id_cliente);
        Cliente get_deposito();
        void set_capacidade(int capacidade);
        void add_cliente(Cliente c);
        void add_rotulo(int id_origem, int id_destino, int rotulo);
        void set_iter_grasp(int n_iter);
        void add_frequencia(int rotulo, int frequencia = 1);
        Rotulo& get_rotulo_entre(Cliente &cliente_origem, Cliente &destino);
        Rotulo& get_rotulo_entre(int id_origem, int id_destino);
        Rotulo& get_rotulo(int id_rotulo);
        int get_frequencia(int id_rotulo);
};

#endif