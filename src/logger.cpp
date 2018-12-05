#include "logger.h"

Logger* Logger::instance = 0;

Logger* Logger::get_instance()
{
    if (Logger::instance == 0)
        instance = new Logger();

    return instance;
}

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

void Logger::finaliza_logger()
{
    logfile.close();
    tttfile.close();
    results.close();

    cout << "Arquivos de log gravados" << endl;
}

Logger::Logger() {
    start = std::chrono::high_resolution_clock::now();
}