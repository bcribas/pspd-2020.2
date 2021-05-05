#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>

using namespace std;

int qtd_variaveis, qtd_clausulas;

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

void matrix_print(vector<vector<int>> valores){
    for(int i = 0; i < valores.size(); i++){
        for(int j = 0; valores[i][j] != 0; j++){
            cout << valores[i][j] << " ";
        }   
        cout << "\n";
    }
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

void avaliar_clausulas(vector<vector<int>> valores, vector<int> avaliadores){
    vector<int> indices_clausulas_falsas;
    vector<int> literais_falsos;
    vector<int> todos_literais_falsos;
    vector<LiteralFalso> literais_falsos_obj;
    // cout << "mostrando clausulas lidas\n";
    for(int c_index = 0; c_index < qtd_clausulas; c_index++){

        int ccontrol = 0;
        int econtrol = 0;
        bool isVerdadeira = false;

        literais_falsos.clear();
        while(valores[c_index][ccontrol] != 0){
            for(int i = 0; i < avaliadores.size(); i++){
                econtrol = i;
                if(abs(valores[c_index][ccontrol]) == abs(avaliadores[i])){
                    break;
                }
            }

            if(valores[c_index][ccontrol] == avaliadores[econtrol]){
                isVerdadeira = true;
                literais_falsos.clear();
                break;
            }

            if(!value_in_vector(literais_falsos, valores[c_index][ccontrol])){
                literais_falsos.push_back(valores[c_index][ccontrol]);
            } else{
                cout << "valor repetido: " << valores[c_index][ccontrol] << " na clausula " << c_index << endl;// fix tem isso mesmo?
            }

            ccontrol++;
            econtrol = 0;
        }

        for(int i = 0; i < literais_falsos.size(); i++){
            int literal_falso = literais_falsos[i];

            if(!value_in_vector(todos_literais_falsos, literal_falso)){
                todos_literais_falsos.push_back(literal_falso);
                LiteralFalso novo_literal(literal_falso, 1);
                literais_falsos_obj.push_back(novo_literal);
            }
            else {
                int idx_literal;
                // procura literal falso no vetor para adicionar frequência
                for (int i = 0; i<literais_falsos_obj.size(); i++) {
                    if(literais_falsos_obj[i].literal == literal_falso) {
                        literais_falsos_obj[i].frequencia++;
                    }
                }
            }

        }

        if(!isVerdadeira){
            indices_clausulas_falsas.push_back(c_index);
        }
    }

    if(indices_clausulas_falsas.empty()){
        printf("SAT\n");
    }
    else{
        printf("[%lu clausulas falsas]", indices_clausulas_falsas.size());
        for(int i = 0; i < indices_clausulas_falsas.size(); i++) printf(" %d", indices_clausulas_falsas[i]);
        printf("\n");

        // ordernar literais por ordem de frequência e decrescente de grandeza
        sort(literais_falsos_obj.begin(), literais_falsos_obj.end());

        printf("[lits]");
        for (LiteralFalso l : literais_falsos_obj) {
            cout << " " << l.literal;
            // cout << " " << l.literal << ":" << l.frequencia; // debug
        }
        printf("\n");

        literais_falsos.clear();
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

            // avaliar clausulas...
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
