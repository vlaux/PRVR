#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "leitor_arquivos.h"
#include "cliente.h"
#include "solucao.h"
#include "rota.h"
// #include "busca_local.h"
#include "vnd.h"

using namespace std;

// int main()
// {
//     srand(time(NULL));

//     int n_clientes, capacidade, n_rotulos;
//     std::vector<Cliente> clientes;
//     Matriz & rotulos;
//     le_arquivo("ins/c50r500i01.txt", clientes, capacidade, n_clientes, n_rotulos, rotulos);

//     std::vector<Rota> rotas; 

//     Solucao s_best(n_rotulos, n_clientes);
//     s_best.cria_solucao(clientes, rotulos, capacidade);
//     cout << "custo da solucao inicial criada: " << s_best.get_custo() << endl;    

//     for (int i = 0; i<10000; i++)
//     {
//         Solucao s(n_rotulos, n_clientes);
//         s.cria_solucao(clientes, rotulos, capacidade);
//         if (s.get_custo() < s_best.get_custo())
//         {
//             s_best = s;
//             cout << "Melhor solução construída na iteração " << i << ". Custo: " << s_best.get_custo() << endl;    
//         }
//     }    

//     s_best.imprime();
    
//     // int iter_max = n_clientes * n_rotulos;
//     // int iter_max = 100000;
//     // int iter = 0;
//     // long double iter_best = -1;

//     Solucao s = s_best;

//     s = aplica_vnd(s, n_clientes, capacidade, rotulos, 6);

//     if (s.get_custo() < s_best.get_custo())
//         s_best = s;

//     s_best.imprime();

//     for (int i = 0; i <= n_clientes; i++)
//         delete [] rotulos[i];
    
//     delete [] rotulos;

//     return EXIT_SUCCESS;
// }