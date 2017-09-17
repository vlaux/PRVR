#include <iostream>
#include "instancia.h"
#include "leitor_arquivos.h"

using namespace std;

int main() {
    srand(time(NULL));

    Instancia ins = le_arquivo("ins/c50r500i01.txt");

    return EXIT_SUCCESS;
}