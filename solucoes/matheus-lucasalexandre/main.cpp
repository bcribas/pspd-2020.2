#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <stdlib.h>
using namespace std;

int v, c, number, counter = 1, flipNumber, falseFlag = 0, falseCounter = 0;
vector<int> values;
vector<int> clauses;
vector<int> failedClauses;
vector<vector<int>> listOfClauses;
vector<int>::iterator it;
vector<int>::iterator it2;
vector<int> lits;
list<int> removeFailed;

string command;

bool compare(pair<int, int> &p1,
             pair<int, int> &p2)
{
  if (p1.second == p2.second)
  {
    return abs(p1.first) > abs(p2.first);
  }
  return p1.second > p2.second;
}

void printSorted(vector<int> param)
{
  map<int, int> m;
  for (auto aux : param)
    m[aux]++;
  int s = m.size();
  pair<int, int> p[s];

  int i = 0;
  for (auto it = m.begin(); it != m.end(); ++it)
    p[i++] = make_pair(it->first, it->second);

  sort(p, p + s, compare);
  for (int i = 0; i < s; i++)
  {
    printf(" ");
    printf("%i", p[i].first);
  }
}

void full()
{
  for (int i = 0; i < listOfClauses.size(); i++)
  {
    for (auto clauseValue : listOfClauses[i])
    {
      it = find(values.begin(), values.end(), clauseValue);
      if (it != values.end())
      {
        falseFlag = 0;
        break;
      }
      else
      {
        falseFlag = 1;
      }
    }
    if (falseFlag == 1)
    {
      falseCounter++;
      it2 = find(failedClauses.begin(), failedClauses.end(), i);
      if (it2 == failedClauses.end())
      {
        for (auto aux : listOfClauses[i])
        {
          lits.push_back(aux);
        }
        failedClauses.push_back(i);
      }
    }
  }
  if (falseCounter > 0)
  {
    printf("[%d clausulas falsas]", falseCounter);
    sort(failedClauses.begin(), failedClauses.end());
    for (auto index : failedClauses)
    {
      printf(" ");
      printf("%d", index);
    }
    printf("\n");
    printf("[lits]");
    int n = sizeof(lits) / sizeof(lits[0]);
    printSorted(lits);
    printf("\n");
  }
  falseCounter = 0;
  falseFlag = 0;
}

void flip()
{
  lits.clear();
  vector<int> aux;
  int h = 0;
  int k = 0;
  bool verificador;
  values[flipNumber - 1] = values[flipNumber - 1] * -1;
  for (auto c : failedClauses)
  {
    for (auto i : listOfClauses[c])
    {
      if (i == values[flipNumber - 1])
      {
        removeFailed.push_back(c);
      }
    }
  }
  for (auto y : removeFailed)
  {
    failedClauses.erase(remove(failedClauses.begin(), failedClauses.end(), y), failedClauses.end());
  }
  removeFailed.clear();
  for (auto i : listOfClauses)
  {
    if (find(i.begin(), i.end(), (values[flipNumber - 1] * -1)) != i.end())
    {
      for (auto v : values)
      {
        if (v != flipNumber * -1)
        {
          if (find(i.begin(), i.end(), v) != i.end())
          {
            verificador = true;
            break;
          }
          else
          {
            verificador = false;
          }
        }
      }
      if (!verificador)
      {
        failedClauses.push_back(h);
      }
    }
    h++;
  }
  if (failedClauses.size() > 0)
  {
    for (auto i : failedClauses)
    {
      for (auto k : listOfClauses[i])
      {
        lits.push_back(k);
      }
    }
    printf("[%zu clausulas falsas]", failedClauses.size());
    aux = failedClauses;
    sort(aux.begin(), aux.end());
    for (auto index : aux)
    {
      printf(" ");
      printf("%d", index);
    }
    printf("\n");
    printf("[lits]");
    int n = sizeof(lits) / sizeof(lits[0]);
    printSorted(lits);
    printf("\n");
  }
  else
  {
    printf("SAT");
  }
  lits.clear();
}

int main()
{

  cin >> v >> c;

  while (counter <= c)
  {
    while (cin >> number && number != 0)
    {
      clauses.push_back(number);
    }
    listOfClauses.push_back(clauses);
    clauses.clear();
    counter++;
  }

  cin >> command;

  for (int i = 0; i < v; i++)
  {
    int aux;
    cin >> aux;
    values.push_back(aux);
  }

  full();

  while (cin >> command)
  {
    if (command.compare("flip") == 0)
    {
      cin >> flipNumber;
      flip();
    }
    else if (command.compare("full") == 0)
    {
      values.clear();
      for (int i = 0; i < v; i++)
      {
        int aux;
        cin >> aux;
        values.push_back(aux);
      }
      failedClauses.clear();
      full();
    }
  }
  return 0;
}