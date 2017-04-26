#include <vector>
#include "leitor_arquivos.h"
#include "cliente.h"

using namespace std;

void le_arquivo(string nome_arquivo, std::vector<Cliente> &clientes, int &capacidade, int &n_clientes, int &n_rotulos, int **&rotulos)
{
    std::ifstream arquivo(nome_arquivo, ios::in);

    if (!arquivo)
    {
        cerr << "Erro ao abrir instância " << nome_arquivo << endl;
        exit(EXIT_FAILURE);
    }

    arquivo >> n_clientes >> capacidade >> n_rotulos;

    Cliente deposito;
    deposito.id = 0;
    deposito.demanda = 0;

    arquivo >> deposito.x_pos >> deposito.y_pos;

    clientes.push_back(deposito);

    for (int i = 1; i <= n_clientes; i++)
    {
        Cliente c;
        c.id = i;
        arquivo >> c.x_pos >> c.y_pos >> c.demanda;
        clientes.push_back(c);
    }

    rotulos = new int *[n_clientes + 1];

    for (int i = 0; i <= n_clientes; i++)
    {
        rotulos[i] = new int[n_clientes + 1];

        for (int j = 0; j <= n_clientes; j++)
        {
            arquivo >> rotulos[i][j];
        }
    }
}
