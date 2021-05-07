#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <stack>
#include <thread>
#include <mutex>
#include <semaphore.h>

#define N_THREADS 4
using namespace std; 

using ii = pair<int, int>;
int v, c;
sem_t c_falses_mtx, v_falses_mtx;
vector<vector<int>> cl;
vector<int> values;
vector<int> c_falses;
unordered_map<int, int> v_falses;
pthread_t threads[N_THREADS];
int args[N_THREADS];
void read_input();
void process_full();
void process_flip();
void calculate();
void* thread_handler(void* _);

int main()
{
    sem_init(&c_falses_mtx, 0, 1);
    sem_init(&v_falses_mtx, 0, 1);
    ios::sync_with_stdio(0);
    cin.tie(0);
    read_input();
    string command;
    while(cin >> command){
        if(command == "full"){
            process_full();
        }
        else{
            process_flip();
        }
        calculate();
    }
}

void read_input()
{
    cin >> v >> c;
    values = vector<int>(v+1);
    cl = vector<vector<int>>(c);
    for (auto &vs : cl)
    {
        int x;
        while (cin >> x, x != 0)
            vs.push_back(x);
    }
}

void process_full(){
    for(int i=1; i<=v; ++i){
        int x;
        cin >> x;
        values[abs(x)] = x<0 ? 0 : 1;
    }
}
void process_flip(){
    int x;
    cin >> x;
    if(values[x] == 0){
        values[x] = 1;
    }
    else{
        values[x] = 0;
    }
}   


void* thread_handler(void* arg){
    int k = *((int *)arg);
    int blk = (int)cl.size()/N_THREADS;
    int l = k*blk;
    int r = (k == N_THREADS-1) ? cl.size() : l+blk;
    
    for(int i=l; i<r; ++i)
    {
        bool res = false;
        stack<int> q;
        for (auto &x : cl[i])
        {
            if (values[abs(x)] == 0 and x < 0)
                res = true;
            else if (values[abs(x)] != 0 and x > 0)
                res = true;
            else
                q.push(x);
        }
        if (!res)
        {
            sem_wait(&c_falses_mtx);
            c_falses.push_back(i);
            sem_post(&c_falses_mtx);

            while (not q.empty())
            {
                sem_wait(&v_falses_mtx);
                ++v_falses[q.top()];
                sem_post(&v_falses_mtx);
                q.pop();
            }
        }
    }
    return NULL;
}
void calculate()
{
    v_falses.clear();
    c_falses.clear();
    
    for(int i=0; i<N_THREADS; i++){
        args[i] = i;
        pthread_create(&threads[i],NULL,thread_handler, (void *)&args[i]);
    }
    for(int i=0;i<N_THREADS;i++){
        pthread_join(threads[i],NULL);
    }
    vector<ii> lits;
    for (auto x : v_falses)
    {
        lits.emplace_back(x.second, x.first);
    }
    sort(lits.begin(), lits.end(), [](ii a, ii b) {
        return a.first == b.first ? abs(a.second) > abs(b.second) : a.first > b.first;
    });
    sort(c_falses.begin(), c_falses.end());
    if (c_falses.empty())
    {
        cout << "SAT\n";
    }
    else
    {
        cout << "[" << c_falses.size() << " clausulas falsas]";
        for (auto &x : c_falses)
            cout << ' ' << x;
        cout << '\n';
        cout << "[lits]";
        for (auto x : lits)
            cout << ' ' << x.second;
        cout << '\n';
    }
}
