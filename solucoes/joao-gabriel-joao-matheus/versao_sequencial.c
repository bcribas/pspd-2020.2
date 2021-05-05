#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int numero_variaveis, numero_clausulas;

int quantidade_full = 0;

int numero_clausulas_falsas = 0, quantidade_lits = 0;

struct CLAUSULA_VALORACAO {
    int indice_clausula;
    struct CLAUSULA_VALORACAO *proxima_clausula;
}; typedef struct CLAUSULA_VALORACAO Clausula_Valoracao;

struct VALORACAO {
    int literal_verdadeiro, literal_verdadeiro_alterado;
    Clausula_Valoracao *primeira_clausula_literal, *ultima_clausula_literal, *primeira_clausula_literal_negado, *ultima_clausula_literal_negado;
}; typedef struct VALORACAO Valoracao;

Valoracao *valoracao;

struct LITERAL {
    int literal;
    struct LITERAL *proximo; // *anterior; 
}; typedef struct LITERAL Literal;

struct CLAUSULA {
    int quantidade_literais_verdadeiros; // quantidade_literais;
    Literal *primeiro_literal, *ultimo_literal;
}; typedef struct CLAUSULA Clausula;

Clausula *clausulas;

struct LIT {
    int literal, literal_absoluto, numero_clausulas_falsas;
}; typedef struct LIT Lit;

struct PONTEIRO_LIT {
    Lit *lit;
}; typedef struct PONTEIRO_LIT Ponteiro_Lit;

Ponteiro_Lit *lits, *lits_auxiliar;

#define chave_0(a) (a.lit->numero_clausulas_falsas)
#define chave_1(a) (a.lit->literal_absoluto)
#define maior(a, b) ((chave_0(a) > chave_0(b)) || ((chave_0(a) == chave_0(b)) && (chave_1(a) > chave_1(b))))
#define troca_posicao(a, b) { Ponteiro_Lit ponteiro_lit = a; a = b; b = ponteiro_lit; }
#define compara_troca(a, b) { if (maior(b, a)) troca_posicao(a, b); }

void ordenar_lits(Ponteiro_Lit *vetor, int posicao_inicio, int posicao_fim){
    for (int i = posicao_fim; i > posicao_inicio; i--)
        compara_troca(vetor[i-1], vetor[i]);

    for (int i = posicao_inicio + 2; i <= posicao_fim; i++){ 
        Ponteiro_Lit tmp = vetor[i];

        while(maior(tmp, vetor[i - 1])){ vetor[i] = vetor[i - 1]; i--; }
        vetor[i] = tmp;
    }
}

void clausulas_literal_falso(Clausula_Valoracao *clausula_valoracao){
    Literal *literal;

    int indice_lits;
   
    while (clausula_valoracao != NULL){
        clausulas[clausula_valoracao->indice_clausula].quantidade_literais_verdadeiros--;
        if (clausulas[clausula_valoracao->indice_clausula].quantidade_literais_verdadeiros == 0){
            numero_clausulas_falsas++;

            literal = clausulas[clausula_valoracao->indice_clausula].primeiro_literal;
            while (literal != NULL){
                indice_lits = (literal->literal < 0) ? (literal->literal * (-1)) - 1 : literal->literal - 1;
                lits_auxiliar[indice_lits].lit->literal = literal->literal;

                if (lits_auxiliar[indice_lits].lit->numero_clausulas_falsas == 0) quantidade_lits++;

                lits_auxiliar[indice_lits].lit->numero_clausulas_falsas++;

                literal = literal->proximo;
            }
        }

        clausula_valoracao = clausula_valoracao->proxima_clausula;
    }
}

void clausulas_literal_verdadeiro(Clausula_Valoracao *clausula_valoracao){
    Literal *literal;

    int indice_lits;

    while (clausula_valoracao != NULL){
        if (clausulas[clausula_valoracao->indice_clausula].quantidade_literais_verdadeiros == 0){
            numero_clausulas_falsas--;

            literal = clausulas[clausula_valoracao->indice_clausula].primeiro_literal;
            while (literal != NULL){
                indice_lits = (literal->literal < 0) ? (literal->literal * (-1)) - 1 : literal->literal - 1;

                if (lits_auxiliar[indice_lits].lit->numero_clausulas_falsas == 1) quantidade_lits--;

                lits_auxiliar[indice_lits].lit->numero_clausulas_falsas--;

                literal = literal->proximo;
            }
        }

        clausulas[clausula_valoracao->indice_clausula].quantidade_literais_verdadeiros++;

        clausula_valoracao = clausula_valoracao->proxima_clausula;
    }
}

void verificador(int split, int inicio_verificacao, int fim_verificacao){
    for (int i=inicio_verificacao; i<fim_verificacao; i++){
        if (split == 0 && valoracao[i].literal_verdadeiro_alterado == 0) continue;
        if (valoracao[i].literal_verdadeiro > 0){
            clausulas_literal_falso(valoracao[i].primeira_clausula_literal_negado);
            if (split == 0 && quantidade_full == 1) continue;
            clausulas_literal_verdadeiro(valoracao[i].primeira_clausula_literal);
        } else {
            clausulas_literal_falso(valoracao[i].primeira_clausula_literal);
            if (split == 0 && quantidade_full == 1) continue;
            clausulas_literal_verdadeiro(valoracao[i].primeira_clausula_literal_negado);
        }
    }

    if (numero_clausulas_falsas == 0){ printf("SAT\n"); return ; }

    printf("[%d clausulas falsas]", numero_clausulas_falsas);
    for (int i=0; i<numero_clausulas; i++){ if (clausulas[i].quantidade_literais_verdadeiros == 0) printf(" %d", i); }

    printf("\n[lits]");
    ordenar_lits(lits, 0, numero_variaveis - 1);
    for (int i=0; i<quantidade_lits; i++) printf(" %d", lits[i].lit->literal); printf("\n");

    return ;
}

int main() {
    scanf("%d %d", &numero_variaveis, &numero_clausulas);

    valoracao = (Valoracao *)malloc(numero_variaveis*sizeof(Valoracao));
    for (int i=0; i<numero_variaveis; i++){
        valoracao[i].literal_verdadeiro = 0; valoracao[i].literal_verdadeiro_alterado = 0;
        valoracao[i].primeira_clausula_literal = NULL; valoracao[i].ultima_clausula_literal = NULL;
        valoracao[i].primeira_clausula_literal_negado = NULL; valoracao[i].ultima_clausula_literal_negado = NULL;
    }

    clausulas = (Clausula *)malloc(numero_clausulas*sizeof(Clausula));
    for (int i=0; i<numero_clausulas; i++){
        clausulas[i].quantidade_literais_verdadeiros = 0; clausulas[i].primeiro_literal = NULL; clausulas[i].ultimo_literal = NULL;
    }

    for (int i=0; i<numero_clausulas; i++){
        int literal, indice_valoracao;

        scanf("%d", &literal);

        while (literal != 0){
            Clausula_Valoracao *nova_clausula_valoracao = (Clausula_Valoracao *)malloc(sizeof(Clausula_Valoracao));
            nova_clausula_valoracao->indice_clausula = i; nova_clausula_valoracao->proxima_clausula = NULL;

            if (literal < 0){
                indice_valoracao = (literal * (-1)) - 1;
                if (valoracao[indice_valoracao].primeira_clausula_literal_negado == NULL){
                    valoracao[indice_valoracao].primeira_clausula_literal_negado = nova_clausula_valoracao;
                    valoracao[indice_valoracao].ultima_clausula_literal_negado = nova_clausula_valoracao;
                } else {
                    valoracao[indice_valoracao].ultima_clausula_literal_negado->proxima_clausula = nova_clausula_valoracao;
                    valoracao[indice_valoracao].ultima_clausula_literal_negado = nova_clausula_valoracao;
                }
            } else {
                indice_valoracao = literal - 1;
                if (valoracao[indice_valoracao].primeira_clausula_literal == NULL){
                    valoracao[indice_valoracao].primeira_clausula_literal = nova_clausula_valoracao;
                    valoracao[indice_valoracao].ultima_clausula_literal = nova_clausula_valoracao;
                } else {
                    valoracao[indice_valoracao].ultima_clausula_literal->proxima_clausula = nova_clausula_valoracao;
                    valoracao[indice_valoracao].ultima_clausula_literal = nova_clausula_valoracao;
                }
            }

            Literal *novo_literal = (Literal *)malloc(sizeof(Literal));
            novo_literal->literal = literal; novo_literal->proximo = NULL;

            if (clausulas[i].primeiro_literal == NULL){
                clausulas[i].primeiro_literal = novo_literal; clausulas[i].ultimo_literal = novo_literal;
            } else {
                clausulas[i].ultimo_literal->proximo = novo_literal; clausulas[i].ultimo_literal = novo_literal;
            }

            clausulas[i].quantidade_literais_verdadeiros++; // Uma clausula pode ter dois literais iguais??

            scanf("%d", &literal);
        }
    }

    lits = (Ponteiro_Lit *)malloc(numero_variaveis*sizeof(Ponteiro_Lit));
    lits_auxiliar = (Ponteiro_Lit *)malloc(numero_variaveis*sizeof(Ponteiro_Lit));

    for (int i=0; i<numero_variaveis; i++){
        Lit *lit = (Lit *)malloc(sizeof(Lit));
        lit->literal = 0; lit->literal_absoluto = i + 1; lit->numero_clausulas_falsas = 0;
        lits[i].lit = lit; lits_auxiliar[i].lit = lit;
    }

    char string_valoracao[5];
    int literal_verdadeiro, literal_absoluto_flip, indice_valoracao;

    while (scanf("%s", string_valoracao) != -1){
        if (strcmp("full", string_valoracao) == 0){
            if (quantidade_full < 2) quantidade_full++;

            for (int i=0; i<numero_variaveis; i++){
                scanf("%d", &literal_verdadeiro);
                if (valoracao[i].literal_verdadeiro != literal_verdadeiro){
                    valoracao[i].literal_verdadeiro_alterado = 1; valoracao[i].literal_verdadeiro = literal_verdadeiro;
                } else
                    valoracao[i].literal_verdadeiro_alterado = 0;
            }

            verificador(0, 0, numero_variaveis);
        } else if (strcmp("flip", string_valoracao) == 0){
            scanf("%d", &literal_absoluto_flip);
            indice_valoracao = literal_absoluto_flip - 1;
            valoracao[indice_valoracao].literal_verdadeiro = valoracao[indice_valoracao].literal_verdadeiro * (-1);
            verificador(1, indice_valoracao, indice_valoracao + 1);
        }
    }

    return 0;
}

// indice_valoracao = [0, numero_variaveis - 1] == [0, |(máximo indice literal)| - 1] === valoracao[|literal| - 1] e indice_lits = [0, numero_variaveis - 1]
// indice_clausula = [0, numero_clausulas - 1] e literais = [-numero_variaveis, numero_variaveis]
