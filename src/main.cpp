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

    cout << "rotulos antes movimento 1" << endl;
        for (int i = 0; i<n_rotulos && s.rotulos[i].vezes_utilizado > 0; i++)
            cout << "[" << s.rotulos[i].id << "]:"<< s.rotulos[i].vezes_utilizado << ", " ;            

    Solucao s1(n_rotulos, n_clientes);
    for(int i = 0; i< 1000; i++)
    {
        s1 = movimento_2(s, capacidade, rotulos);
        if (s1.get_custo() < s.get_custo())
        {
            s1.imprime();
            cout << "Custo após movimento: " << s1.get_custo() << endl;

            cout << "rotulos após movimento 1" << endl;
            for (int i = 0; i<n_rotulos && s1.rotulos[i].vezes_utilizado > 0 ; i++)
                cout << "[" << s1.rotulos[i].id << "]:"<< s1.rotulos[i].vezes_utilizado << ", " ;
            
            break;
        }
    }
    
    cout << "Não conseguiu" << endl;

    return EXIT_SUCCESS;
}