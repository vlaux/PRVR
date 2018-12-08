#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <string.h>
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

        // ----------- com ILS -----------
        else if (strcmp(metodo_bl, "ILS") == 0)
        {
            file_prefix += "ILS_";

            int iter_ils = atoi(argv[6]);
            int iter_vns_aux = 1; // FIXAR BASEADO EM OUTROS TESTES (?)
            bl_aux = new Vns(iter_vns_aux); // FIXAR BASEADO EM TESTES GRASP
            bl = new Ils(iter_ils, bl_aux);
        }

        // ----------- com VND -----------
        else if (strcmp(metodo_bl, "VND") == 0)
        {
            file_prefix += "VND_";
            bl = new Vnd();
        }

        // ----------- com TABU -----------
        else if (strcmp(metodo_bl, "TABU") == 0)
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

        solucao_final = Grasp(is_reativo, iter_grasp, alpha, bl, logger).executa(ins);
        
        delete bl;
        if (bl_aux != nullptr) delete bl_aux;
    }

    // --------------- VNS ---------------
    else if (strcmp(metaheuristica, "VNS") == 0)
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

        BuscaLocal* vns = new Vns(iter_vns);
        vns->registra_logger(logger);
        solucao_final = vns->executa(s);

        delete vns;
    }

    // --------------- ILS ---------------
    else if (strcmp(metaheuristica, "ILS") == 0)
    {
        file_prefix += "_ILS_";

        BuscaLocal* bl = nullptr;

        int iter_ils = atoi(argv[3]);
        char* modo_construcao = argv[4];
        char* tipo_bl = argv[5];

        Solucao s(ins);
        if (strcmp(modo_construcao, "GUL") == 0)
        {
            file_prefix += "GUL_";

            s = Construtor(ins).construcao_gulosa();
        }
        else if (strcmp(modo_construcao, "AL") == 0)
        {
            file_prefix += "AL_";

            s = Construtor(ins).construcao_aleatoria();
        }
        else if (strcmp(modo_construcao, "GUL_AL") == 0)
        {
            file_prefix += "GUL_AL_";

            float alpha_construcao = 0.8; // FIXAR BASEADO EM TESTES GRASP
            s = Construtor(ins).construcao_gulosa_aleatoria(alpha_construcao);
        }

        // ----------- ILS com VNS -----------
        if (strcmp(tipo_bl, "VNS") == 0)
        {
            file_prefix += "VNS_";

            int iter_vns = atoi(argv[6]);
            bl = new Vns(iter_vns);
        }
        // ----------- ILS com VND -----------
        else if (strcmp(tipo_bl, "VND") == 0)
        {
            file_prefix += "VND_";

            bl = new Vnd();
        }

        logger->inicia_logger(file_prefix);

        BuscaLocal* ils = new Ils(iter_ils, bl);
        ils->registra_logger(logger);
        solucao_final = ils->executa(s);

        delete ils;
    }

    // --------------- TABU ---------------
    else if (strcmp(metaheuristica, "TABU") == 0)
    {
        file_prefix += "_TABU_";

        int iter_tabu = atoi(argv[3]);
        int tamanho_lista = atoi(argv[4]);
        char* modo_construcao = argv[5];
        char* tipo_bl_tabu = argv[6];
        BuscaLocal* bl_tabu = nullptr;
        BuscaLocal* bl_ils_tabu = nullptr;

        Solucao s(ins);
        if (strcmp(modo_construcao, "GUL") == 0)
        {
            file_prefix += "GUL_";

            s = Construtor(ins).construcao_gulosa();
        }
        else if (strcmp(modo_construcao, "AL") == 0)
        {
            file_prefix += "AL_";

            s = Construtor(ins).construcao_aleatoria();
        }
        else if (strcmp(modo_construcao, "GUL_AL") == 0)
        {
            file_prefix += "GUL_AL_";

            float alpha_construcao = 0.8; // FIXAR TESTES GRASP
            s = Construtor(ins).construcao_gulosa_aleatoria(alpha_construcao);
        }

        // ----------- TABU com VNS -----------
        if (strcmp(tipo_bl_tabu, "VNS") == 0)
        {
            file_prefix += "VNS_";

            int iter_bl_aux = atoi(argv[7]);
            bl_tabu = new Vns(iter_bl_aux);
        }
        // ----------- TABU com VND -----------
        else if (strcmp(tipo_bl_tabu, "VND") == 0)
        {
            file_prefix += "VND_";

            bl_tabu = new Vnd();
        }
        // ----------- TABU com ILS -----------
        else if (strcmp(tipo_bl_tabu, "ILS") == 0)
        {
            file_prefix += "ILS_";

            int iter_bl_aux = atoi(argv[7]);

            int iter_bl_ils_tabu = 100; // FIXAR BASEADO EM TESTES GRASP
            bl_ils_tabu = new Vns(iter_bl_ils_tabu);

            bl_tabu = new Ils(iter_bl_aux, bl_ils_tabu);
        }

        logger->inicia_logger(file_prefix);

        BuscaLocal* bt = new BuscaTabu(iter_tabu, tamanho_lista, bl_tabu);
        bt->registra_logger(logger);
        solucao_final = bt->executa(s);

        delete bt;

        delete bl_tabu;
        if (bl_ils_tabu != nullptr) delete bl_ils_tabu;
    }

    solucao_final.imprime();
    Validador().valida(ins, solucao_final);

    delete ins;

    logger->salva_resultado_final(solucao_final.get_custo());
    logger->finaliza_logger();

    return EXIT_SUCCESS;
}