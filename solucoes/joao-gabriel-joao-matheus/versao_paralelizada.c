#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<string.h>

#define NUMERO_THREADS 12 // 4 6 8 12

#define chave_0(a) (a.lit->numero_clausulas_falsas)
#define chave_1(a) (a.lit->literal_absoluto)
#define maior(a, b) ((chave_0(a) > chave_0(b)) || ((chave_0(a) == chave_0(b)) && (chave_1(a) > chave_1(b))))
#define troca_posicao(a, b) { Ponteiro_Lit ponteiro_lit = a; a = b; b = ponteiro_lit; }
#define compara_troca(a, b) { if (maior(b, a)) troca_posicao(a, b); }

int numero_variaveis, numero_clausulas;

int etapa_0_atendida = 0, quantidade_full = 0, full = 0, variavel_inicio_verificacao = 0, variavel_fim_verificacao = 0, recebimento_valoracao_finalizado = 0;
int etapa_2_atendida = 0, sat = 0, etapa_3_atendida = 0;

int total_verificacoes = 0, numero_clausulas_falsas = 0, quantidade_lits = 0, numero_verificacoes_concluidas = 0;

struct CLAUSULA {
    int indice_clausula;
    struct CLAUSULA *proxima_clausula;
}; typedef struct CLAUSULA Clausula;

struct VALORACAO {
    int literal_verdadeiro, literal_verdadeiro_alterado;
    Clausula *primeira_clausula_literal, *ultima_clausula_literal, *primeira_clausula_literal_negado, *ultima_clausula_literal_negado;
}; typedef struct VALORACAO Valoracao;

Valoracao *valoracao;

struct LITERAL {
    int literal;
    struct LITERAL *proximo; // *anterior; 
}; typedef struct LITERAL Literal;

struct CLAUSULAS {
    int quantidade_literais_verdadeiros; // quantidade_literais;
    Literal *primeiro_literal, *ultimo_literal;
}; typedef struct CLAUSULAS Clausulas;

Clausulas *clausulas;

struct LIT {
    int literal, literal_absoluto, numero_clausulas_falsas;
}; typedef struct LIT Lit;

struct PONTEIRO_LIT {
    Lit *lit;
}; typedef struct PONTEIRO_LIT Ponteiro_Lit;

Ponteiro_Lit *lits, *lits_auxiliar;

pthread_mutex_t etapa_0_atendida_mutex, inicio_fim_verificacao_mutex, clausulas_mutex, numero_clausulas_falsas_mutex, lits_mutex, quantidade_lits_mutex;
pthread_mutex_t numero_verificacoes_concluidas_mutex, etapa_2_atendida_mutex, etapa_3_atendida_mutex;

pthread_barrier_t barreira_0, barreira_1;

sem_t sem_aguardar_leitura[NUMERO_THREADS], sem_etapa_2_finalizada;

void etapa_0(){
    pthread_mutex_lock(&etapa_0_atendida_mutex);
    int etapa_atendida = etapa_0_atendida;
    if (etapa_atendida == 0) etapa_0_atendida = 1;
    pthread_mutex_unlock(&etapa_0_atendida_mutex);

    if (etapa_atendida == 0){
        char string_valoracao[5];
        if (scanf("%s", string_valoracao) != -1){
            if (strcmp("full", string_valoracao) == 0){
                if (quantidade_full < 2) quantidade_full++;

                int literal_verdadeiro;
                for (int i=0; i<numero_variaveis; i++){
                    scanf("%d", &literal_verdadeiro);
                    if (valoracao[i].literal_verdadeiro != literal_verdadeiro){
                        valoracao[i].literal_verdadeiro_alterado = 1; valoracao[i].literal_verdadeiro = literal_verdadeiro;
                    } else
                        valoracao[i].literal_verdadeiro_alterado = 0;
                }

                full = 1; variavel_inicio_verificacao = 0; variavel_fim_verificacao = numero_variaveis; total_verificacoes = numero_variaveis;
            } else if (strcmp("flip", string_valoracao) == 0){
                int literal_absoluto_flip, indice_valoracao;
                scanf("%d", &literal_absoluto_flip);
                indice_valoracao = literal_absoluto_flip - 1;
                valoracao[indice_valoracao].literal_verdadeiro = valoracao[indice_valoracao].literal_verdadeiro * (-1);
                full = 0; variavel_inicio_verificacao = indice_valoracao; variavel_fim_verificacao = indice_valoracao + 1; total_verificacoes = 1;
            }
        } else recebimento_valoracao_finalizado = 1;

        for (int i=0; i<(NUMERO_THREADS); i++) sem_post(&sem_aguardar_leitura[i]);
    }
}

void clausulas_literal(Clausula *clausula, int valor){
    while (clausula != NULL){
        pthread_mutex_lock(&clausulas_mutex);
        if (valor == 0) clausulas[clausula->indice_clausula].quantidade_literais_verdadeiros--;
        int quantidade_literais_verdadeiros_clausula = clausulas[clausula->indice_clausula].quantidade_literais_verdadeiros;
        if (valor == 1) clausulas[clausula->indice_clausula].quantidade_literais_verdadeiros++;
        pthread_mutex_unlock(&clausulas_mutex);

        if (quantidade_literais_verdadeiros_clausula == 0){
            pthread_mutex_lock(&numero_clausulas_falsas_mutex);
            if (valor == 0) numero_clausulas_falsas++; else numero_clausulas_falsas--;
            pthread_mutex_unlock(&numero_clausulas_falsas_mutex);

            Literal *literal = clausulas[clausula->indice_clausula].primeiro_literal;
            while (literal != NULL){
                int indice_lits = (literal->literal < 0) ? (literal->literal * (-1)) - 1 : literal->literal - 1;
                pthread_mutex_lock(&lits_mutex);
                lits_auxiliar[indice_lits].lit->literal = valoracao[indice_lits].literal_verdadeiro * (-1);
                int numero_clausulas_falsas_literal = lits_auxiliar[indice_lits].lit->numero_clausulas_falsas;
                if (valor == 0) lits_auxiliar[indice_lits].lit->numero_clausulas_falsas++; else lits_auxiliar[indice_lits].lit->numero_clausulas_falsas--;
                pthread_mutex_unlock(&lits_mutex);

                if (valor == 0 && numero_clausulas_falsas_literal == 0){
                    pthread_mutex_lock(&quantidade_lits_mutex); quantidade_lits++; pthread_mutex_unlock(&quantidade_lits_mutex);
                } else if (valor == 1 && numero_clausulas_falsas_literal == 1){
                    pthread_mutex_lock(&quantidade_lits_mutex); quantidade_lits--; pthread_mutex_unlock(&quantidade_lits_mutex);                
                }

                literal = literal->proximo;
            }
        }

        clausula = clausula->proxima_clausula;
    }
}

void etapa_1(int variavel_em_analise){
    if (full == 1 && valoracao[variavel_em_analise].literal_verdadeiro_alterado == 0) return ;
    if (valoracao[variavel_em_analise].literal_verdadeiro > 0){
        clausulas_literal(valoracao[variavel_em_analise].primeira_clausula_literal_negado, 0);
        if (full == 1 && quantidade_full == 1) return ;
        clausulas_literal(valoracao[variavel_em_analise].primeira_clausula_literal, 1);
    } else {
        clausulas_literal(valoracao[variavel_em_analise].primeira_clausula_literal, 0);
        if (full == 1 && quantidade_full == 1) return ;
        clausulas_literal(valoracao[variavel_em_analise].primeira_clausula_literal_negado, 1);
    }
}

void aguardar_etapa_1(){
    while (1){
        pthread_mutex_lock(&inicio_fim_verificacao_mutex);
        int inicio_verificacao = variavel_inicio_verificacao, fim_verificacao = variavel_fim_verificacao;
        pthread_mutex_unlock(&inicio_fim_verificacao_mutex);
        pthread_mutex_lock(&numero_verificacoes_concluidas_mutex);
        int numero_concluidas = numero_verificacoes_concluidas;
        pthread_mutex_unlock(&numero_verificacoes_concluidas_mutex);
        if (inicio_verificacao == fim_verificacao && numero_concluidas == total_verificacoes) break;
    }
}

void etapa_2(){
    pthread_mutex_lock(&etapa_2_atendida_mutex);
    int etapa_atendida = etapa_2_atendida;
    if (etapa_atendida == 0) etapa_2_atendida = 1;
    pthread_mutex_unlock(&etapa_2_atendida_mutex);

    if (etapa_atendida == 0){
        aguardar_etapa_1();

        if (numero_clausulas_falsas == 0){ sat = 1; printf("SAT\n"); sem_post(&sem_etapa_2_finalizada); return ; }

        printf("[%d clausulas falsas]", numero_clausulas_falsas);               
        for (int i=0; i<numero_clausulas; i++){ if (clausulas[i].quantidade_literais_verdadeiros == 0) printf(" %d", i); }

        printf("\n[lits]"); sem_post(&sem_etapa_2_finalizada);
    }
}

void ordenar_lits(Ponteiro_Lit *vetor, int posicao_inicio, int posicao_fim){
    for (int i = posicao_fim; i > posicao_inicio; i--)
        compara_troca(vetor[i-1], vetor[i]);

    for (int i = posicao_inicio + 2; i <= posicao_fim; i++){ 
        Ponteiro_Lit tmp = vetor[i];

        while(maior(tmp, vetor[i - 1])){ vetor[i] = vetor[i - 1]; i--; }
        vetor[i] = tmp;
    }
}

void etapa_3(){
    pthread_mutex_lock(&etapa_3_atendida_mutex);
    int etapa_atendida = etapa_3_atendida;
    if (etapa_atendida == 0) etapa_3_atendida = 1;
    pthread_mutex_unlock(&etapa_3_atendida_mutex);

    if (etapa_atendida == 0){
        aguardar_etapa_1();

        ordenar_lits(lits, 0, numero_variaveis - 1);

        char a[5*(numero_variaveis)];
        char *ap = a;

        for (int i; i<quantidade_lits; i++) ap+=sprintf(ap, " %d", lits[i].lit->literal);

        sem_wait(&sem_etapa_2_finalizada);

        if (sat == 1) return ;

        printf("%s\n", a);

        // for (int i=0; i<quantidade_lits; i++) printf(" %d", lits[i].lit->literal); printf("\n");
    }
}

void *verificar(void *argumento){
    int i = (*(int *)(argumento));

    while (1){  
        etapa_0(); // Receber valoração

        sem_wait(&sem_aguardar_leitura[i]);

        if (recebimento_valoracao_finalizado == 1) break;

        while (1){
            pthread_mutex_lock(&inicio_fim_verificacao_mutex);
            int inicio_verificacao = variavel_inicio_verificacao, fim_verificacao = variavel_fim_verificacao;
            if (inicio_verificacao < fim_verificacao) variavel_inicio_verificacao++;
            pthread_mutex_unlock(&inicio_fim_verificacao_mutex);
            if (inicio_verificacao == fim_verificacao) break;

            etapa_1(inicio_verificacao);

            pthread_mutex_lock(&numero_verificacoes_concluidas_mutex);
            numero_verificacoes_concluidas++;
            pthread_mutex_unlock(&numero_verificacoes_concluidas_mutex);
        }

        etapa_2(); // Imprimir cláusulas falsas
        etapa_3(); // Imprimir lits

        pthread_barrier_wait(&barreira_0);

        if (sat == 1) break;

        etapa_0_atendida = 0; numero_verificacoes_concluidas = 0; etapa_2_atendida = 0; etapa_3_atendida = 0;

        pthread_barrier_wait(&barreira_1);
    }
    pthread_exit(0);
}

int main() {
    scanf("%d %d", &numero_variaveis, &numero_clausulas);

    valoracao = (Valoracao *)malloc(numero_variaveis*sizeof(Valoracao));
    for (int i=0; i<numero_variaveis; i++){
        valoracao[i].literal_verdadeiro = 0; valoracao[i].literal_verdadeiro_alterado = 0;
        valoracao[i].primeira_clausula_literal = NULL; valoracao[i].ultima_clausula_literal = NULL;
        valoracao[i].primeira_clausula_literal_negado = NULL; valoracao[i].ultima_clausula_literal_negado = NULL;
    }

    clausulas = (Clausulas *)malloc(numero_clausulas*sizeof(Clausulas));
    for (int i=0; i<numero_clausulas; i++){
        clausulas[i].quantidade_literais_verdadeiros = 0;
        clausulas[i].primeiro_literal = NULL; clausulas[i].ultimo_literal = NULL;
    }

    for (int i=0; i<numero_clausulas; i++){
        int literal, indice_valoracao;

        scanf("%d", &literal);

        while (literal != 0){
            Clausula *nova_clausula = (Clausula *)malloc(sizeof(Clausula));
            nova_clausula->indice_clausula = i; nova_clausula->proxima_clausula = NULL;

            if (literal < 0){
                indice_valoracao = (literal * (-1)) - 1;
                if (valoracao[indice_valoracao].primeira_clausula_literal_negado == NULL){
                    valoracao[indice_valoracao].primeira_clausula_literal_negado = nova_clausula;
                    valoracao[indice_valoracao].ultima_clausula_literal_negado = nova_clausula;
                } else {
                    valoracao[indice_valoracao].ultima_clausula_literal_negado->proxima_clausula = nova_clausula;
                    valoracao[indice_valoracao].ultima_clausula_literal_negado = nova_clausula;
                }
            } else {
                indice_valoracao = literal - 1;
                if (valoracao[indice_valoracao].primeira_clausula_literal == NULL){
                    valoracao[indice_valoracao].primeira_clausula_literal = nova_clausula;
                    valoracao[indice_valoracao].ultima_clausula_literal = nova_clausula;
                } else {
                    valoracao[indice_valoracao].ultima_clausula_literal->proxima_clausula = nova_clausula;
                    valoracao[indice_valoracao].ultima_clausula_literal = nova_clausula;
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

    pthread_t thread[NUMERO_THREADS];

    int identificador_thread[NUMERO_THREADS];
    for (int i=0; i<(NUMERO_THREADS); i++) identificador_thread[i] = i;

    pthread_mutex_init(&etapa_0_atendida_mutex, NULL); pthread_mutex_init(&inicio_fim_verificacao_mutex, NULL); pthread_mutex_init(&clausulas_mutex, NULL);
    pthread_mutex_init(&numero_clausulas_falsas_mutex, NULL); pthread_mutex_init(&lits_mutex, NULL); pthread_mutex_init(&quantidade_lits_mutex, NULL);
    pthread_mutex_init(&numero_verificacoes_concluidas_mutex, NULL); pthread_mutex_init(&etapa_2_atendida_mutex, NULL);
    pthread_mutex_init(&etapa_3_atendida_mutex, NULL);

    for (int i=0; i<(NUMERO_THREADS); i++) sem_init(&sem_aguardar_leitura[i], 0, 0);
    sem_init(&sem_etapa_2_finalizada, 0, 0);

    pthread_barrier_init(&barreira_0, NULL, NUMERO_THREADS); pthread_barrier_init(&barreira_1, NULL, NUMERO_THREADS);

    for (int i=0; i<NUMERO_THREADS; i++) pthread_create(&thread[i], NULL, &verificar, (void *)(&identificador_thread[i]));
    for (int i=0; i<NUMERO_THREADS; i++) pthread_join(thread[i], NULL);

    pthread_barrier_destroy(&barreira_0); pthread_barrier_destroy(&barreira_1);

    for (int i=0; i<(NUMERO_THREADS); i++) sem_destroy(&sem_aguardar_leitura[i]);
    sem_destroy(&sem_etapa_2_finalizada);

    pthread_mutex_destroy(&etapa_0_atendida_mutex); pthread_mutex_destroy(&inicio_fim_verificacao_mutex); pthread_mutex_destroy(&clausulas_mutex);
    pthread_mutex_destroy(&numero_clausulas_falsas_mutex); pthread_mutex_destroy(&lits_mutex); pthread_mutex_destroy(&quantidade_lits_mutex);
    pthread_mutex_destroy(&numero_verificacoes_concluidas_mutex); pthread_mutex_destroy(&etapa_2_atendida_mutex);
    pthread_mutex_destroy(&etapa_3_atendida_mutex);

    return 0;
}

// indice_valoracao = [0, numero_variaveis - 1] == [0, |(máximo indice literal)| - 1] === valoracao[|literal| - 1] e indice_lits = [0, numero_variaveis - 1]
// indice_clausula = [0, numero_clausulas - 1] e literais = [-numero_variaveis, numero_variaveis]