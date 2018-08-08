#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <list>

using namespace std;

typedef std::tuple<int, std::vector<int>> Movimento; // <id_rota, lista de clientes>

int main() 
{
    vector<int> vet = {1,2};
    Movimento mov1 = std::make_tuple(1, vet);

    vector<int> vet2 = {1,3};
    Movimento mov2 = std::make_tuple(1, vet2);

    std::list<Movimento> lista;
    lista.push_back(mov1);

    if (find(lista.begin(), lista.end(), mov2) != lista.end())
        cout << "acheeeeei" << endl;

    cout << "fim " << endl;
    return 0;
}