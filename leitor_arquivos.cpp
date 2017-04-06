#include "./leitor_arquivos.h"
#include "./cliente.h"

using namespace std;

void le_arquivo(string nome_arquivo)
{
    std::ifstream arquivo(nome_arquivo, ios::in);

    if (!arquivo)
    {
        cerr << "Erro ao abrir instância " << nome_arquivo << endl;
        exit(EXIT_FAILURE);
    }

    int n_clientes, capacidade;

    arquivo >> n_clientes >> capacidade;

    Cliente clientes[n_clientes + 1];

    clientes[0].id = 0;
    clientes[0].demanda = 0;
    arquivo >> clientes[0].x_pos >> clientes[0].y_pos;

    for (int i = 1; i <= n_clientes; i++)
    {
        clientes[i].id = i;
        arquivo >> clientes[i].x_pos >> clientes[i].y_pos >> clientes[i].demanda;
    }

    int rotulos[n_clientes + 1][n_clientes + 1];

    for (int i = 0; i <= n_clientes; i++)
    {
        for (int j = 0; j <= n_clientes; j++)
        {
            if (i != j)
                arquivo >> rotulos[i][j];
            else
                rotulos[i][j] = -1;
        }
    }
}
