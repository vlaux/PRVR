#ifndef LEITOR_ARQUIVOS
#define LEITOR_ARQUIVOS

#include <iostream>
#include <fstream>
#include <vector>
#include "cliente.h"
using namespace std;

void le_arquivo(string nome_arquivo, std::vector<Cliente> &clientes, int &capacidade, int &n_clientes, int &n_rotulos, int **&rotulos);

#endif