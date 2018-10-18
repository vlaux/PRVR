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
#include "construtor.h"
#include "validador.h"
#include "busca_local.h"

using namespace std;

int main(int argc, char *argv[])
{
    srand(time(NULL));

    char *nome_arquivo = argv[1];
    cout << "Executando GRASP para instância " << nome_arquivo << endl;

    Instancia* ins = le_arquivo(nome_arquivo);
    
    int n_iter = ins->get_n_clientes();

    cout << n_iter << endl;

    Solucao s = Construtor(ins).construcao_aleatoria();

    s.imprime();
    // vector<Rotulo> sorted1 = s.rotulos;
    // std::sort(sorted1.begin(), sorted1.end(), greater<Rotulo>());
    // cout << endl;

    // for (int i = 0; i < ins->get_n_rotulos(); i++)
    //     cout << sorted1[i].get_frequencia() << " ";

    Validador().valida(ins, s);

    delete ins;

    return EXIT_SUCCESS;
}