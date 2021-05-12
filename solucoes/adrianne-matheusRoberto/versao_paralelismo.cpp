#include <bits/stdc++.h>
#include <pthread.h>
#include <iostream>
#include <stack>
#include <string>
#include <mutex>
#include <semaphore.h>

// #include <thread>
#define QTD_THREADS 4
using namespace std;

sem_t s_execucao[QTD_THREADS];
sem_t s_impressao[QTD_THREADS];
sem_t s_comando;
pthread_t threads[QTD_THREADS];
bool rodando = true;
bool comando_flip_full;


long int numero_variaveis, numero_clausulas;
vector<vector<long int>> clausulas;
vector<bool> mudancas;
vector<long int> clausulas_verdadeiras;
set<long int> clausulas_falsas;
map<long int, long int> literais_errados_total;
map<long int,vector<long int>>variaveis_clausulas;
mutex mtx, mtx1, mtx_bd, mtx3, mtx4, mtx5, mtx6, mtx10;
mutex mtx_literais_errados, mtx_index, mtx_index_flip, mtx_bd_clausulas, mtx_bd_literais, mtx_clausulas_verdadeiras;
long int literal_flip;
long int indice_threads=0;
long int indice_threads_flip=0;
vector <long int> v1;
vector <long int> v2;

void verifica_flip() {
    long int tamanho_vetor1 = v1.size();
    while(indice_threads<tamanho_vetor1) {
        mtx_index.lock();
        int index = indice_threads++;
        mtx_index.unlock();
        if(index<tamanho_vetor1) {
            long int valor = v1[index];
            if(mudancas[literal_flip]){
                mtx3.lock();
                if(clausulas_verdadeiras[valor] == 0){
                    mtx_literais_errados.lock();
                    for(long int i=0;i<clausulas[valor].size();i++){
                        literais_errados_total[clausulas[valor][i]]--;
                        if(literais_errados_total[clausulas[valor][i]]<=0){
                            mtx_bd_literais.lock();
                            literais_errados_total.erase(clausulas[valor][i]);
                            mtx_bd_literais.unlock();
                        }
                    }
                    mtx_literais_errados.unlock();
                    mtx_bd_clausulas.lock();
                    clausulas_falsas.erase(valor);
                    mtx_bd_clausulas.unlock();
                }
                mtx_clausulas_verdadeiras.lock();
                clausulas_verdadeiras[valor]++;
                mtx_clausulas_verdadeiras.unlock();
                mtx3.unlock();
            } else if(!mudancas[literal_flip]){
                mtx4.lock();
                if(clausulas_verdadeiras[valor] == 1) {
                    mtx_literais_errados.lock();
                    for(long int i=0;i<clausulas[valor].size();i++){
                        literais_errados_total[clausulas[valor][i]]++;
                    }
                    mtx_literais_errados.unlock();
                    mtx_bd_clausulas.lock();
                    clausulas_falsas.insert(valor);
                    mtx_bd_clausulas.unlock();
                }
                mtx_clausulas_verdadeiras.lock();
                clausulas_verdadeiras[valor]--;
                mtx_clausulas_verdadeiras.unlock();
                mtx4.unlock();
            }
        }
    }
}
void verifica_flip2() {
    long int tamanho_vetor_2 = v2.size();
    while(indice_threads_flip<tamanho_vetor_2) {
        mtx_index_flip.lock();
        int index = indice_threads_flip++;
        mtx_index_flip.unlock();
        if(index<tamanho_vetor_2) {
            long int valor = v2[index];
            if(mudancas[literal_flip]){
                mtx5.lock();
                if(clausulas_verdadeiras[valor] == 1) {
                    mtx_literais_errados.lock();
                    for(long int i=0;i<clausulas[valor].size();i++){
                        literais_errados_total[clausulas[valor][i]]++;
                    }
                    mtx_literais_errados.unlock();
                    mtx_bd_clausulas.lock();
                    clausulas_falsas.insert(valor);
                    mtx_bd_clausulas.unlock();
                }
                mtx_clausulas_verdadeiras.lock();
                clausulas_verdadeiras[valor]--;
                mtx_clausulas_verdadeiras.unlock();
                mtx5.unlock();
            } else if(!mudancas[literal_flip]){
                mtx6.lock();
                if(clausulas_verdadeiras[valor] == 0){
                    mtx_literais_errados.lock();
                    for(long int i=0;i<clausulas[valor].size();i++){
                        literais_errados_total[clausulas[valor][i]]--;
                        if(literais_errados_total[clausulas[valor][i]]<=0){
                            mtx_bd_literais.lock();
                            literais_errados_total.erase(clausulas[valor][i]);
                            mtx_bd_literais.unlock();
                        }
                    }
                    mtx_literais_errados.unlock();
                    mtx_bd_clausulas.lock();
                    clausulas_falsas.erase(valor);
                    mtx_bd_clausulas.unlock();
                }
                mtx_clausulas_verdadeiras.lock();
                clausulas_verdadeiras[valor]++;
                mtx_clausulas_verdadeiras.unlock();
                mtx6.unlock();
            }
        }
    }
}
void verifica_full() {
    while(indice_threads<clausulas.size()) {
        mtx.lock();
		long int i = indice_threads++;
        mtx.unlock();
        if (i < clausulas.size()) {
            for(auto valor:clausulas[i]) {
                if ((valor > 0 and mudancas[abs(valor)] == true) || (valor < 0 and mudancas[abs(valor)] == false)) {
                    mtx_bd.lock();
                    clausulas_verdadeiras[i]++;
                    mtx_bd.unlock();
                }
            }
            if(clausulas_verdadeiras[i] == 0) {
                mtx3.lock();
                clausulas_falsas.insert(i);
                for(auto valor:clausulas[i]) {
                    literais_errados_total[valor]++; 
                }
                mtx3.unlock();
            }
        }
    }
}

void imprimir_lista_literais_errados()
{
    if(clausulas_falsas.empty()) {
        cout << "SAT\n";
    } else {
        vector<pair<long int, long int>> aux_errados;
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

void* funcao_thread(void* arg) {
  long int index = *((long int *) arg);
  delete (long int *) arg;
  while (rodando) {
    sem_wait(&s_execucao[index]);
    if (rodando) {
        if (comando_flip_full) {
            verifica_full();
        }
        else {
            verifica_flip();
            verifica_flip2();
        }
      sem_post(&s_impressao[index]);
    }
  }
  return 0;
}

void executa_threads() {
  for (long int i = 0; i < QTD_THREADS; i++) {
    sem_post(&s_execucao[i]);
  }
  sem_post(&s_comando);
}


int main()
{
    cin >> numero_variaveis >> numero_clausulas;
    clausulas_verdadeiras = vector<long int>(numero_clausulas, 0);
    for(long int i=0; i<numero_clausulas; i++) {
        long int valor;
        vector<long int> vetor_clausula;
        while (cin >> valor, valor != 0) {
            vetor_clausula.push_back(valor);
            
            variaveis_clausulas[valor].push_back(i);
        }
        clausulas.push_back(vetor_clausula);
    }	

    sem_init(&s_comando, 0, 0);
    for (long int i = 0; i<QTD_THREADS; i++) {
        sem_init(&s_execucao[i], 0, 0);
        sem_init(&s_impressao[i], 0, 0);
    }

    for(long int i=0; i<QTD_THREADS; i++) {
        pthread_create(&threads[i], NULL, funcao_thread, new long int(i));
    } 

    string comando;
    mudancas.assign(numero_variaveis, false);
    while(cin >> comando) {
        long int valor;
		indice_threads = 0;
        indice_threads_flip = 0;
		pthread_t threads[QTD_THREADS];
        if(comando.compare("full") == 0) {
            literais_errados_total.clear();
            clausulas_falsas.clear();
            clausulas_verdadeiras.assign(numero_clausulas, 0);;
            comando_flip_full = true;
            for(long int i=0; i<numero_variaveis; i++) {
                cin >> valor;
                if (valor > 0) {
                    mudancas[abs(valor)] = true;
                } else {
                    mudancas[abs(valor)] = false;
                }
            }
        }
        else{
            comando_flip_full = false;
            cin >> literal_flip;
            mudancas[abs(literal_flip)] = !mudancas[abs(literal_flip)];
            v1.clear();
            v2.clear();
            for( auto valor:variaveis_clausulas[literal_flip]) {
                v1.push_back(valor);
            }
            for( auto valor:variaveis_clausulas[-literal_flip]) {
                v2.push_back(valor);
            }
        }
        executa_threads();
        for (long int i = 0; i < QTD_THREADS; i++) {
            sem_wait(&s_impressao[i]);
        }
        imprimir_lista_literais_errados();
    }
    rodando = false;
    for (long int i = 0; i < QTD_THREADS; i++) {
        sem_post(&s_execucao[i]);
    }
    for (long int i = 0; i < QTD_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

}