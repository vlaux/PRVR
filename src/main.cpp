#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "leitor_arquivos.h"
#include "instancia.h"
#include "solucao.h"
#include "grasp.h"
#include "ils.h"
#include "vns.h"
#include "busca_tabu.h"

using namespace std;

int main(int argc, char *argv[])
{
    srand(time(NULL));

    char *nome_arquivo = argv[1];

    Instancia ins = le_arquivo(nome_arquivo);

    char *modo = argv[2];
    
    if (strcmp(modo, "GRASP") == 0) {
        cout << "Executando GRASP para instância " << nome_arquivo << endl;
        int n_iter = *argv[3];
        Solucao s = Grasp(true, n_iter).executa(ins, argv);
        s.imprime();
    }
    else if (strcmp(modo, "ILS") == 0) {
        cout << "Executando ILS para instância " << nome_arquivo << endl;
        
        Solucao sol_inicial(ins);
        sol_inicial = Grasp(false, 0, 0, 1).constroi_solucao(ins);

        int max_iter = atoi(argv[3]);
        Solucao s = Ils().executa(ins, sol_inicial, max_iter, argv);
        s.imprime();
    }
    else if (strcmp(modo, "VNS") == 0) {
        cout << "Executando VNS para instância " << nome_arquivo << endl;
        
        Solucao sol_inicial(ins);
        sol_inicial = Grasp(false, 0, 0, 1).constroi_solucao(ins);

        int k_max = atoi(argv[3]);
        Solucao s = Vns().executa(sol_inicial, ins, argv, k_max);
        s.imprime();
    }
    else if (strcmp(modo, "TABU") == 0) {
        cout << "Executando TABU para instância " << nome_arquivo << endl;
        
        Solucao sol_inicial(ins);
        sol_inicial = Grasp(false, 0, 0, 1).constroi_solucao(ins);

        int max_iter = atoi(argv[3]);
        Solucao s = BuscaTabu().executa(ins, sol_inicial, max_iter, argv);
        s.imprime();
    }
    else {
        cout << "deu merda" << endl;
    }

    return EXIT_SUCCESS;
}

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