#ifndef ROTULO
#define ROTULO

#include "utils.h"

class Rotulo {
    public:
        Rotulo(int id);
        int id;
        int frequencia;
        int get_frequencia();
        bool operator < (const Rotulo& r) const
        {
            return (frequencia < r.frequencia);
        }
        bool operator > (const Rotulo& r) const
        {
            return (frequencia > r.frequencia);
        }
};

#endif