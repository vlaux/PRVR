#ifndef LOGGER
#define LOGGER

#include <chrono>
#include <iostream>
#include <fstream>
#include <list>

using namespace std;

class Logger {
    private:
        /* Here will be the instance stored. */
        static Logger* instance;
        /* Private constructor to prevent instancing. */
        Logger();

        std::chrono::time_point<std::chrono::high_resolution_clock> start;
        fstream logfile;
        fstream results;

    public:
        /* Static access method. */
        static Logger* get_instance();

        void inicia_logger(list<string> call_args);
        void finaliza_logger();

        void salva_resultado_final(int custo);
        void salva_resultado_parcial(int iter, int custo);
        long double get_elapsed_time();
};



#endif