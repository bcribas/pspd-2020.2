#ifndef _GOBAL_
#define _GOBAL_
#include <vector>
#include <unordered_map>
#include <map>
#include <utility>

#define MAX_CLAUSULA 134217728
#define MAX_VAR 1048576

using namespace std;

struct Variavel;
struct Clausula;

typedef struct Variavel {
  vector<int> clausulas;
  int qtd_clausulas_falsas = 0;
  bool valor = false;
  int index = 0;
} Variavel;

typedef struct Clausula {
  vector<int> variaveis;
  bool valor;
  int index = 0;
  int last_update;
} Clausula;

typedef struct Flip{
  int var;
}Flip;

typedef struct full {
  vector<Flip> flips;
  vector<Variavel> vars;
  vector<Clausula> clausulas;
  int qtd_clausulas_falsas;
}Full;

typedef struct to_print{
  int qtd_clauses;
  vector<int> clauses;
  vector<pair<int, int>> lits;
  bool sat = false;
  int index = 0;
}to_print;

typedef struct metadata{
  vector<to_print*> mtdt;
  int indexes;
}Metadata;

extern int to_index(int t);
extern int from_index(int t);

extern vector<Variavel> variaveis;
extern vector<Clausula> clausulas;
extern vector<Full> fulls;
extern vector<Metadata*> metadata;

extern int numero_clausulas, numero_variaveis;

#endif
