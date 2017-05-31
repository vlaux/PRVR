#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
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
    cout << "custo da solucao inicial criada: " << s_best.get_custo() << endl;    

    for (int i = 0; i<10000; i++)
    {
        Solucao s(n_rotulos, n_clientes);
        s.cria_solucao(clientes, rotulos, capacidade);
        if (s.get_custo() < s_best.get_custo())
        {
            s_best = s;
            cout << "Melhor solução construída na iteração " << i << ". Custo: " << s_best.get_custo() << endl;    
        }
    }    

    s_best.imprime();

    // cout << "rotulos antes movimento:" << endl;
    //     for (int i = 0; i<n_rotulos && s.rotulos[i].vezes_utilizado > 0; i++)
    //         cout << "[" << s.rotulos[i].id << "]:"<< s.rotulos[i].vezes_utilizado << ", " ;     
    
    ListaTabu tabu = ListaTabu(n_rotulos);
    
    // int iter_max = n_clientes * n_rotulos;
    int iter_max = 100000;
    int iter = 0;
    long double iter_best = -1;

    Solucao s = s_best;

    int k = 1;

    while (iter < iter_max)
    {
        switch(rand()%N_MOVIMENTOS)
        {
            case 0:
                s = movimento_intra_rota(s, rotulos, k);
                break;
            case 1:
                s = movimento_intra_rota_n_rotas(s, rotulos, k);
                break;
            case 2:
                s = movimento_inter_move_n(s, capacidade, rotulos, k);
                break;
            case 3:
                s = movimento_intra_2_opt(s, rotulos, k);
                break;
            default:
                cerr << "Que movimento é esse???" << endl;
                exit(EXIT_FAILURE);
        }
        if (s.get_custo() < s_best.get_custo())
        {
            cout << "<<<<<<<<<<<< Novo s_best na iter " << iter << " com custo: " << s.get_custo() << " >>>>>>>>>>>>>>>" <<endl;
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

    for (int i = 0; i <= n_clientes; i++)
        delete [] rotulos[i];
    
    delete [] rotulos;

    return EXIT_SUCCESS;
}