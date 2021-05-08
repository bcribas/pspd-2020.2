#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

#define NUM_THREADS 12
#define BLOCK 12000

using namespace std;

pthread_t tid[NUM_THREADS];

int arr[NUM_THREADS];

int numberClauses = 0, numberVariables = 0, numberValuations = 0;
vector<map<int,int>> valuations;
vector<vector<int>> clauses;

int arrNumberNotSatisfiedClauses[BLOCK];
vector<int> arrIndexesOfClausesNotSatisfied[BLOCK];
vector<pair<int, int> > arrSortedLits[BLOCK];

bool comparator(pair<int, int> a, pair<int, int> b) { 
    return a.second != b.second ? a.second > b.second : abs(a.first) > abs(b.first);
} 
  
vector<pair<int, int> > sortLits(map<int, int> lits) { 
  
    vector<pair<int, int> > sortedPair;
    for (auto it : lits) { 
        sortedPair.push_back(it); 
    } 
  
    sort(sortedPair.begin(), sortedPair.end(), comparator); 

    return sortedPair;
} 

void printOutput(int numberNotSatisfiedClauses, vector<int> indexesOfClausesNotSatisfied, vector<pair<int, int> > sortedLits) {
    if (numberNotSatisfiedClauses == 0) {
        printf("SAT\n");
    } else {
        printf("[%d clausulas falsas]", numberNotSatisfiedClauses);
        for (int index : indexesOfClausesNotSatisfied) {
            printf(" %d", index);
        }
        printf("\n");

        printf("[lits]");
        for (auto& it : sortedLits) {
            printf(" %d", it.first);
        }
        printf("\n");
    }
}

void verifyValuationToClauses(int iteration) {
    int numberNotSatisfiedClauses = 0;
    map<int, int> lits;
    vector<int> indexesOfClausesNotSatisfied;
    for (int i = 0; i < numberClauses; i++) {
        bool satisfied = false;
        vector<int> litsTemp;
        for (int variable : clauses[i]) {
            if (valuations[iteration][abs(variable)] == variable) {
                satisfied = true;
                break;
            } else {
                litsTemp.push_back(variable);
            }
        }
        if (!satisfied) {
            numberNotSatisfiedClauses++;
            indexesOfClausesNotSatisfied.push_back(i);
            for (int lit : litsTemp) {
                lits[lit]++;
            }
        }
    }

    vector<pair<int, int> > sortedLits = sortLits(lits);

    arrNumberNotSatisfiedClauses[iteration] = numberNotSatisfiedClauses;
    arrIndexesOfClausesNotSatisfied[iteration] = indexesOfClausesNotSatisfied;
    arrSortedLits[iteration] = sortedLits;
}

void *threadFunction(void* arg) {
    int turn = *(int*)arg;

    while (1) {
        if (turn >= numberValuations) {
            pthread_exit(NULL);
        }
        verifyValuationToClauses(turn);
        turn += NUM_THREADS;
    }
}

int main() {
    int temp;
    int scanfReturn;
    scanfReturn = scanf("%d", &numberVariables);
    if (scanfReturn != 1) {
        return 0;
    }
    scanfReturn = scanf("%d", &numberClauses);
    if (scanfReturn != 1) {
        return 0;
    }

    for (int i = 0; i < numberClauses; i++) {
        vector<int> auxVec;
        while (true) {
            scanfReturn = scanf("%d", &temp);
            if (temp == 0) {
                break;
            }
            auxVec.push_back(temp);
        }
        clauses.push_back(auxVec);
    }

    char reader[5];
    map<int, int> valuationsMap;
    while (1) {
        int ret = scanf("%s", reader);
        if (ret != EOF) {
            if (strcmp("full", reader) == 0) {
                for (int i = 1; i <= numberVariables; i++) {
                    scanfReturn = scanf("%d", &temp);
                    valuationsMap[i] = temp;
                }
            } else if (strcmp("flip", reader) == 0) {
                scanfReturn = scanf("%d", &temp);
                int temporaryValue = valuationsMap[temp];
                valuationsMap[temp] = -temporaryValue;
            }
            valuations.push_back(valuationsMap);
            numberValuations++;
        }

        if (ret == EOF || numberValuations == BLOCK) {
            for (int i = 0; i < NUM_THREADS; i++) {
                arr[i] = i;
                pthread_create(&tid[i], NULL, &threadFunction, (void*)&arr[i]);
            }
        
            for (int i = 0; i < NUM_THREADS; i++) {
                pthread_join(tid[i], NULL);
            }

            for (int i = 0; i < numberValuations; i++) {
                printOutput(arrNumberNotSatisfiedClauses[i], arrIndexesOfClausesNotSatisfied[i], arrSortedLits[i]);
            }
            
            if (ret == EOF) {
                break;
            }
            valuations.clear();
            numberValuations = 0;
        }
    }

    return 0;
}
