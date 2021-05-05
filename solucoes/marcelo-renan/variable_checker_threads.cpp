#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <thread>
#include <mutex>

#define THREAD_NUMBER 2

using namespace std;

int qtd_variaveis, qtd_clausulas;
mutex global_mutex;

struct{
    bool operator()(int a, int b) const { return abs(b) - abs(a); }
} compare_elements;

class LiteralFalso {
  public:
    int literal;
    int frequencia;
    LiteralFalso(int lit, int freq) {
        literal = lit;
        frequencia = freq;
    }
    bool operator<(const  LiteralFalso & other) {
        if(frequencia == other.frequencia)
            return abs(other.literal) < abs(literal);
        return frequencia > other.frequencia;
    }
};

vector<int> indices_clausulas_falsas;
vector<int> todos_literais_falsos;
vector<LiteralFalso> literais_falsos_obj;

vector<vector<int>> read_values(){
    vector<vector<int>> valores;
    int variavel;
    int current_input;

    for(int i = 0; i < qtd_clausulas; i++){
        vector<int> vetor_auxiliar;

        while (1){
            cin >> current_input;
            vetor_auxiliar.push_back(current_input);

            if(current_input == 0) break;
        }

        valores.push_back(vetor_auxiliar);
    }

    return valores;
}

int get_index(vector<int> v, int K){
    auto it = find(v.begin(), v.end(), K);
    if (it != v.end()) {
        int index = it - v.begin();
        return index;
    }
    else {
        // element not found
        return -1;
    }
}

bool value_in_vector(vector<int> v, int value){
    return get_index(v, value) != -1;
}

void avaliar_uma_clausula(vector<vector<int>> valores, int indiceDaClausula, vector<int> avaliadores){
    // cout << "sou a thread : " << this_thread::get_id() << " avaliando o indice : " << indiceDaClausula << endl;
    int ccontrol = 0;
    int econtrol = 0;
    bool isVerdadeira = false;

    vector<int> literais_falsos;
    literais_falsos.clear();
    while(valores[indiceDaClausula][ccontrol] != 0){
        for(int i = 0; i < avaliadores.size(); i++){
            econtrol = i;
            if(abs(valores[indiceDaClausula][ccontrol]) == abs(avaliadores[i])){
                break;
            }
        }

        if(valores[indiceDaClausula][ccontrol] == avaliadores[econtrol]){
            isVerdadeira = true;
            literais_falsos.clear();
            break;
        }

        if(!value_in_vector(literais_falsos, valores[indiceDaClausula][ccontrol])){
            literais_falsos.push_back(valores[indiceDaClausula][ccontrol]);
        } 
        // else{
            // cout << "valor repetido: " << valores[indiceDaClausula][ccontrol] << " na clausula " << indiceDaClausula << endl;// fix tem isso mesmo?
        // }

        ccontrol++;
        econtrol = 0;
    }

    for(int i = 0; i < literais_falsos.size(); i++){
        int literal_falso = literais_falsos[i];

        if(!value_in_vector(todos_literais_falsos, literal_falso)){
            global_mutex.lock();
            todos_literais_falsos.push_back(literal_falso);
            LiteralFalso novo_literal(literal_falso, 1);
            literais_falsos_obj.push_back(novo_literal);
            global_mutex.unlock();
        }
        else {
            int idx_literal;
            global_mutex.lock();
            for (int i = 0; i<literais_falsos_obj.size(); i++) {
                if(literais_falsos_obj[i].literal == literal_falso) {
                    literais_falsos_obj[i].frequencia++;
                }
            }
            global_mutex.unlock();
        }

    }

    if(!isVerdadeira){
        global_mutex.lock();
        indices_clausulas_falsas.push_back(indiceDaClausula);
        global_mutex.unlock();
    }
}

void vector_print(vector<int> vector){
    for(auto i : vector) cout << " " << i;
    cout << endl;
}

void join_all_threads(thread *allThreads, int number_of_threads){
    for(int i = 0; i < number_of_threads; i++){
        if(allThreads[i].joinable()) allThreads[i].join();
    }
}

bool using_threads(int thread_number, vector<vector<int>> valores, vector<int> avaliadores){

    thread *allMyThreads = new thread[thread_number];
    int c_index = 0;

    while(true){

        for(int i = 0; i < thread_number; i++){
            allMyThreads[i] = thread(avaliar_uma_clausula, valores, c_index, avaliadores);
            if(c_index == qtd_clausulas-1){
                join_all_threads(allMyThreads, thread_number);
                return false;
            }
            c_index++;
        }

        join_all_threads(allMyThreads, thread_number);
    }
}

void avaliar_clausulas(vector<vector<int>> valores, vector<int> avaliadores){
    indices_clausulas_falsas.clear();
    todos_literais_falsos.clear();
    literais_falsos_obj.clear();

    while(true){
        if(!using_threads(THREAD_NUMBER, valores, avaliadores)) break;
    }

    if(indices_clausulas_falsas.empty()){
        printf("SAT\n");
    }
    else{
        printf("[%lu clausulas falsas]", indices_clausulas_falsas.size());
        for(int i = 0; i < indices_clausulas_falsas.size(); i++) printf(" %d", indices_clausulas_falsas[i]);
        printf("\n");

        sort(literais_falsos_obj.begin(), literais_falsos_obj.end());

        printf("[lits]");
        for (LiteralFalso l : literais_falsos_obj) {
            cout << " " << l.literal;
        }
        printf("\n");
    }

}

int main(int argc, char const *argv[]){
    cin >> qtd_variaveis;
    cin >> qtd_clausulas;

    vector<vector<int>> valores = read_values();

    vector<int> avaliadores;
    string avaliacao;

    while(1){
        cin >> avaliacao;

        if(!cin.good()) break;

        if(avaliacao.compare("full") == 0){
            int valor_lido;
            for(int i = 0; i < qtd_variaveis; i++){
                cin >> valor_lido;
                avaliadores.push_back(valor_lido);
            }

            avaliar_clausulas(valores, avaliadores);

        } else if(avaliacao.compare("flip") == 0){
            int valor_flip;
            cin >> valor_flip;
            avaliadores[valor_flip-1] *= -1;

            avaliar_clausulas(valores, avaliadores);
        }
        else cout << "não reconheço\n";
    }

    return 0;
}
