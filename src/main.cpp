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
#include "logger.h"

using namespace std;

Logger *logger = Logger::get_instance();

int main(int argc, char *argv[])
{
    srand(time(NULL));

    char *nome_arquivo = argv[1];

    string file_prefix(nome_arquivo);
    // usa apenas nome da instância (remove pasta "ins/" e extensão ".ins")
    file_prefix = file_prefix.substr(4, file_prefix.size() - 8);

    Instancia *ins = le_arquivo(nome_arquivo);

    Solucao solucao_final(ins);

    char *metaheuristica = argv[2];

    // --------------- GRASP ---------------
    if (strcmp(metaheuristica, "GRASP") == 0)
    {
        file_prefix += "_GRASP_";

        float alpha = atof(argv[3]);
        int iter_grasp = atoi(argv[4]);

        BuscaLocal *bl = nullptr;
        BuscaLocal *bl_aux = nullptr;

        char *metodo_bl = argv[5];

        // ----------- com VNS -----------
        if (strcmp(metodo_bl, "VNS") == 0)
        {
            file_prefix += "VNS_";

            int iter_vns = atoi(argv[6]);
            bl = new Vns(iter_vns);
        }

        // ----------- com VND -----------
        if (strcmp(metodo_bl, "VND") == 0)
        {
            file_prefix += "VND_";
            bl = new Vnd();
        }

        // ----------- com TABU -----------
        if (strcmp(metodo_bl, "TABU") == 0)
        {
            file_prefix += "TABU_";

            int iter_tabu = atoi(argv[6]);
            int tamanho_lista = atoi(argv[7]);
            char *bl_tabu = argv[8];

            // ----------- TABU com VNS -----------
            if (strcmp(bl_tabu, "VNS") == 0)
            {
                file_prefix += "VNS_";

                int iter_bl_aux = atoi(argv[9]);
                bl_aux = new Vns(iter_bl_aux);
            }
            // ----------- TABU com VND -----------
            else if (strcmp(bl_tabu, "VND") == 0)
            {
                file_prefix += "VND_";
                bl_aux = new Vnd();
            }
            bl = new BuscaTabu(iter_tabu, tamanho_lista, bl_aux);
        }

        logger->inicia_logger(file_prefix);

        bool is_reativo = alpha < 0;

        solucao_final = Grasp(is_reativo, iter_grasp, alpha, bl).executa(ins);

        delete bl;
        delete bl_aux;
    }

    // --------------- VNS ---------------
    if (strcmp(metaheuristica, "VNS") == 0)
    {
        file_prefix += "_VNS_";

        int iter_vns = atoi(argv[3]);
        char* modo_construcao = argv[4];

        Solucao s(ins);
        if (strcmp(modo_construcao, "GUL") == 0)
        {
            file_prefix += "GUL_";

            s = Construtor(ins).construcao_gulosa();
        }
        if (strcmp(modo_construcao, "AL") == 0)
        {
            file_prefix += "AL_";

            s = Construtor(ins).construcao_aleatoria();
        }
        if (strcmp(modo_construcao, "GUL_AL") == 0)
        {
            file_prefix += "GUL_AL_";

            float alpha_construcao = atof(argv[5]);
            s = Construtor(ins).construcao_gulosa_aleatoria(alpha_construcao);
        }

        logger->inicia_logger(file_prefix);

        solucao_final = Vns(iter_vns).executa(s);
    }

    // --------------- TABU ---------------
    if (strcmp(metaheuristica, "TABU") == 0)
    {
        file_prefix += "_TABU_";

        int iter_tabu = atoi(argv[3]);
        int tamanho_lista = atoi(argv[4]);
        char* tipo_bl_tabu = argv[5];
        char* modo_construcao = argv[6];
        BuscaLocal* bl_tabu = nullptr;

        Solucao s(ins);
        if (strcmp(modo_construcao, "GUL") == 0)
        {
            file_prefix += "GUL_";

            s = Construtor(ins).construcao_gulosa();
        }
        if (strcmp(modo_construcao, "AL") == 0)
        {
            file_prefix += "AL_";

            s = Construtor(ins).construcao_aleatoria();
        }
        if (strcmp(modo_construcao, "GUL_AL") == 0)
        {
            file_prefix += "GUL_AL_";

            float alpha_construcao = atof(argv[7]);
            s = Construtor(ins).construcao_gulosa_aleatoria(alpha_construcao);
        }

        // ----------- TABU com VNS -----------
        if (strcmp(tipo_bl_tabu, "VNS") == 0)
        {
            file_prefix += "VNS_";

            int iter_bl_aux = atoi(argv[9]);
            bl_tabu = new Vns(iter_bl_aux);
        }
        // ----------- TABU com VND -----------
        else if (strcmp(tipo_bl_tabu, "VND") == 0)
        {
            file_prefix += "VND_";

            bl_tabu = new Vnd();
        }

        logger->inicia_logger(file_prefix);

        solucao_final = BuscaTabu(iter_tabu, tamanho_lista, bl_tabu).executa(s);

        delete bl_tabu;
    }

    Validador().valida(ins, solucao_final);

    delete ins;

    logger->finaliza_logger();

    return EXIT_SUCCESS;
}