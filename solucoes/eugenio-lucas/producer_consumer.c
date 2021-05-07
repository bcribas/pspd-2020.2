#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/queue.h>

#define CMD_FULL 0
#define CMD_FLIP 1

STAILQ_HEAD(inputhead, queue_input_elem) input_queue;
STAILQ_HEAD(resulthead, queue_result_elem) result_queue;

typedef struct
{
    pthread_t id;
    int thread_index;
    int start;
    int end;
} ThreadParams;

typedef struct
{
    int lits_amount;
    int *lits;
} Clause;

typedef struct
{
    int value;
    int count;
} FalseLiteral;

typedef struct queue_input_elem
{
    u_int8_t type;
    int *full_vars;
    int flip_var;
    int reads_remaining;
    STAILQ_ENTRY(queue_input_elem) entries;
} input_elem_t;

typedef struct queue_result_elem
{
    int false_clauses_count;
    int *false_clauses;
    FalseLiteral *false_literals_hist;
    int written_count;
    STAILQ_ENTRY(queue_result_elem) entries;
} result_elem_t;

sem_t sem_read_input;
sem_t sem_write_result;
sem_t *sem_available_inputs;
sem_t sem_available_results;

int vars_amount, clauses_amount;
Clause *clauses;

int threads_num = 1;
int inputs_finished = 0, inputs_amount = 0;

void read_clauses();
void *read_commands();
void split_clauses_to_threads(ThreadParams *threads, int num_clauses);
void *sat_checker(void *params);
void *show_results();
int compare_false_literal(const void *literal_a, const void *literal_b);
void enqueue_input(input_elem_t *input);
input_elem_t *get_input(input_elem_t *previous_input, int thread_index);
result_elem_t *enqueue_result(result_elem_t *previous_result, int thread_index, int false_clauses_count, int *false_clauses, int *false_literals_partial, int *current_vars);
result_elem_t *get_result();

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        threads_num = atoi(argv[1]);
    }

    ThreadParams threads[threads_num];
    pthread_t thread_read_inputs, thread_read_results;

    sem_init(&sem_read_input, 0, 1);
    sem_init(&sem_write_result, 0, 1);
    sem_init(&sem_available_results, 0, 0);

    sem_available_inputs = (sem_t *) malloc(threads_num * sizeof(sem_t));
    for (int thread_index = 0; thread_index < threads_num; thread_index++)
    {
        sem_init(&sem_available_inputs[thread_index], 0, 0);
    }

    STAILQ_INIT(&input_queue);
    STAILQ_INIT(&result_queue);

    scanf("%d %d", &vars_amount, &clauses_amount);

    clauses = (Clause *) calloc(clauses_amount, sizeof(Clause));

    read_clauses();
    split_clauses_to_threads(threads, clauses_amount);

    for (int i = 0; i < threads_num; i++)
        pthread_create(&threads[i].id, NULL, &sat_checker, (void *)(&threads[i]));

    pthread_create(&thread_read_inputs, NULL, &read_commands, NULL);
    pthread_create(&thread_read_results, NULL, &show_results, NULL);

    pthread_join(thread_read_results, NULL);

    return 0;
}

void read_clauses()
{
    for (int clause = 0; clause < clauses_amount; clause++)
    {
        int lit = 0;
        clauses[clause].lits_amount = 0;
        while (1)
        {
            scanf("%d", &lit);
            if (lit == 0)
                break;

            clauses[clause].lits = (int *) realloc(
                clauses[clause].lits,
                ++clauses[clause].lits_amount * sizeof(int));
            clauses[clause].lits[clauses[clause].lits_amount - 1] = lit;
        }
    }
}

void *read_commands()
{
    char command[10];
    int var_val = 0;

    while (scanf("%s", command) != EOF)
    {
        input_elem_t *input = (input_elem_t *)calloc(1, sizeof(input_elem_t));

        if (strcmp("full", command) == 0)
        {
            input->type = CMD_FULL;
            input->full_vars = (int *)calloc(vars_amount, sizeof(int));
            for (int i = 1; i <= vars_amount; i++)
            {
                scanf("%d", &var_val);

                input->full_vars[abs(var_val) - 1] = var_val;
            }
        }
        else if (strcmp("flip", command) == 0)
        {
            input->type = CMD_FLIP;
            scanf("%d", &var_val);
            input->flip_var = var_val;
        }

        enqueue_input(input);
        inputs_amount++;
    }

    inputs_finished = 1;
}

void split_clauses_to_threads(ThreadParams *threads, int num_clauses)
{
    if (num_clauses < threads_num)
    {
        printf("Can't run this input with more than %d threads\n", num_clauses);
        exit(0);
    }

    int div = num_clauses / threads_num;
    int remaining = num_clauses % threads_num;
    int init, end = -1;

    for (int i = 0; i < threads_num; i++)
    {
        init = end + 1;
        end = init + div + (remaining-- > 0 ? 0 : -1);

        threads[i].thread_index = i;
        threads[i].start = init;
        threads[i].end = end;
    }
}

void *sat_checker(void *t_params)
{
    ThreadParams *params = ((ThreadParams *)t_params);
    int thread_clauses_amount = params->end - params->start + 1;
    result_elem_t *last_result = NULL;
    input_elem_t *command = NULL;
    int vars[vars_amount];

    while (1)
    {
        sem_wait(&sem_available_inputs[params->thread_index]);
        int false_clauses_count = 0;
        int *false_clauses = (int *) malloc(false_clauses_count);
        int *false_literals_partial = (int *) calloc(vars_amount, sizeof(int));
        command = get_input(command, params->thread_index);

        // Atualiza array de variaveis local (thread)
        if (command->type == CMD_FULL)
        {
            for (int i = 0; i < vars_amount; i++)
            {
                vars[i] = command->full_vars[i];
            }
        } else {
            vars[abs(command->flip_var) - 1] = vars[abs(command->flip_var) - 1] * (-1);
        }

        // Verifica SAT
        for (int clause_index = params->start; clause_index <= params->end; clause_index++)
        {
            int lit;
            int var_index = 0;
            u_int8_t satisfied_clause = 0;

            for (int lit_index = 0; lit_index < clauses[clause_index].lits_amount; lit_index++)
            {
                lit = clauses[clause_index].lits[lit_index];
                int var_index = abs(lit) - 1;

                if (vars[var_index] == lit)
                {
                    satisfied_clause = 1;
                    break;
                }
            }

            if (!satisfied_clause)
            {
                false_clauses_count++;

                false_clauses = (int *) realloc(false_clauses, false_clauses_count * sizeof(int));
                false_clauses[false_clauses_count - 1] = clause_index;

                var_index = 0;
                for (int lit_index = 0; lit_index < clauses[clause_index].lits_amount; lit_index++)
                {
                    lit = clauses[clause_index].lits[lit_index];
                    var_index = abs(lit) - 1;

                    if (vars[var_index] != lit)
                    {
                        false_literals_partial[var_index]++;
                    }
                }
            }
        }

        last_result = enqueue_result(last_result, params->thread_index, false_clauses_count, false_clauses, false_literals_partial, vars);
    }
}

void *show_results()
{
    int results_count = 0;

    while (1)
    {
        sem_wait(&sem_available_results);
        result_elem_t *result = get_result();

        if (result->false_clauses_count > 0)
        {
            printf("[%d clausulas falsas]", result->false_clauses_count);
            for (int i = 0; i < clauses_amount; i++)
                if (result->false_clauses[i])
                    printf(" %d", i);
            printf("\n");

            qsort(result->false_literals_hist, vars_amount, sizeof(FalseLiteral), compare_false_literal);

            printf("[lits]");
            for (int i = 0; i < vars_amount; i++)
            {
                if (result->false_literals_hist[i].count > 0)
                    printf(" %d", result->false_literals_hist[i].value);
            }
            printf("\n");
        }
        else
            printf("SAT\n");

        results_count++;
        if (inputs_finished && inputs_amount == results_count)
            break;
    }
}

int compare_false_literal(const void *literal_a, const void *literal_b)
{
    FalseLiteral false_lit_a = *((FalseLiteral *)literal_a);
    FalseLiteral false_lit_b = *((FalseLiteral *)literal_b);

    return (false_lit_a.count < false_lit_b.count) ||
           ((false_lit_a.count == false_lit_b.count) &&
            (abs(false_lit_a.value) < abs(false_lit_b.value)));
}

void enqueue_input(input_elem_t *input)
{
    if (input != NULL)
        input->reads_remaining = threads_num;

    STAILQ_INSERT_TAIL(&input_queue, input, entries);
    for (int i = 0; i < threads_num; i++)
        sem_post(&sem_available_inputs[i]);
}

input_elem_t *get_input(input_elem_t *previous_input, int thread_index)
{
    sem_wait(&sem_read_input);
    input_elem_t *input;

    if (previous_input != NULL)
        input = STAILQ_NEXT(previous_input, entries);
    else
        input = STAILQ_FIRST(&input_queue);

    if (previous_input != NULL)
    {
        previous_input->reads_remaining--;

        // Remove input anterior se todas as threads ja tiverem o consumido
        if (previous_input->reads_remaining <= 0)
        {
            STAILQ_REMOVE_HEAD(&input_queue, entries);
            free(previous_input);
        }
    }

    sem_post(&sem_read_input);
    return input;
}

result_elem_t *enqueue_result(result_elem_t *previous_result, int thread_index, int false_clauses_count, int *false_clauses, int *false_literals_partial, int *current_vars)
{
    sem_wait(&sem_write_result);
    result_elem_t *result;

    if (previous_result != NULL)
        result = STAILQ_NEXT(previous_result, entries);
    else
        result = STAILQ_FIRST(&result_queue);

    if (result == NULL)
    {
        // Resultado nao comecou a ser ecrito por nenhuma thread
        result = (result_elem_t *)malloc(sizeof(result_elem_t));
        result->false_clauses = (int *) calloc(clauses_amount, sizeof(int));
        result->false_literals_hist = (FalseLiteral *) calloc(vars_amount, sizeof(FalseLiteral));
        result->false_clauses_count = 0;
        result->written_count = 0;

        STAILQ_INSERT_TAIL(&result_queue, result, entries);
    }

    result->false_clauses_count += false_clauses_count;
    for (int i = 0; i < false_clauses_count; i++) {
        result->false_clauses[false_clauses[i]] = 1;
    }
    for (int i = 0; i < vars_amount; i++) {
        result->false_literals_hist[i].value = current_vars[i] * (-1);
        result->false_literals_hist[i].count += false_literals_partial[i];
    }

    result->written_count++;
    if (result->written_count >= threads_num)
        sem_post(&sem_available_results);
    sem_post(&sem_write_result);

    return result;
}

result_elem_t *get_result()
{
    result_elem_t *elem = STAILQ_FIRST(&result_queue);
    STAILQ_REMOVE_HEAD(&result_queue, entries);
    return elem;
}
