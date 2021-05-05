#include <iostream>
#include <iterator>
#include <cmath>
#include <map>
#include <set>
#include <vector>
#include <pthread.h>
#include <cstdio>
#include <algorithm>

using namespace std;

bool compareValues(pair<int, int> a, pair<int, int> b) {
	if(a.second > b.second)
		return true;
	if(a.second == b.second) {
		if(abs(a.first) > abs(b.first))
			return true;
		return false;
	}

	return false;
}

vector<pair<int, int>>* sortValues(map<int, int> &x) {
	vector<pair<int, int>> *aux = new vector<pair<int, int>>();

	for(map<int, int>::iterator value = x.begin(); value != x.end(); ++value)
		aux->push_back(pair<int, int>(value->first, value->second));

	sort(aux->begin(), aux->end(), compareValues);
	return aux;
}

void printResult();
class Statement {
	public:
		static map<int, int> occurrences;
		
		Statement() {
			this->result = false;
			this->trueVariables = 0;
		}
		
		void addVariable(int var) {
			if(var > 0) {
				this->expression.insert(pair<int, bool>(var, true));
				this->trueVariables++;
			}
				
			else
				this->expression.insert(pair<int, bool>(var, false));
		}
		
		void full(int key) {
			this->iterator = expression.find(key);
			if(this->iterator != this->expression.end()) {
				if(!this->iterator->second)
					this->trueVariables++;
				this->iterator->second = true;
				return;
			}
			this->iterator = expression.find(key * (-1));
			if(this->iterator != this->expression.end()) {
				if(this->iterator->second)
					this->trueVariables--;
				this->iterator->second = false;
				return;
			}
		}
		
		void flip(int key) {
			this->iterator = expression.find(key);
			if(this->iterator != this->expression.end()) {
				if(this->iterator->second)
					this->trueVariables--;
				else
					this->trueVariables++;

				this->iterator->second = !this->iterator->second;
			}

			this->iterator = expression.find((-1) * key);
			if(this->iterator != this->expression.end()) {
				if(this->iterator->second)
					this->trueVariables--;
				else
					this->trueVariables++;

				this->iterator->second = !this->iterator->second;
			}
		}
		
		void addOccurrences() {
			int variable;
			for(map<int, bool>::iterator occurrencePair = this->expression.begin(); occurrencePair != this->expression.end(); ++occurrencePair) {
				variable = occurrencePair->first;
				if((this->occurrenceIterator = Statement::occurrences.find(variable)) != Statement::occurrences.end())
					this->occurrenceIterator->second++;
				else
					Statement::occurrences.insert(pair<int, int>(variable, 1));
			}
		}
		
		bool getResult() {
			if(this->trueVariables > 0)
				return true;
			return false;
		}
		
	private:
		map<int, bool> expression;
		map<int, bool>::iterator iterator;
		map<int, int>::iterator occurrenceIterator;
		bool result;
		int trueVariables;
};

vector<Statement*> statements;
int C, V;

void flip(int value) {
	for(int c = 0; c < C; c++)
		statements[c]->flip(value);
}

void full(int value) {
	for(int c = 0; c < C; c++)
		statements[c]->full(value);
}

void printIteration() {
	vector<int> falseStatements;

	for(int c = 0; c < C; c++) {
		if(!statements[c]->getResult()) {
			statements[c]->addOccurrences();

			falseStatements.push_back(c);
		}
	}

	int falseCount = falseStatements.size();
	if(!falseCount)
		printf("SAT\n");
	else {
		printf("[%d clausulas falsas]", falseCount);

		for(int &k : falseStatements)
			printf(" %d", k);
		printf("\n");
		printf("[lits]");

		vector<pair<int,int>> * sorted = sortValues(Statement::occurrences);
		for(vector<pair<int,int>>::iterator it = sorted->begin(); it != sorted->end(); ++it){
			printf(" %d",it->first);
		}
		printf("\n");
	}

	Statement::occurrences.clear();
}

map<int,int> Statement::occurrences = {};

int main() {
	int c, value, variable;
	string command;
	
	cin >> V;
	cin >> C;
	
	for(c = 0; c < C; c++) {
		Statement *statement = new Statement();
		cin >> variable;
		while(variable) {
			statement->addVariable(variable);
			cin >> variable;
		}

		statements.push_back(statement);
	}
	
	while(cin >> command) {
		if(command == "full") {
			for(c = 0; c < V; c++) {
				cin >> value;
				full(value);
			}
		}

		else {
			cin >> value;
			flip(value);
		}

		printIteration();
	}
	
	return 0;
}