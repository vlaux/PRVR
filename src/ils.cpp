#include "ils.h"
#include "vnd.h"
#include "grasp.h"
#include "busca_local.h"

Solucao Ils::executa(Instancia &ins, Solucao sol_inicial, int max_iter, char* argv[])
{   
    Solucao s_best(ins);

    char* tipo_busca = argv[4];

    s_best = busca_local(sol_inicial, ins, tipo_busca, argv);

    int iter_sem_melhora = 0;

    while(iter_sem_melhora < max_iter)
    {
        Solucao s_temp = movimento_perturbacao_cortes(s_best, ins.get_capacidade(), ins.get_mapa_rotulos());

        s_temp = busca_local(s_temp, ins, tipo_busca, argv);

        if (s_temp.get_custo() < s_best.get_custo())
        {
            iter_sem_melhora = 0;
            s_best = s_temp;
        }

        iter_sem_melhora++;
    }

    return s_best;
}

Solucao Ils::busca_local(Solucao s, Instancia ins, char *tipo_busca, char* argv[]) {
    if (strcmp(tipo_busca, "VND") == 0) {
        int k_max = atoi(argv[5]);
        return Vnd().executa(s, ins, k_max);
    }
    else if (strcmp(tipo_busca, "VNS") == 0) {
        cout << "vns para ils não suportado" << endl;
        abort();
    }
    else if (strcmp(tipo_busca, "TABU") == 0) {
        cout << "tabu para ILS ainda não suportado" << endl;
        abort();
    }
    else {
        cout << "deveria executar movs aleatórios no ILS: não implementado yet" << endl;
        abort();
    }
    return s;
}