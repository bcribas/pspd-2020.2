#include "../inc/scan.hpp"

void scan_clausulas() {
  for(int i = 0; i < 2*numero_variaveis; i++){
   variaveis.push_back(Variavel());
  }
  for (int i = 0; i < numero_clausulas; i++) {
    clausulas.push_back(Clausula());
    int valor_variavel;
    while ((cin >> valor_variavel) && (valor_variavel != 0)) {
      int indice_variavel = to_index(valor_variavel);
      Variavel * v = &variaveis[indice_variavel];
      clausulas[i].variaveis.push_back(indice_variavel);
      v->clausulas.push_back(i);
      v->index++;
      clausulas[i].index++;
    }
  }
}
