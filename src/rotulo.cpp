#include "rotulo.h"

Rotulo::Rotulo(int id) {
    this->id = id;
    this->frequencia = 0;
}

int Rotulo::get_frequencia() {
    return frequencia;
}