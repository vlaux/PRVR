#include "logger.h"

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

void Logger::inicia_logger(string file_prefix) 
{
    auto now = std::chrono::high_resolution_clock::now();

    string timestamp = to_string(now.time_since_epoch().count());

    string logfile_name = "output/" + file_prefix + timestamp + "_log.txt";
    string tttfile_name = "output/" + file_prefix + timestamp + "_ttt.txt";
    string resultsfile_name = "output/resultados/" + file_prefix + ".txt";

    logfile.open(logfile_name, fstream::out);
    tttfile.open(tttfile_name, fstream::out);
    results.open(resultsfile_name, fstream::app);
}

void Logger::salva_resultado_parcial(int iter, int custo)
{

    logfile << iter << "\t" << get_elapsed_time() << "\t" << custo << endl;

    tttfile << get_elapsed_time() << endl;
}

void Logger::salva_resultado_final(int custo)
{
    results << get_elapsed_time() << "\t" << custo << endl;
}

void Logger::finaliza_logger()
{
    logfile.close();
    tttfile.close();
    results.close();

    #ifdef DEBUG
    cout << "Arquivos de log gravados" << endl;
    #endif
}

Logger::Logger() {
    start = std::chrono::high_resolution_clock::now();
}
