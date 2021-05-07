#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct
{
    int result;
    int lits_amount;
    int *lits;
} Clause;

typedef struct 
{
    int value;
    int count;
} FalseLiteral;


int vars_amount, clauses_amount;
int *vars;
Clause *clauses;

int false_clauses_count = 0;

void read_clauses();
int read_command();
void sat_full(int start, int end, FalseLiteral *false_literals_hist);
void sat_flip(int start, int end, FalseLiteral *false_literals_hist, int flipped_var);
void show_results(FalseLiteral *false_literals_hist);
int compare_false_lit(const void *literal_a, const void *literal_b);

int main(int argc, char *argv[])
{
    scanf("%d %d", &vars_amount, &clauses_amount);

    vars = (int *) calloc(vars_amount, sizeof(int));
    clauses = (Clause *)calloc(clauses_amount, sizeof(Clause));

    read_clauses();

    FalseLiteral false_literals_hist[vars_amount];
    int flipped_var = 0;

    while (1)
    {

        flipped_var = read_command();

        if (flipped_var < 0)
            break;

        // if (1) {
        if (flipped_var == 0)
        {
            false_clauses_count = 0;
            for (int i = 0; i < vars_amount; i++)
            {
                false_literals_hist[i].value = vars[i] * (-1);
                false_literals_hist[i].count = 0;
            }

            sat_full(0, clauses_amount - 1, false_literals_hist);
        }
        else
        {
            false_literals_hist[flipped_var - 1].value = vars[flipped_var - 1] * (-1);
            false_literals_hist[flipped_var - 1].count = 0;

            sat_flip(0, clauses_amount - 1, false_literals_hist, flipped_var);
        }

        show_results(false_literals_hist);
    }
    return 0;
}

void add_lit_sorted_in_clause(int clause_pos, int lit_count, int lit)
{
    int pos = 0;
    int var = abs(lit);
    while (var >= abs(clauses[clause_pos].lits[pos]) && pos < lit_count - 1)
        pos++;

    int new_var = lit;
    int old_var = clauses[clause_pos].lits[pos];
    clauses[clause_pos].lits[pos] = new_var;

    for (int i = pos + 1; i < lit_count; i++)
    {
        new_var = old_var;
        old_var = clauses[clause_pos].lits[i];
        clauses[clause_pos].lits[i] = new_var;
    }
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
            add_lit_sorted_in_clause(clause, clauses[clause].lits_amount, lit);
        }
    }
}

int read_command()
{
    char command[10];
    int var_val = 0;

    if (scanf("%s", command) != EOF)
    {
        if (strcmp("full", command) == 0)
        {
            for (int i = 1; i <= vars_amount; i++)
            {
                scanf("%d", &var_val);

                vars[abs(var_val) - 1] = var_val;
            }
            return 0;
        }
        else if (strcmp("flip", command) == 0)
        {
            scanf("%d", &var_val);
            vars[var_val - 1] = vars[var_val - 1] * (-1);
            return var_val;
        }
    }
    return -1;
}

void sat_full(int start, int end, FalseLiteral *false_literals_hist)
{
    for (int i = start; i <= end; i++)
    {
        int lit;
        u_int8_t satisfied_clause = 0;

        for (int j = 0; j < clauses[i].lits_amount; j++)
        {
            lit = clauses[i].lits[j];
            int var_index = abs(lit) - 1;

            if (vars[var_index] == lit)
            {
                satisfied_clause = 1;
                break;
            }
        }

        clauses[i].result = 1;
        if (!satisfied_clause)
        {
            false_clauses_count++;
            clauses[i].result = 0;

            int var_index = 0;
            for (int j = 0; j < clauses[i].lits_amount; j++)
            {
                lit = clauses[i].lits[j];
                var_index = abs(lit) - 1;

                if (vars[var_index] != lit)
                {
                    false_literals_hist[var_index].count++;
                }
            }
        }
    }
}

int cmp_search(const void *a, const void *b)
{
    return (abs(*(int *)a) - abs(*(int *)b));
}

void sat_flip(int start, int end, FalseLiteral *false_literals_hist, int flipped_var)
{
    for (int i = start; i <= end; i++)
    {
        int *lit_found = (int *)bsearch(&flipped_var, clauses[i].lits, clauses[i].lits_amount, sizeof(int), cmp_search);


        if (lit_found != NULL)
        {
            int lit_true = vars[flipped_var - 1] == *lit_found;
            int lit, clause_lit_pos = 0;


            if (lit_true && clauses[i].result == 0)
            {
                // Se clausula foi alterada para verdadeira, eh necessario retirar os literais que foram contados como falso na rodada passada
                int var_index = 0;


                clauses[i].result = 1;
                false_clauses_count--;
                for (int j = 0; j < clauses[i].lits_amount; j++)
                {
                    lit = clauses[i].lits[j];
                    var_index = abs(lit) - 1;

                    if (vars[var_index] != lit)
                    {
                        false_literals_hist[var_index].count--;
                    }
                }
            }
            else if (!lit_true && clauses[i].result == 1)
            {
                u_int8_t satisfied_clause = 0;

                // Verifica se depois do literal se tornar falso, se ainda existe outro literal verdadeiro para validar a clausula
                for (int j = 0; j < clauses[i].lits_amount; j++)
                {
                    lit = clauses[i].lits[j];
                    int var_index = abs(lit) - 1;

                    if (vars[var_index] == lit)
                    {
                        satisfied_clause = 1;
                        clauses[i].result = 1;
                        break;
                    }
                }

                if (!satisfied_clause)
                {
                    // Se a clausula se tornou falsa, entao é necessario contar todos os literais falsos
                    clauses[i].result = 0;
                    false_clauses_count++;

                    int var_index = 0;
                    for (int j = 0; j < clauses[i].lits_amount; j++)
                    {
                        lit = clauses[i].lits[j];
                        var_index = abs(lit) - 1;

                        if (vars[var_index] != lit)
                        {
                            false_literals_hist[var_index].count++;
                        }
                    }
                }
            } else if (!lit_true && clauses[i].result == 0) {
                // Se clausula ja era falsa e o literal flipado se tornou falso, apenas conte ele no histograma
                false_literals_hist[flipped_var - 1].count++;
            }
        }
    }
}

int **copy_false_literals(int (*false_literals_hist)[2], int rows) {
  int **new_array = (int **)malloc(rows * sizeof(int *));
  
  for (int i = 0; i < rows; i++) {
      new_array[i] = (int *)malloc(2 * sizeof(int));
      new_array[i][0] = false_literals_hist[i][0];
      new_array[i][1] = false_literals_hist[i][1];
  }

  return new_array;
}

void show_results(FalseLiteral *false_literals_hist)
{
    if (false_clauses_count > 0)
    {
        printf("[%d clausulas falsas]", false_clauses_count);
        for (int i = 0; i < clauses_amount; i++)
        {
            if (!clauses[i].result)
                printf(" %d", i);
        }
        printf("\n");

        // COPY FALSE LITERALS ARRAY
        int false_literals_copy[vars_amount][2];

        for (int i = 0; i < vars_amount; i++) {
            false_literals_copy[i][0] = false_literals_hist[i].value;
            false_literals_copy[i][1] = false_literals_hist[i].count;
        }

        qsort(false_literals_copy, vars_amount, sizeof(int) * 2, compare_false_lit);

        printf("[lits]");
        for (int i = 0; i < vars_amount; i++)
        {
            if (false_literals_copy[i][1] > 0)
                printf(" %d", false_literals_copy[i][0]);
        }
        printf("\n");
    }
    else
        printf("SAT\n");
}

int compare_false_lit(const void *literal_a, const void *literal_b)
{
    int a_val = abs(*((int *)literal_a + 0)), a_occurrences = *((int *)literal_a + 1);
    int b_val = abs(*((int *)literal_b + 0)), b_occurrences = *((int *)literal_b + 1);

    return (a_occurrences < b_occurrences) || ((a_occurrences == b_occurrences) && (a_val < b_val));
}

int compare_false_lit_pointer(const void *literal_a, const void *literal_b)
{
    int a_val = abs((*(const int **)literal_a)[0]), a_occurrences = (*(const int **)literal_a)[1];
    int b_val = abs((*(const int **)literal_b)[0]), b_occurrences = (*(const int **)literal_b)[1];

    return (a_occurrences < b_occurrences) || ((a_occurrences == b_occurrences) && (a_val < b_val));
}
