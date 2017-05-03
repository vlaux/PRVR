#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "leitor_arquivos.h"
#include "cliente.h"
#include "solucao.h"
#include "rota.h"

using namespace std;

int main()
{
    srand(time(NULL));

    int n_clientes, capacidade, n_rotulos;
    std::vector<Cliente> clientes;
    int** rotulos;
    le_arquivo("ins/c50r500i01.txt", clientes, capacidade, n_clientes, n_rotulos, rotulos);

    std::vector<Rota> rotas;

    Solucao s(n_rotulos, n_clientes);

    s.cria_solucao(clientes, rotulos, capacidade);

    s.imprime();

    cout << "custo da solucao: " << s.get_custo() << endl;

    cout << "rotulos" << endl;
    for (int i = 0; i<n_rotulos; i++)
        cout << s.rotulos[i].vezes_utilizado << " " ;

    return EXIT_SUCCESS;
}