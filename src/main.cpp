#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <tuple>
#include "leitor_arquivos.h"
#include "cliente.h"
#include "solucao.h"
#include "rota.h"
#include "busca_local.h"
#include "tabu.h"

using namespace std;

int main()
{
    srand(time(NULL));

    int n_clientes, capacidade, n_rotulos;
    std::vector<Cliente> clientes;
    int** rotulos;
    le_arquivo("ins/c50r500i01.txt", clientes, capacidade, n_clientes, n_rotulos, rotulos);

    std::vector<Rota> rotas;

    Solucao s_best(n_rotulos, n_clientes);

    s_best.cria_solucao(clientes, rotulos, capacidade);

    s_best.imprime();

    cout << "custo da solucao: " << s_best.get_custo() << endl;

    // cout << "rotulos antes movimento:" << endl;
    //     for (int i = 0; i<n_rotulos && s.rotulos[i].vezes_utilizado > 0; i++)
    //         cout << "[" << s.rotulos[i].id << "]:"<< s.rotulos[i].vezes_utilizado << ", " ;     

    ListaTabu tabu = ListaTabu(n_rotulos);
    
    // int iter_max = n_clientes * n_rotulos;
    int iter_max = 1000;
    int iter = 0;
    int iter_best = -1;

    Solucao s = s_best;

    while (iter < iter_max)
    {
        switch(rand()%3)
        {
            case 0:
                s = movimento_1(s, tabu, rotulos);
                break;
            case 1:
                s = movimento_2(s, capacidade, tabu, rotulos);
                break;
            case 2:
                s = movimento_3(s, capacidade, tabu, rotulos);
                break;
        }
        if (s.get_custo() < s_best.get_custo())
        {
            cout << "Novo s_best na iter " << iter << " com custo: " << s.get_custo() << endl;
            s_best = s;
            iter_best = iter;
            iter = 0;
        }
        else 
        {
            s = s_best;
            iter++;
        }
    }
    
    cout << "Melhor solução encontrada na iteração " << iter_best << " com custo " << s_best.get_custo() << endl;

    s_best.imprime();

    return EXIT_SUCCESS;
}