#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <map>
#include <algorithm>
#include<stack>
using namespace std;
using ii = pair<int, int>;
int v, c;
vector<vector<int>> cl;
vector<int> values;
void read_input();
void process_full();
void process_flip();
void calculate();

int main()
{
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
    return 0;
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

void calculate()
{
    int i = 0;
    vector<int> wrong;
    map<int, int> falses;
    for(auto &clause:cl){
        bool res = false;
        stack<int> q;
        for(auto &x:clause){
            if(values[abs(x)] == 0 and x<0)
                res = true;
            else if(values[abs(x)] != 0 and x>0)
                res = true;
            else
                q.push(x);
        }
        if(!res){
            wrong.push_back(i);
            while(not q.empty()){
                ++falses[q.top()];
                q.pop();
            }
        }
        ++i;
    }
    vector<ii> lits;
    for(auto x:falses){
        lits.emplace_back(x.second, x.first);
    }
    sort(lits.begin(), lits.end(), [](ii a, ii b){
        return a.first == b.first ? abs(a.second) > abs(b.second) : a.first > b.first;
    });
    if(wrong.empty()){
        cout << "SAT\n";
    }
    else{
        cout << "[" << wrong.size() << " clausulas falsas]";
        for(auto &x:wrong)
            cout << ' ' << x;
        cout << '\n';
        cout << "[lits]";
        for(auto x:lits)
            cout << ' ' << x.second;
        cout << '\n'; 
    }
}
