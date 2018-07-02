#include <vector>
#include <math.h>
#include <assert.h>
#include <iostream>
#include <fstream>

using namespace std;

const float A = (sqrt(5) - 1)/2;

int get_rotulo(int i, int j, int n_rotulos) {
    i = i+1; j = j+1;
    int rotulo = floor(n_rotulos*(i*j*A - floor(i*j*A)));
    assert(rotulo < n_rotulos);
    return rotulo;
}

int main(int argc, char *argv[]) {

    char *nome_arquivo = argv[1];
    std::ifstream leitura(nome_arquivo, ios::in);
    std::ofstream arquivo(nome_arquivo, ios::app);

    if (!arquivo)
    {
        cerr << "Erro ao abrir instância " << nome_arquivo << endl;
        return 0;
    }
    int n_clientes, capacidade, n_rotulos = 0;

    leitura >> n_clientes >> capacidade >> n_rotulos;
    cout << "clientes: " << n_clientes << " | rotulos:" << n_rotulos << endl;

    int rotulos[n_clientes+1][n_clientes+1];

    for (int i = 0; i < n_clientes + 1; i++) {
        for (int j = 0; j < n_clientes + 1; j++) {
            if (i == j) 
                arquivo << -1 << " ";
            else 
                arquivo << get_rotulo(i, j, n_rotulos) << " ";
        }
        arquivo << "\n";
    }

    cout << "Salvando arquivo..." << endl;

    return 1;
}
