#include <bits/stdc++.h>
#include <iostream>
#include <stack>
#include <string>

using namespace std;

int numero_variaveis, numero_clausulas;
vector<vector<int>> clausulas;
vector<bool> mudancas;
vector<int> clausulas_verdadeiras;
set<int> clausulas_falsas;
map<int, int> literais_errados_total;
map<int,vector<int>>variaveis_clausulas;
int literal_flip;
int indice_threads=0;
int indice_threads_flip=0;

void verifica_flip() {
    vector <int> v1;
    vector <int> v2;
    for( auto valor:variaveis_clausulas[literal_flip]) {
        v1.push_back(valor);
    }
    for( auto valor:variaveis_clausulas[-literal_flip]) {
        v2.push_back( valor );
    }

    while(indice_threads<v1.size()) {
        int index = indice_threads++;
        int valor = v1[index];
        if(mudancas[literal_flip] == true){
            if(clausulas_verdadeiras[valor] == 0){
                for(int i=0;i<clausulas[valor].size();i++){
                    literais_errados_total[clausulas[valor][i]]--;
                    if(literais_errados_total[clausulas[valor][i]]<=0){
                        literais_errados_total.erase(clausulas[valor][i]);
                    }
                }
                clausulas_falsas.erase(valor);
            }
            clausulas_verdadeiras[valor]++;
        } else if(mudancas[literal_flip] == false){
            if(clausulas_verdadeiras[valor] == 1) {
                for(int i=0;i<clausulas[valor].size();i++){
                    literais_errados_total[clausulas[valor][i]]++;
                }
                clausulas_falsas.insert(valor);
            }
            clausulas_verdadeiras[valor]--;
        }
    }
    while(indice_threads_flip<v2.size()) {
        int index = indice_threads_flip++;
        int valor = v2[index];
        if(mudancas[literal_flip] == true){
            if(clausulas_verdadeiras[valor] == 1) {
                for(int i=0;i<clausulas[valor].size();i++){
                    literais_errados_total[clausulas[valor][i]]++;
                }
                clausulas_falsas.insert(valor);
            }
            clausulas_verdadeiras[valor]--;
        } else if(mudancas[literal_flip] == false){
            if(clausulas_verdadeiras[valor] == 0){
                for(int i=0;i<clausulas[valor].size();i++){
                    literais_errados_total[clausulas[valor][i]]--;
                    if(literais_errados_total[clausulas[valor][i]]<=0){
                        literais_errados_total.erase(clausulas[valor][i]);
                    }
                }
                clausulas_falsas.erase(valor);
            }
            clausulas_verdadeiras[valor]++;
        }
    }
}

void verifica_full() {
    while(indice_threads<clausulas.size()) {
		int i = indice_threads++;
		for(auto valor:clausulas[i]) {
            if ((valor > 0 and mudancas[abs(valor)] == true) || (valor < 0 and mudancas[abs(valor)] == false)) {
                clausulas_verdadeiras[i]++;
            }
		}
		if(clausulas_verdadeiras[i] == 0) {
			clausulas_falsas.insert(i);
			for(auto valor:clausulas[i]) {
				literais_errados_total[valor]++; 
            }
		}
    }
}

void imprimir_lista_literais_errados()
{
    if(clausulas_falsas.empty()) {
        cout << "SAT\n";
    } else {
        vector<pair<int, int>> aux_errados;
        for(auto x:literais_errados_total) {
            if(x.second>0) 
                aux_errados.emplace_back(x.first, x.second);
        }
        sort(aux_errados.begin(), aux_errados.end(), [](auto &left, auto &right) {
            if (left.second == right.second){
                return abs(left.first) > abs(right.first);	
            }
            else {
                return left.second > right.second;
            }
        });
        cout << "[" << clausulas_falsas.size() << " clausulas falsas]"; 
        for(auto x:clausulas_falsas)
            cout << ' ' << x;
        cout << '\n';
        cout << "[lits]";
        for(auto x:aux_errados) {
            cout << " " << x.first;
        }
        cout << '\n';
    }
}

int main()
{
    cin >> numero_variaveis >> numero_clausulas;
    clausulas_verdadeiras = vector<int>(numero_clausulas, 0);
    for(int i=0; i<numero_clausulas; i++) {
        int valor;
        vector<int> vetor_clausula;
        while (cin >> valor, valor != 0) {
            vetor_clausula.push_back(valor);
            
            variaveis_clausulas[valor].push_back(i);
        }
        clausulas.push_back(vetor_clausula);
    }	

    string comando;
    while(cin >> comando) {
        int valor;
		indice_threads = 0;
        indice_threads_flip = 0;
        if(comando.compare("full") == 0) {
            literais_errados_total.clear();
            clausulas_verdadeiras.assign(numero_clausulas, 0);;
            clausulas_falsas.clear();
            mudancas.assign(numero_variaveis, false);
            for(int i=0; i<numero_variaveis; i++) {
                cin >> valor;
                if (valor > 0) {
                    mudancas[abs(valor)] = true;
                } else {
                    mudancas[abs(valor)] = false;
                }
            }
            verifica_full();
        }
        else{
            cin >> literal_flip;
            mudancas[abs(literal_flip)] = !mudancas[abs(literal_flip)];
            verifica_flip();
        }
        imprimir_lista_literais_errados();
    }
}