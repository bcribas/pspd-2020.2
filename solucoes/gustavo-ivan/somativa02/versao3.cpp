#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <set>
using namespace std;

int NVariable, NClause;
vector<unordered_map<int, int>> clauses;
vector<bool> valueOf;
vector<int> NTrueVariables;
set<int> fausesClauses;
unordered_map<int, int> falssesVariables;

void full();
void flip();
void calculate();

int main()
{
    scanf("%d %d", &NVariable, &NClause);

    for (int i = 0; i < NClause; ++i)
    {
        int x;
        unordered_map<int, int> mp;
        while (true)
        {
            scanf("%d", &x);
            if (x == 0)
                break;
            ++mp[x];
        }
        clauses.push_back(mp);
    }
    char command[5];
    while (scanf("%s", command) != EOF)
    {
        if (strcmp(command, "full") == 0)
        {
            full();
        }
        if (strcmp(command, "flip") == 0)
        {
            flip();
        }
        calculate();
    }
    return 0;
}

void full()
{
    fausesClauses.clear();
    falssesVariables.clear();
    valueOf.assign(NVariable, false);
    NTrueVariables.assign(NClause, 0);

    for (int i = 0; i < NVariable; ++i)
    {
        int x;
        scanf("%d", &x);
        if (x > 0)
            valueOf[abs(x)] = true;
        else
            valueOf[abs(x)] = false;
    }
    for (int i = 0; i < NClause; ++i)
    {
        for (auto x : clauses[i])
        {
            int variable = x.first;
            if (variable > 0 and valueOf[abs(variable)] == true)
                NTrueVariables[i] += x.second;

            if (variable < 0 and valueOf[abs(variable)] == false)
                NTrueVariables[i] += x.second;
        }
        if (not clauses[i].empty() and NTrueVariables[i] == 0)
        {
            fausesClauses.insert(i);
            for (auto x : clauses[i])
                falssesVariables[x.first] += x.second;
        }
    }
}

void flip()
{
    int x;
    scanf("%d", &x);
    valueOf[x] = !valueOf[x];

    for (int i = 0; i < NClause; ++i)
    {
        bool findPositive = clauses[i].find(x) != clauses[i].end();
        bool findNegative = clauses[i].find(-x) != clauses[i].end();

        if (clauses[i].empty() or (not findPositive and not findNegative))
            continue;

        if (findPositive)
        {
            if (valueOf[x] == true)
                NTrueVariables[i] += clauses[i][x];
            else
                NTrueVariables[i] -= clauses[i][x];
        }
        if (findNegative)
        {
            if (valueOf[x] == false)
                NTrueVariables[i] += clauses[i][-x];
            else
                NTrueVariables[i] -= clauses[i][-x];
        }

        if (NTrueVariables[i] == 0 and fausesClauses.find(i) == fausesClauses.end())
        {
            fausesClauses.insert(i);
            for (auto y : clauses[i])
                falssesVariables[y.first] += y.second;
        }
        if (NTrueVariables[i] != 0 and fausesClauses.find(i) != fausesClauses.end())
        {
            fausesClauses.erase(i);
            for (auto y : clauses[i])
                falssesVariables[y.first] -= y.second;
        }
    }
}

void calculate()
{
    if (fausesClauses.empty())
    {
        printf("SAT\n");
        return;
    }
    /* Printing falses clauses */
    printf("[%d clausulas falsas]", (int)fausesClauses.size());
    for (auto i : fausesClauses)
        printf(" %d", i);
    printf("\n");

    vector<pair<int, int>> lits;
    for (auto x : falssesVariables)
    {
        if (x.second > 0)
            lits.emplace_back(x.first, x.second);
    }
    sort(lits.begin(), lits.end(), [](pair<int, int> a, pair<int, int> b) {
        return a.second == b.second ? abs(a.first) > abs(b.first) : a.second > b.second;
    });
    printf("[lits]");
    for (auto x : lits)
        printf(" %d", x.first);
    printf("\n");
}
