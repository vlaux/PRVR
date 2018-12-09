#include <iostream>
#include <fstream>
#include <string>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

using namespace std;

int main(int argc, char *argv[])
{
    string path = "output/results";
    string log_path = "output";

    string prefix = argv[1];

    for (int i = 1; i <= 7; i++)
    {
        for (int j = 1; j <= 3; j++)
        {
            string curr_ins = "prvr-" + to_string(i) + "-" + to_string(j) + "_" + prefix;
            string nome_arquivo;

            for (const auto &p : fs::directory_iterator(path))
            {
                nome_arquivo = p.path();

                float tempo;
                int custo;

                if (nome_arquivo.find(curr_ins) != string::npos)
                {
                    std::ifstream arquivo(nome_arquivo, ios::in);

                    int best_custo = INT32_MAX;
                    int sum_custo = 0;
                    float best_tempo = INT32_MAX;
                    float sum_tempo = 0;
                    float count = 0;

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

                    cout << "===============================" << endl;
                    cout << curr_ins << endl;
                    cout << "--------" << endl;
                    cout << "Melhor custo:\t\t" << best_custo << endl;
                    cout << "Custo médio:\t\t" << sum_custo / count << endl;
                    cout << "Melhor tempo:\t\t" << best_tempo << endl;
                    cout << "Tempo médio:\t\t" << sum_tempo / count << endl;

                    arquivo.close();
                }
            }

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
                if (nome_log.find(curr_ins) != string::npos)
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
                            int diferenca = curr_iter - last_iter;
                            sum_intervalos += diferenca;

                            if (diferenca > maior_intervalo)
                                maior_intervalo = diferenca;
                        }

                        last_iter = curr_iter;
                    }

                    arquivo_log.close();
                }
            }

            cout << "Maior iteração:\t\t" << maior_iteracao << endl;
            cout << "Maior intervalo:\t" << maior_intervalo << endl;
            cout << "Média intervalo:\t" << sum_intervalos / count_intervalos << endl;
        }
    }

    return EXIT_SUCCESS;
}
