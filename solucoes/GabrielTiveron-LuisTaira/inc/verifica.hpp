#ifndef _VERIFICA_
#define _VERIFICA_
#include "../inc/global.hpp"
#include<iostream>
#include <algorithm>
#include <utility>

using namespace std;

bool verifica_clausula(Clausula *, vector<Variavel>);

void verifica_todas_as_clausula(to_print *prnt);

void reavalia_variavel(int var, to_print *prnt);

void verifica_formula(to_print *prnt, Full * full);

#endif
