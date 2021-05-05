// g++ -o prog -O2 -static -Wshadow -Wall -pedantic -pthread -lm main.cpp
#include <bits/stdc++.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_THREADS 1

using namespace std;

pthread_mutex_t lock_vec;
sem_t lock_get[MAX_THREADS], lock_post[MAX_THREADS];

typedef long long ll;

vector<bool> false_vector;

vector<vector<ll>> c_vector;
vector<pair<bool, ll>> v_vector;  // Com valores puros
ll counter;
int running = 1;

bool check_literal_value(ll value) { return (value < 0) ? false : true; }

bool check_logical_value(ll value) {
    if (value > 0) {
        return v_vector[value].first;
    } else {
        return !v_vector[abs(value)].first;
    }
}

bool compare_lits(pair<ll, ll> a, pair<ll, ll> b) {
    if (a.second > b.second) {
        return true;
    } else if (a.second < b.second) {
        return false;
    }
    return (abs(a.first) > abs(b.first));
}

void print_lits(ll v_size) {
    vector<pair<ll, ll>> lits;
    for (ll i = 1; i <= v_size; i++) {
        if (v_vector[i].second > 0) {
            ll result = v_vector[i].first ? i * (-1) : i;
            lits.push_back(make_pair(result, v_vector[i].second));
        }
    }
    sort(lits.begin(), lits.end(), compare_lits);
    for (auto item : lits) {
        printf(" %lld", item.first);
    }
}

typedef struct Limits {
    ll inf_limit;
    ll sup_limit;
    int idx;
} Limits;

void *valorate(void *args) {
    Limits *limits = (Limits *)args;

    while (1) {
        sem_wait(&lock_post[limits->idx]);
        if (!running) break;

        for (ll i = limits->inf_limit; i < limits->sup_limit; i++) {
            bool flag = false;

            for (auto v : c_vector[i]) {
                if (check_logical_value(v)) {
                    flag = true;
                    break;
                }
            }

            if (!flag) {
                false_vector[i] = true;
                pthread_mutex_lock(&lock_vec);
                counter++;
                for (auto v : c_vector[i]) {
                    v_vector[abs(v)].second++;
                }
                pthread_mutex_unlock(&lock_vec);
            }
        }

        sem_post(&lock_get[limits->idx]);
    }

    return NULL;
}

int main() {
    pthread_t tid[MAX_THREADS];
    int num_threads = MAX_THREADS;

    char command[5];
    ll v_size, c_size;

    cin >> v_size >> c_size;

    if (c_size < MAX_THREADS) num_threads = c_size;

    int interval = c_size / num_threads;

    Limits limits[MAX_THREADS];

    int pos = 0;

    for (int i = 0; i < num_threads; i++) {
        limits[i].inf_limit = pos;

        if (i == num_threads - 1)
            limits[i].sup_limit = c_size;
        else
            limits[i].sup_limit = pos + interval;

        limits[i].idx = i;
        pos += interval;
    }

    for (int i = 0; i < num_threads; i++) {
        sem_init(&lock_post[i], 0, 0);
        sem_init(&lock_get[i], 0, 0);
        pthread_create(&tid[i], NULL, &valorate, &limits[i]);
    }

    for (ll i = 0; i < c_size; i++) {
        vector<ll> aux_vec;
        ll aux_value;

        false_vector.push_back(false);

        scanf("%lld", &aux_value);

        while (aux_value != 0) {
            aux_vec.push_back(aux_value);
            scanf("%lld", &aux_value);
        }

        c_vector.push_back(aux_vec);
        false_vector.push_back(false);
    }

    while (scanf("%s", command) != EOF) {
        counter = 0;
        if (strcmp(command, "full") == 0) {
            v_vector.clear();
            v_vector.push_back(make_pair(false, 0));  // Inicializa a 1 posição

            bool value;
            ll aux;
            pair<bool, ll> v_pair;
            for (ll i = 0; i < v_size; i++) {
                scanf("%lld", &aux);

                value = check_literal_value(aux);
                v_pair = make_pair(value, 0);
                v_vector.push_back(v_pair);
            }
        } else {
            ll idx;

            scanf("%lld", &idx);
            v_vector[idx].first = !v_vector[idx].first;
        }

        for (int i = 0; i < num_threads; i++) sem_post(&lock_post[i]);
        for (int i = 0; i < num_threads; i++) sem_wait(&lock_get[i]);

        if (counter > 0) {
            
            printf("[%lld clausulas falsas]", counter);
            for (ll i=0; i<(ll)false_vector.size(); i++){
                if(false_vector[i]){
                    printf(" %lld", i);
                    false_vector[i] = false;
                }
            }

            printf("\n[lits]");
            print_lits(v_size);
            printf("\n");

        } else {
            printf("SAT\n");
        }

        for (ll i = 0; i <= v_size; i++) {
            v_vector[i].second = 0;
        }
    }

    running = 0;

    for (int i = 0; i < num_threads; i++) sem_post(&lock_post[i]);
    for (int i = 0; i < num_threads; i++) pthread_join(tid[i], NULL);

    return 0;
}