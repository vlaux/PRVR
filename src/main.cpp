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

#define ITER_VNS 50

Logger *logger = Logger::get_instance();

int main(int argc, char *argv[])
{
    cout << "Iniciando execução PRVR" << endl;

    srand(time(NULL));

    char *nome_arquivo = argv[1];

    list<string> call_args;
    for (int i = 1; i < argc; i++) call_args.push_back(argv[i]);

    logger->inicia_logger(call_args);

    Instancia *ins = le_arquivo(nome_arquivo);

    Solucao solucao_final(ins);

    char *metaheuristica = argv[2];

    // --------------- GRASP ---------------
    if (strcmp(metaheuristica, "GRASP") == 0)
    {
        float alpha = atof(argv[3]);
        int iter_grasp = atoi(argv[4]);

        BuscaLocal *bl = nullptr;
        BuscaLocal *bl_aux = nullptr;

        char *metodo_bl = argv[5];

        // ----------- com VNS -----------
        if (strcmp(metodo_bl, "VNS") == 0)
        {
            // int iter_vns = atoi(argv[6]);
            bl = new Vns(ITER_VNS);
        }

        // ----------- com ILS -----------
        else if (strcmp(metodo_bl, "ILS") == 0)
        {
            int iter_ils = atoi(argv[6]);
            bl_aux = new Vns(ITER_VNS); // baseado em testes
            bl = new Ils(iter_ils, bl_aux);
        }

        // ----------- com VND -----------
        else if (strcmp(metodo_bl, "VND") == 0)
        {
            bl = new Vnd();
        }

        // ----------- com TABU -----------
        else if (strcmp(metodo_bl, "TABU") == 0)
        {
            int iter_tabu = atoi(argv[6]);
            int tamanho_lista = atoi(argv[7]);
            char *bl_tabu = argv[8];

            // ----------- TABU com VNS -----------
            if (strcmp(bl_tabu, "VNS") == 0)
            {
                bl_aux = new Vns(ITER_VNS);
            }
            // ----------- TABU com VND -----------
            else if (strcmp(bl_tabu, "VND") == 0)
            {
                bl_aux = new Vnd();
            }
            bl = new BuscaTabu(iter_tabu, tamanho_lista, bl_aux);
        }

        bool is_reativo = alpha < 0;

        solucao_final = Grasp(is_reativo, iter_grasp, alpha, bl, logger).executa(ins);
        
        delete bl;
        if (bl_aux != nullptr) delete bl_aux;
    }

    // --------------- VNS ---------------
    else if (strcmp(metaheuristica, "VNS") == 0)
    {
        int iter_vns = atoi(argv[3]);
        char* modo_construcao = argv[4];

        Solucao s(ins);
        if (strcmp(modo_construcao, "GUL") == 0)
            s = Construtor(ins).construcao_gulosa();

        if (strcmp(modo_construcao, "AL") == 0)
            s = Construtor(ins).construcao_aleatoria();

        if (strcmp(modo_construcao, "GUL_AL") == 0)
        {
            float alpha_construcao = atof(argv[5]);
            s = Construtor(ins).construcao_gulosa_aleatoria(alpha_construcao);
        }

        BuscaLocal* vns = new Vns(iter_vns);
        vns->registra_logger(logger);
        solucao_final = vns->executa(s);

        delete vns;
    }

    // --------------- ILS ---------------
    else if (strcmp(metaheuristica, "ILS") == 0)
    {
        BuscaLocal* bl = nullptr;

        int iter_ils = atoi(argv[3]);
        char* modo_construcao = argv[4];
        char* tipo_bl = argv[5];

        Solucao s(ins);
        if (strcmp(modo_construcao, "GUL") == 0)
            s = Construtor(ins).construcao_gulosa();

        else if (strcmp(modo_construcao, "AL") == 0)
            s = Construtor(ins).construcao_aleatoria();

        else if (strcmp(modo_construcao, "GUL_AL") == 0)
        {
            float alpha_construcao = 0.8; // FIXAR BASEADO EM TESTES GRASP
            s = Construtor(ins).construcao_gulosa_aleatoria(alpha_construcao);
        }

        // ----------- ILS com VNS -----------
        if (strcmp(tipo_bl, "VNS") == 0)
        {
            bl = new Vns(ITER_VNS);
        }
        // ----------- ILS com VND -----------
        else if (strcmp(tipo_bl, "VND") == 0)
            bl = new Vnd();

        BuscaLocal* ils = new Ils(iter_ils, bl);
        ils->registra_logger(logger);
        solucao_final = ils->executa(s);

        delete ils;
    }

    // --------------- TABU ---------------
    else if (strcmp(metaheuristica, "TABU") == 0)
    {
        int iter_tabu = atoi(argv[3]);
        int tamanho_lista = atoi(argv[4]);
        char* modo_construcao = argv[5];
        char* tipo_bl_tabu = argv[6];
        BuscaLocal* bl_tabu = nullptr;
        BuscaLocal* bl_ils_tabu = nullptr;

        Solucao s(ins);
        if (strcmp(modo_construcao, "GUL") == 0)
            s = Construtor(ins).construcao_gulosa();

        else if (strcmp(modo_construcao, "AL") == 0)
            s = Construtor(ins).construcao_aleatoria();

        else if (strcmp(modo_construcao, "GUL_AL") == 0)
        {
            float alpha_construcao = 0.8; // FIXAR TESTES GRASP
            s = Construtor(ins).construcao_gulosa_aleatoria(alpha_construcao);
        }

        // ----------- TABU com VNS -----------
        if (strcmp(tipo_bl_tabu, "VNS") == 0)
        {
            bl_tabu = new Vns(ITER_VNS);
        }
        // ----------- TABU com VND -----------
        else if (strcmp(tipo_bl_tabu, "VND") == 0)
            bl_tabu = new Vnd();

        // ----------- TABU com ILS -----------
        else if (strcmp(tipo_bl_tabu, "ILS") == 0)
        {
            int iter_bl_aux = atoi(argv[7]);

            bl_ils_tabu = new Vns(ITER_VNS);

            bl_tabu = new Ils(iter_bl_aux, bl_ils_tabu);
        }

        BuscaLocal* bt = new BuscaTabu(iter_tabu, tamanho_lista, bl_tabu);
        bt->registra_logger(logger);
        solucao_final = bt->executa(s);

        delete bt;

        delete bl_tabu;
        if (bl_ils_tabu != nullptr) delete bl_ils_tabu;
    }

    #ifdef DEBUG
    cout << "Solução final:" << endl;
    solucao_final.imprime();
    #endif

    Validador().valida(ins, solucao_final);

    delete ins;

    logger->salva_resultado_final(solucao_final.get_custo());
    logger->finaliza_logger();

    cout << "Finalizado em " << logger->get_elapsed_time() << " s" << endl;

    return EXIT_SUCCESS;
}