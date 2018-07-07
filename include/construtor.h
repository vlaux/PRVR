#ifndef CONSTRUTOR
#define CONSTRUTOR

#include "solucao.h"
#include "cliente.h"
#include "rota.h"
#include "instancia.h"

#define LIMITE_CONSTRUCAO 0.8

class Construtor {
    public:
        Construtor();
        Construtor(Instancia &ins);
        Solucao construcao_aleatoria();
        Solucao construcao_gulosa();
        Solucao construcao_gulosa_aleatoria(float alpha = 1);
    private:
        Instancia instancia;
        Cliente get_cliente_aleatorio(Solucao &s);
        Cliente get_cliente_guloso(Solucao &s, Cliente &origem);
        Cliente get_cliente_guloso_aleatorio(Solucao &s, Cliente &origem, float alpha);
};

#endif