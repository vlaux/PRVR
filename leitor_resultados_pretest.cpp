#include <iostream>
#include <fstream>
#include <string>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

using namespace std;

// Compilar com -std=c++17 -lstdc++fs
int main(int argc, char *argv[])
{
    string results_path = argv[1];
    string log_path = argv[2];

    int maior_intervalo_geral = -1;
    int sum_intervalo_geral = 0;
    float count_intervalo_geral = 0;

    for (const auto &p : fs::directory_iterator(results_path))
    {
        auto path = p.path();
        string path_arquivo = p.path();
        string nome_arquivo_completo = path.stem();
        string estrategia = nome_arquivo_completo.substr(0, nome_arquivo_completo.length() - 4);

        std::ifstream arquivo(path_arquivo, ios::in);

        int best_custo = INT32_MAX;
        int sum_custo = 0;
        float best_tempo = INT32_MAX;
        float sum_tempo = 0;
        float count = 0;

        float tempo;
        int custo;

        while (arquivo >> tempo)
        {
            arquivo >> custo;

            if (tempo < best_tempo)
                best_tempo = tempo;
            if (custo < best_custo)
                best_custo = custo;

            count += 1;
            sum_custo += custo;
            sum_tempo += tempo;
        }

        std::cout << "===============================" << endl;
        std::cout << estrategia << endl;
        std::cout << "--------" << endl;
        std::cout << "Melhor custo:\t\t" << best_custo << endl;
        std::cout << "Custo médio:\t\t" << sum_custo / count << endl;
        std::cout << "Melhor tempo:\t\t" << best_tempo << endl;
        std::cout << "Tempo médio:\t\t" << sum_tempo / count << endl;

        arquivo.close();


        int maior_intervalo = -1;
        int maior_iteracao = -1;
        int sum_intervalos = 0;
        float count_intervalos = 0;

        for (const auto &log : fs::directory_iterator(log_path))
        {
            int last_iter;
            int curr_iter = -1;
            float _tempo;
            int _valor;

            string nome_log = log.path();
            if (nome_log.find(estrategia) != string::npos)
            {
                std::ifstream arquivo_log(nome_log, ios::in);
                last_iter = -1;

                while (arquivo_log >> curr_iter)
                {
                    arquivo_log >> _tempo;
                    arquivo_log >> _valor;

                    if (curr_iter > maior_iteracao)
                        maior_iteracao = curr_iter;

                    if (last_iter > 0)
                    {
                        count_intervalos += 1;
                        count_intervalo_geral += 1;

                        int diferenca = curr_iter - last_iter;
                        sum_intervalos += diferenca;
                        sum_intervalo_geral += diferenca;

                        if (diferenca > maior_intervalo)
                            maior_intervalo = diferenca;
                        if (diferenca > maior_intervalo_geral)
                            maior_intervalo_geral = diferenca;
                    }

                    last_iter = curr_iter;
                }

                arquivo_log.close();
            }
        }

        std::cout << "Maior iteração:\t\t" << maior_iteracao << endl;
        std::cout << "Maior intervalo:\t" << maior_intervalo << endl;
        std::cout << "Média intervalo:\t" << sum_intervalos / count_intervalos << endl;
    }

    std::cout << "===============================" << endl;

    std::cout << "Maior intervalo geral: " << maior_intervalo_geral << endl;
    std::cout << "Média intervalo geral: " << sum_intervalo_geral / count_intervalo_geral << endl;

    return EXIT_SUCCESS;
}
