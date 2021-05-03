#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <cstring>
#include <algorithm>
#include <unordered_set>
#include <set>
#include <iterator>

#define LITS true
#define PRINT true
#define PRINT_RESULT true

// g++ -std=c++17 -O2 main.cpp -o prog

using namespace std;

int n_variaveis, n_clausulas;
vector<vector<int> > clausulas;
vector<vector<bool> > l_clausulas;
unordered_set<int> idx_clausulas_nao_satisfeitas;
#if LITS
vector<int> literais_falsos;
#endif
vector<bool> valores;

void le_entrada();
void le_comandos();
void executa_full();
void executa_flip();
void imprime_resultado();

bool compara(pair<int, int>& a, pair<int, int>& b) {
  if (a.second == b.second) {
    return abs(a.first) > abs(b.first);
  }
  return a.second > b.second;
}

int main() {
  
  le_entrada();
  le_comandos();

  return 0;
}

void le_entrada() {
  scanf("%d", &n_variaveis);
  scanf("%d", &n_clausulas);

  l_clausulas.resize(n_variaveis, vector<bool>(n_clausulas));
  valores.resize(n_variaveis, false);
  #if LITS
  literais_falsos.resize(n_variaveis, 0);
  #endif

  for (int i = 0; i < n_clausulas; i++) {
    vector<int> literais;
    int num;
    do {
      scanf("%d", &num);
      if (num != 0) {
        literais.push_back(num);
        l_clausulas[abs(num) - 1][i] = true;
      }
    } while (num != 0);
    clausulas.push_back(literais);
    literais.clear();
  }
}

void le_comandos() {
  char comando[4];
  while(scanf("%4s", comando) != EOF) {
    if (strcmp(comando, "full") == 0) {
      executa_full();
    }
    if (strcmp(comando, "flip") == 0) {
      executa_flip();
    }
    #if PRINT_RESULT
    imprime_resultado();
    #endif
  }
}

void executa_full() {
  idx_clausulas_nao_satisfeitas.clear();
  literais_falsos.clear();

  int valor;
  for (int i = 0; i < n_variaveis; i++) {
    scanf("%d", &valor);
    valores[abs(valor) - 1] = valor > 0 ? true : false;
  }

  for (int i = 0; i < n_clausulas; i++) {
    bool satisfaz = false;
    for (int j = 0; j < clausulas[i].size(); j++) {
      int base = clausulas[i][j];
      int valor = valores[abs(base) - 1] ? base : base * -1;
      
      if (valor > 0) {
        satisfaz = true;
        break;
      }
    }
    if (!satisfaz) {
      idx_clausulas_nao_satisfeitas.insert(i);
      #if LITS
      for (int j = 0; j < clausulas[i].size(); j++) {
        int base = clausulas[i][j];
        int valor = valores[abs(base) - 1] ? base : base * -1;
        if (valor < 0) {
          literais_falsos[abs(base) - 1] += 1;
        }
      }
      #endif
    }
  }
}

void executa_flip() {
  int valor;
  scanf("%d", &valor);
  valores[abs(valor) - 1] = !valores[abs(valor) - 1];

  for (int i = 0; i < n_clausulas; i++) {
    if (!l_clausulas[abs(valor) - 1][i]) {
      continue;
    }
    bool satisfaz = false;
    for (int j = 0; j < clausulas[i].size(); j++) {
      int base = clausulas[i][j];

      int valor = valores[abs(base) - 1] ? base : base * -1;
      
      if (valor > 0) {
        satisfaz = true;
        break;
      }
    }
    if (satisfaz) {
      if (idx_clausulas_nao_satisfeitas.find(i) != idx_clausulas_nao_satisfeitas.end()) {
        #if LITS
        for (int j = 0; j < clausulas[i].size(); j++) {
          int base = clausulas[i][j];
          bool flip = valores[abs(base) - 1];
          if (abs(base) == valor) {
            flip = !flip;
          }
          int valor = flip ? base : base * -1;
          if (valor < 0) {
            literais_falsos[abs(base) - 1] -= 1;
          }
        }
        #endif
        idx_clausulas_nao_satisfeitas.erase(i);
      }
    } else {
      if (idx_clausulas_nao_satisfeitas.find(i) == idx_clausulas_nao_satisfeitas.end()) {
        idx_clausulas_nao_satisfeitas.insert(i);
        #if LITS
        for (int j = 0; j < clausulas[i].size(); j++) {
          int base = clausulas[i][j];
          int valor = valores[abs(base) - 1] ? base : base * -1;
          if (valor < 0) {
            literais_falsos[abs(base) - 1] += 1;
          }
        }
        #endif
      }
    }
  }
}

void imprime_resultado() {
  if (idx_clausulas_nao_satisfeitas.empty()) {
    #if PRINT
    printf("SAT\n");
    #endif
  } else {
    int size = idx_clausulas_nao_satisfeitas.size();
    #if PRINT
    printf("[%d clausulas falsas]", size);
    #endif
    set<int> ordered(idx_clausulas_nao_satisfeitas.begin(), idx_clausulas_nao_satisfeitas.end());
    for (const auto& element: ordered) {
      #if PRINT
      printf(" %d", element);
      #endif
    }
    #if PRINT
    printf("\n");
    #endif
    #if LITS
    #if PRINT
    printf("[lits]");
    #endif
    vector<pair<int, int>> lits;
    for (int i = 0; i < n_variaveis; i++) {
      if (literais_falsos[i] > 0) {
        int literal = (i + 1) * (valores[i] ? 1 : -1) * -1;
        int contador = literais_falsos[i];
        lits.push_back(make_pair(literal, contador));
      }
    }
    sort(lits.begin(), lits.end(), compara);
    for (int i = 0; i < lits.size(); i++) {
      #if PRINT
      printf(" %d", lits[i].first);
      #endif
    }
    #if PRINT
    printf("\n");
    #endif
    #endif
  }
}
