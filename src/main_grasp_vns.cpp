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
#include "vnd.h"
#include "busca_tabu.h"
#include "construtor.h"
#include "validador.h"
#include "busca_local.h"
#include "utils.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        cerr << "Quantidade de parâmetros incorreta. Uso: `/a.out file.ins alpha n_iter_grasp" << endl;
        return EXIT_FAILURE;
    }

    srand(time(NULL));

    char *nome_arquivo = argv[1];
    float alpha = atof(argv[2]);
    unsigned int n_iter_grasp = atoi(argv[3]);

    cout << "Executando GRASP para instância " << nome_arquivo << endl;

    Instancia* ins = le_arquivo(nome_arquivo);

    BuscaLocal* bl = new Vns();
    BuscaLocal* bt = new BuscaTabu(100, 1, bl);
    Solucao s = Grasp().executa(ins, bt);

    Validador().valida(ins, s);

    delete ins;
    delete bl;
    delete bt;

    return EXIT_SUCCESS;
}