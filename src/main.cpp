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
        char* tipo_bl = argv[4];
        int k_max_bl = atoi(argv[5]);
        Solucao s = Vns(tipo_bl, k_max_bl).executa(sol_inicial, ins, k_max);
        s.imprime();
    }
    else if (strcmp(modo, "TABU") == 0) {
        cout << "Executando TABU para instância " << nome_arquivo << endl;
        
        Solucao sol_inicial(ins);
        sol_inicial = Grasp(false, 0, 0, 1).constroi_solucao(ins);

        int max_iter = atoi(argv[3]);
        char* tipo_bl = argv[4];
        int k_max_bl = atoi(argv[5]);
        Solucao s = BuscaTabu(tipo_bl, k_max_bl).executa(ins, sol_inicial, max_iter);
        s.imprime();
    }
    else {
        cout << "Heurística desconhecida" << endl;
        abort();
    }

    return EXIT_SUCCESS;
}