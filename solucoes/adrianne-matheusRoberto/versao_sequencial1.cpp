#include <bits/stdc++.h>
#include <iostream>
#include <string>
using namespace std;

int numero_variaveis, numero_clausulas;

vector<set<int>> clausulas;
map<int, bool> mudancas;
vector<int> clausulas_verdadeiras;
set<int> clausulas_falsas;
map<int, int> literais_errados_total;

void verifica_flip(int literal) {
	// Para cada cláusula
    for(int i=0; i<numero_clausulas; i++) {
        int estado_inicial = clausulas_verdadeiras[i]; // Resgata qual total de verdadeiros na cláusula
		
        if(clausulas[i].find(literal) != clausulas[i].end() || clausulas[i].find(-literal) != clausulas[i].end()) { // Se encontrou o literal
            if(clausulas[i].find(literal) != clausulas[i].end() && mudancas[literal] == false) { // Se encontrou o literal absoluto e a mudança o deixa negativo
                clausulas_verdadeiras[i]--; // Remove da contagem de verdadeiros
            } else if(clausulas[i].find(-literal) != clausulas[i].end() && mudancas[-literal] == false) { // Senão se o literal negativo e a mudança aplicada o deixa falso
                clausulas_verdadeiras[i]--; // Remove da contagem de verdadeiros
            } else{
                clausulas_verdadeiras[i]++; // Se mudança não deixou falso, soma ao número de v's
            }
			
			// Se tinha literais positivos e depois das mudanças não tem
            if(estado_inicial!=0 and clausulas_verdadeiras[i] == 0) {
                clausulas_falsas.insert(i); // Adiciona à contagem de cláusulas falsas

				// Para cada literal da cláusula
                for(auto valor:clausulas[i]){ 
                    literais_errados_total[valor]++; // Conta nos literais totais errados
				}
            } else if(estado_inicial==0 and clausulas_verdadeiras[i] != 0) { // Se não tinha positivos e agora tem
                clausulas_falsas.erase(i); // Apaga do set de cláusulas falsas

                for(auto valor:clausulas[i]){ // Para cada literal da cláusula
                    literais_errados_total[valor]--; // Remove da contagem de literais errados
				}
            }
        }
    }
}

void verifica_full() {
	// Para cada cláusula
    for(int i=0; i<numero_clausulas; i++) {
        clausulas_verdadeiras[i] = 0; // Zera a contagem de positivos

		// Para os literais de cada cláusula
        for(auto valor:clausulas[i]) {
            if(mudancas[valor]) { // Se a mudança do literal for positiva
                clausulas_verdadeiras[i]++; // Soma na contagem de positivos da cláusula
            }
        }

		// Se no final não tem literal verdadeiro
        if(clausulas_verdadeiras[i] == 0) {
            clausulas_falsas.insert(i); // Cláusula falsa é guardada
            
			// Para os literais da cláusula analisada
			for(auto valor:clausulas[i]) { 
                literais_errados_total[valor]++; // Soma o valor de errados
			}
        }
    }
}

void imprimir_lista_literais_errados(){
    if(clausulas_falsas.empty()) { // Se não tiver nenhuma errada
        cout << "SAT\n";
    } else { // Se tiver alguma cláusula errada
        vector<pair<int, int>> aux_errados;

        for(auto x:literais_errados_total) { // Para cada literal errado de cada cláusula
            if(x.second>0){ // Se existe errados
				aux_errados.push_back({x.second, x.first}); // Guarda o valor
			}
        }

		// Ordena errados
        sort(aux_errados.begin(), aux_errados.end(), [](auto &left, auto &right) {
            if (left.first == right.first){
                return abs(left.second) > abs(right.second);	
            }
            else {
                return left.first > right.first;
            }
        });

        cout << "[" << clausulas_falsas.size() << " clausulas falsas]"; 
        for(auto x:clausulas_falsas)
            cout << ' ' << x;
        cout << '\n';
        cout << "[lits]";
        for(auto x:aux_errados) {
            cout << ' ' << x.second;
        }
        cout << '\n';
    }
}

int main(){	
	string comando;
	int x, valor;

	// Lê número de variáveis e número de cláusulas a proposição
    cin >> numero_variaveis >> numero_clausulas;

	// Inicializa clausulas e controle de cláusulas verdadeiras
    clausulas = vector<set<int>>(numero_clausulas);
    clausulas_verdadeiras = vector<int>(numero_clausulas);

	// Armazena cláusulas no vetor de elementos set
    for(int i=0; i<numero_clausulas; i++){        
        while (cin >> x, x != 0){
            clausulas[i].insert(x);
		}
    }
    
	// Lê os comandos full e flip 
    while(cin >> comando) {     
        if(comando.compare("full") == 0) { // Se for do tipo full
            literais_errados_total.clear(); // Zera map de pares index-valor de literais errados
            clausulas_falsas.clear(); // // Zera set de cláusulas falsas 

			// Lê todos os valores do full
            for(int i=0; i<numero_variaveis; i++) {
                cin >> valor;
                mudancas[valor] = true; // Armazena o valor positivo do literal 
                mudancas[-valor] = false; // Armazena o valor negativo do literal 
            }
            verifica_full(); // Chama função que aplica full
        } else{
            cin >> valor; // Lê valor que sofrerá mudança
            mudancas[valor] = !mudancas[valor]; // Armazena a mudança invertendo os valores
            mudancas[-valor] = !mudancas[-valor];
            verifica_flip(valor); // Chama função que aplica flip 
        }
        imprimir_lista_literais_errados(); // Imprime resposta final
    }
}