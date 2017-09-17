#ifndef INSTANCIA
#define INSTANCIA

#include <iostream>
#include <vector>
#include "instancia.h"
#include "cliente.h"

using namespace std;

class Instancia {
    private:
        int capacidade;
        int n_clientes;
        int n_rotulos;
        int ** mapa_rotulos;
        std::vector<Cliente> clientes;
    public:
        Instancia(int n_clientes, int n_rotulos);
        ~Instancia();
        int get_capacidade();
        int get_rotulo(int id_origem, int id_destino);
        Cliente get_cliente(int id_cliente);
        void set_capacidade(int capacidade);
        void add_cliente(Cliente c);
        void add_rotulo(int id_origem, int id_destino, int rotulo);
};

#endif