#ifndef LOGGER
#define LOGGER

#include <chrono>
#include <iostream>
#include <fstream>

using namespace std;

class Logger {
    private:
        /* Here will be the instance stored. */
        static Logger* instance;
        /* Private constructor to prevent instancing. */
        Logger();

        std::chrono::time_point<std::chrono::high_resolution_clock> start;
        fstream logfile;
        fstream tttfile;
        fstream results;

        long double get_elapsed_time();
    public:
        /* Static access method. */
        static Logger* get_instance();

        void inicia_logger(string file_prefix);
        void finaliza_logger();

        void salva_resultado_final(int custo);
        void salva_resultado_parcial(int iter, int custo);
        void salva_ttt();
};



#endif