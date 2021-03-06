#include <vector>
#include "leitor_arquivos.h"
#include "cliente.h"
#include "instancia.h"
#include <math.h>
#include <assert.h>

using namespace std;

Instancia* le_arquivo(string nome_arquivo)
{
    std::ifstream arquivo(nome_arquivo, ios::in);

    if (!arquivo)
    {
        cerr << "Erro ao abrir instância " << nome_arquivo << endl;
        exit(EXIT_FAILURE);
    }

    int n_clientes, capacidade, n_rotulos;

    arquivo >> n_clientes >> capacidade >> n_rotulos;

    Instancia* instancia = new Instancia(n_clientes, n_rotulos);

    instancia->set_capacidade(capacidade);

    Cliente deposito;
    deposito.id = 0;
    deposito.demanda = 0;

    arquivo >> deposito.x_pos >> deposito.y_pos;

    instancia->add_cliente(deposito);

    for (int i = 1; i <= n_clientes; i++)
    {
        Cliente c;
        c.id = i;
        arquivo >> c.x_pos >> c.y_pos >> c.demanda;
        instancia->add_cliente(c);
    }

    int rotulo;
    for (int i = 0; i <= n_clientes; i++)
        for (int j = 0; j <= n_clientes; j++) 
        {
            arquivo >> rotulo;
            instancia->add_rotulo(i, j, rotulo);
            
            if (j < i) 
                instancia->add_frequencia(rotulo);
        }

    return instancia;
}
