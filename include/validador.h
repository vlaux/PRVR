#ifndef VALIDADOR
#define VALIDADOR

#include "solucao.h"
#include "instancia.h"

class Validador {
    public:
        bool valida(Instancia& ins, Solucao& s);
};

#endif