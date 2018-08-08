// #include <iostream>
// #include "busca_tabu.h"
// #include "instancia.h"
// #include "grasp.h"
// #include "lista_tabu.h"
// #include "vnd.h"

// using namespace std;

// BuscaTabu::BuscaTabu() {}

// BuscaTabu::BuscaTabu(char* busca_local, int k_max_bl)
// {
//     this->tipo_busca_local = busca_local;
//     this->k_max_bl = k_max_bl;
// }

// Solucao BuscaTabu::executa(Instancia &ins, Solucao sol_inicial, int max_iter)
// {
//     Solucao s_best(ins);
//     Solucao s(ins);
//     s = sol_inicial;

//     lista_tabu = new ListaTabu(tam_maximo_lista); // vai ser definido por algum param da instância    

//     int iter_sem_melhora = 0, iter = 0;
//     while(iter_sem_melhora < max_iter)
//     {
//         s = busca_local(s, ins);

//         if (s.get_custo() < s_best.get_custo()) 
//         {
//             s_best = s;
//             iter_sem_melhora = 0;
//         }
//         else
//         {
//             s = s_best;
//             iter_sem_melhora++;
//         }

//         iter++;
//         avalia_tamanho_lista_tabu(s_best, iter);
//     }

//     delete lista_tabu;

//     return s;
// }

// void BuscaTabu::avalia_tamanho_lista_tabu(Solucao s_best, int iter)
// {
//     if (iter % iter_until_update == 0) 
//     {
//         if (s_best.get_custo() < ultimo_custo_avaliado)
//             lista_tabu->diminui_lista();

//         else
//             lista_tabu->aumenta_lista();
//     }
// }

// Solucao BuscaTabu::busca_local(Solucao s, Instancia ins) {
//     if (strcmp(tipo_busca_local, "VND") == 0) {
//         return Vnd(lista_tabu).executa(s, ins, k_max_bl);
//     }
//     else if (strcmp(tipo_busca_local, "VNS") == 0) {
//         cout << "vns para tabu não implementado" << endl;
//         abort();
//     }
//     else {
//         cout << "deveria executar movs aleatórios na Tabu: não implementado" << endl;
//         abort();
//     }
//     return s;
// }