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
    string logfile_name = "/output/" + file_prefix + "._log.txt";
    string tttfile_name = "/output/" + file_prefix + "._ttt.txt";
    string resultsfile_name = "/output/resultados/" + file_prefix + ".txt";

    logfile.open(logfile_name, ios::out);
    tttfile.open(tttfile_name, ios::out);
    results.open(resultsfile_name, ios::app);
}

Logger::Logger() {
    start = std::chrono::high_resolution_clock::now();
}