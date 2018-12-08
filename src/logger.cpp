#include "logger.h"
#include <sstream>

Logger* Logger::instance = 0;

Logger* Logger::get_instance()
{
    if (Logger::instance == 0)
        instance = new Logger();

    return instance;
}

// retorna tempo em segundos
long double Logger::get_elapsed_time()
{
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = now - start;
    return elapsed.count();
}

void Logger::inicia_logger(list<string> call_args) 
{
    std::stringstream ss;
    string instance_name = call_args.front();
    call_args.pop_front();
    ss << instance_name.substr(4, instance_name.length() - 8);
    for (string arg: call_args) ss << "_" << arg;
    string file_prefix = ss.str();

    auto now = std::chrono::high_resolution_clock::now();
    string timestamp = to_string(now.time_since_epoch().count());

    string logfile_name = "output/" + file_prefix + "_" + timestamp + "_log.txt";
    string resultsfile_name = "output/resultados/" + file_prefix + ".txt";

    logfile.open(logfile_name, fstream::out);
    results.open(resultsfile_name, fstream::app);
}

void Logger::salva_resultado_parcial(int iter, int custo)
{
    logfile << iter << "\t" << get_elapsed_time() << "\t" << custo << endl;
}

void Logger::salva_resultado_final(int custo)
{
    results << get_elapsed_time() << "\t" << custo << endl;
}

void Logger::finaliza_logger()
{
    logfile.close();
    results.close();

    #ifdef DEBUG
    cout << "Arquivos de log gravados" << endl;
    #endif
}

Logger::Logger() {
    start = std::chrono::high_resolution_clock::now();
}
