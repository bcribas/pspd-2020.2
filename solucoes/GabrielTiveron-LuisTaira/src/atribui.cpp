#include "../inc/atribui.hpp"
#include<iostream>

using namespace std;

void para_verdadeiro(int var, Full * full, int ind_flip) {
  int index = to_index(var);
  full->vars[index].valor = true;
  int len = full->vars[index].index;
  for(int i = 0; i < len; i++){
    int _index = full->vars[index].clausulas[i];
    full->clausulas[_index].valor = true;
    full->clausulas[_index].last_update = ind_flip;
  }
}

void para_falso(int var, Full * full,int ind_flip) {
  int index = to_index(var);
  full->vars[index].valor = false;
  int len = full->vars[index].index;
  for(int i = 0; i < len; i++){
    int _index = full->vars[index].clausulas[i];
    if(full->clausulas[_index].last_update != ind_flip)
      verifica_clausula(&full->clausulas[_index], full->vars);
  }
}

void flip_variavel(int var, to_print *prnt, Full* full, int ind_flip) {
  if(full->vars[to_index(var)].valor == true) {
    para_verdadeiro(var *-1, full,ind_flip);
    para_falso(var, full,ind_flip);
  } else {
    para_verdadeiro(var, full,ind_flip);
    para_falso(var * -1, full,ind_flip);
  }
  verifica_formula(prnt, full);
}
