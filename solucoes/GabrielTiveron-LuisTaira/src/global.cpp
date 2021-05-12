#include "../inc/global.hpp"

int to_index(int t) {
  return t > 0 ? (t - 1) : (numero_variaveis + (-1 * t) - 1);
}

int from_index(int t){
  return (t+1) > numero_variaveis ? (t - numero_variaveis) + (t - numero_variaveis) * (-2) -1 : t+1;
}

vector<Variavel> variaveis;
vector<Clausula> clausulas;
vector<Full> fulls;
vector<Metadata*> metadata;

int numero_clausulas, numero_variaveis;
