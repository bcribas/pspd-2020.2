#include <bits/stdc++.h>
#include <semaphore.h>
#define N_THREADS 4

using namespace std;

int n_variables, n_clauses;
sem_t sem_clause, sem_variable;

vector<vector<int>> clauses;
vector<bool> value_of;
vector<int> true_in_clause;
set<int> false_clauses;
map<int, int> false_variables;
map<int, vector<int>> variable_in_clauses;
vector<pair<int, int>> variable_to_process;

void full();
void flip();
void resultado();

int main()
{
    sem_init(&sem_clause, 0, 1);
    sem_init(&sem_variable, 0, 1);

    cin >> n_variables >> n_clauses;
    value_of.assign(n_variables + 1, 0);
    true_in_clause.assign(n_clauses, -1);

    for (int i = 0; i < n_clauses; i++)
    {
        /* Resolvendo literais repetidos e tautologia */
        int x;
        vector<int> temp_clause;
        set<int> st;
        bool taut = false;
        while (cin >> x, x != 0)
        {
            if (st.find(-x) != st.end())
                taut = true;
            st.insert(x);
        }
        if (!taut)
        {
            for (auto t : st)
            {
                temp_clause.push_back(t);
                variable_in_clauses[t].push_back(i);
            }
        }
        clauses.push_back(temp_clause);
    }

    string s;
    while (cin >> s)
    {
        if (s == "full")
        {
            full();
        }
        else if (s == "flip")
        {
            flip();
        }
        resultado();
    }
    return 0;
}

void *process_full(void *arg)
{
    int t_index = *((int *)arg);
    int blk = (int)clauses.size() / N_THREADS;
    int l = t_index * blk;
    int r = (t_index == N_THREADS - 1) ? clauses.size() : l + blk;
    if (blk == 0)
    {
        if ((t_index == N_THREADS - 1))
        {
            l = 0;
            r = clauses.size();
        }
        else
        {
            return NULL;
        }
    }

    map<int, int> false_variables_to_add;
    vector<int> false_clauses_to_insert;
    vector<int> false_clauses_to_erase;
    for (int i = l; i < r; i++)
    {
        int last_true_value = true_in_clause[i];
        true_in_clause[i] = 0;
        for (size_t j = 0; j < clauses[i].size(); j++)
        {
            int t = clauses[i][j];
            if ((t < 0 and value_of[abs(t)] == 0) or (t > 0 and value_of[t] == 1))
            {
                true_in_clause[i] += 1;
            }
        }
        if (true_in_clause[i] == 0)
        {
            false_clauses_to_insert.push_back(i);
            if (last_true_value != 0)
            {
                for (auto x : clauses[i])
                {
                    ++false_variables_to_add[x];
                }
            }
        }
        else
        {
            false_clauses_to_erase.push_back(i);
            if (last_true_value == 0)
            {
                for (auto x : clauses[i])
                {
                    --false_variables_to_add[x];
                }
            }
        }
    }
    sem_wait(&sem_variable);
    for (auto x : false_variables_to_add)
    {
        false_variables[x.first] += x.second;
        if (false_variables[x.first] == 0)
            false_variables.erase(x.first);
    }
    sem_post(&sem_variable);
    sem_wait(&sem_clause);
    for (auto &x : false_clauses_to_insert)
        false_clauses.insert(x);
    for (auto &x : false_clauses_to_erase)
        false_clauses.erase(x);
    sem_post(&sem_clause);
    return NULL;
}

void *process_flip(void *arg)
{
    int t_index = *((int *)arg);
    int blk = (int)variable_to_process.size() / N_THREADS;
    int l = t_index * blk;
    int r = (t_index == N_THREADS - 1) ? variable_to_process.size() : l + blk;
    if (blk == 0)
    {
        if ((t_index == N_THREADS - 1))
        {
            l = 0;
            r = variable_to_process.size();
        }
        else
        {
            return NULL;
        }
    }

    map<int, int> false_variables_to_add;
    vector<int> false_clauses_to_insert;
    vector<int> false_clauses_to_erase;
    for (int _i = l; _i < r; _i++)
    {
        int i = variable_to_process[_i].first;
        int t = variable_to_process[_i].second;

        int last_true_value = true_in_clause[i];
        if ((t < 0 and value_of[abs(t)] == 0) or (t > 0 and value_of[abs(t)] == 1))
            true_in_clause[i] += 1;
        else
            true_in_clause[i] -= 1;

        if (true_in_clause[i] == 0)
        {
            false_clauses_to_insert.push_back(i);
            if (last_true_value != 0)
            {
                for (auto x : clauses[i])
                {
                    ++false_variables_to_add[x];
                }
            }
        }
        else
        {
            false_clauses_to_erase.push_back(i);
            if (last_true_value == 0)
            {
                for (auto x : clauses[i])
                {
                    --false_variables_to_add[x];
                }
            }
        }
    }
    sem_wait(&sem_variable);
    for (auto x : false_variables_to_add)
    {
        false_variables[x.first] += x.second;
        if (false_variables[x.first] == 0)
            false_variables.erase(x.first);
    }
    sem_post(&sem_variable);
    sem_wait(&sem_clause);
    for (auto &x : false_clauses_to_insert)
        false_clauses.insert(x);
    for (auto &x : false_clauses_to_erase)
        false_clauses.erase(x);
    sem_post(&sem_clause);
    return NULL;
}

void full()
{
    for (int i = 0; i < n_variables; i++)
    {
        int t;
        cin >> t;
        if (t < 0)
            value_of[abs(t)] = 0;
        else
            value_of[abs(t)] = 1;
    }

    pthread_t threads[N_THREADS];
    int args[N_THREADS];

    for (int i = 0; i < N_THREADS; i++)
    {
        args[i] = i;
        pthread_create(&threads[i], NULL, process_full, (void *)&args[i]);
    }
    for (int i = 0; i < N_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
}

void flip()
{
    int t;
    cin >> t;
    int k = abs(t);
    value_of[k] = value_of[k] ? 0 : 1;

    set<int> st;
    for (auto &c : variable_in_clauses[k])
    {
        variable_to_process.emplace_back(c, k);
        st.insert(c);
    }
    for (auto &c : variable_in_clauses[-k])
    {
        variable_to_process.emplace_back(c, -k);
        st.insert(c);
    }

    pthread_t threads[N_THREADS];
    int args[N_THREADS];

    for (int i = 0; i < N_THREADS; i++)
    {
        args[i] = i;
        pthread_create(&threads[i], NULL, process_flip, (void *)&args[i]);
    }
    for (int i = 0; i < N_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    variable_to_process.clear();
}

void resultado()
{
    if (false_clauses.empty())
    {
        cout << "SAT" << '\n';
        return;
    }

    cout << "[" << false_clauses.size() << " clausulas falsas]";
    for (auto const &p : false_clauses)
    {
        cout << ' ' << p;
    }
    cout << '\n';

    vector<pair<int, int>> false_variables_sorted;
    for (auto x : false_variables)
        false_variables_sorted.push_back(x);

    sort(false_variables_sorted.begin(), false_variables_sorted.end(),
         [](const pair<int, int> &a, const pair<int, int> &b) {
             if (a.second != b.second)
             {
                 return a.second > b.second;
             }
             return abs(a.first) > abs(b.first);
         });

    cout << "[lits]";
    for (auto const &p : false_variables_sorted)
    {
        cout << ' ' << p.first;
    }
    cout << '\n';
}