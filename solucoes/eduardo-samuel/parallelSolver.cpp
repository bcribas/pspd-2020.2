#include <iostream>
#include <iterator>
#include <cmath>
#include <map>
#include <set>
#include <vector>
#include <pthread.h>
#include <cstdio>
#include <algorithm>
#include <unistd.h>

using namespace std;

#define WORKER_THREADS_NUM 3

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

class Statement {
	public:
		static map<int, int> occurrences;
		
		Statement() {
			this->result = false;
		}
		
		void addVariable(int var) {
			if(var > 0)
				this->expression.insert(pair<int, bool>(var, true));
				
			else
				this->expression.insert(pair<int, bool>(var, false));
		}
		
		void full(int key) {
			this->iterator = expression.find(key);
			if(this->iterator != this->expression.end()) {
				this->iterator->second = true;
				return;
			}
			this->iterator = expression.find(key * (-1));
			if(this->iterator != this->expression.end()) {
				this->iterator->second = false;
				return;
			}
		}
		
		void flip(int key) {
			this->iterator = expression.find(key);
			if(this->iterator != this->expression.end())
				this->iterator->second = !this->iterator->second;

			this->iterator = expression.find((-1) * key);
			if(this->iterator != this->expression.end())
				this->iterator->second = !this->iterator->second;
		}
		
		void generateResult() {
			for(pair<int, bool> e : this->expression){
				this->result = this->result || e.second;
				if(this->result) break;
			}
			this->solution.push_back(this->result);
			this->result = false;
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
		
		bool getResult(int index) {
			return this->solution[index];
		}
		
	private:
		map<int, bool> expression;
		map<int, bool>::iterator iterator;
		map<int, int>::iterator occurrenceIterator;
		bool result;
		vector<bool> solution;
};

typedef struct Argument {
	int start, end, threadId;
} Argument;

typedef struct Command {
	int type;
} Command;

typedef struct FlipCommand : Command {
	int value;
} FlipCommand;

typedef struct FullCommand : Command {
	vector<int> values;
} FullCommand;

vector<Statement*> statements;
vector<Command*> commands;
int currentCommand[WORKER_THREADS_NUM];

bool checkThreadsIteration(int iteration) {
	for(int c = 0; c < WORKER_THREADS_NUM; c++) {
		if(currentCommand[c] <= iteration)
			return false;
	}

	return true;
}

void printIteration(int iteration) {
	int currentStatement = 0;
	vector<int> falseStatements;

	for(Statement* &statement : statements) {
		if(!statement->getResult(iteration)) {
			statement->addOccurrences();

			falseStatements.push_back(currentStatement);
		}

		currentStatement++;
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
		for(vector<pair<int,int>>::iterator it = sorted->begin(); it != sorted->end(); ++it) {
			printf(" %d",it->first);
		}
		printf("\n");
	}

	Statement::occurrences.clear();
}

void *printResults(void *args) {
	int commandsNum = commands.size();

	for(int c = 0; c < commandsNum; c++) {
		while(!checkThreadsIteration(c)) { usleep(1); }

		printIteration(c);
	}

	return NULL;
}

void *calculateResults(void *args) {
	Argument *arg = (Argument*) args;
	FullCommand *fullCommand;
	FlipCommand *flipCommand;

	for(Command *&command : commands) {
		if(command->type) {
			for(int c = arg->start; c < arg->end; c++) {
				fullCommand = (FullCommand*) command;
				for(int &value : fullCommand->values)
					statements[c]->full(value);

				statements[c]->generateResult();
			}
		}

		else {
			for(int c = arg->start; c < arg->end; c++) {
				flipCommand = (FlipCommand*) command;
				statements[c]->flip(flipCommand->value);

				statements[c]->generateResult();
			}
		}

		currentCommand[arg->threadId]++;
	}
	
	return NULL;
}

map<int,int> Statement::occurrences = {};

int main() {
	int V, C, c, value, variable;
	string command;
	pthread_t threads[WORKER_THREADS_NUM];
	pthread_t printThread;
	Argument arguments[WORKER_THREADS_NUM];
	
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
			FullCommand *fullCommand = new FullCommand();
			fullCommand->type = 1;
			for(c = 0; c < V; c++) {
				cin >> value;
				fullCommand->values.push_back(value);
			}

			commands.push_back(fullCommand);
		}

		else {
			FlipCommand *flipCommand = new FlipCommand();
			flipCommand->type = 0;
			cin >> flipCommand->value;
			
			commands.push_back(flipCommand);
		}
	}

	const int step = C/WORKER_THREADS_NUM;

	for(c = 0; c < WORKER_THREADS_NUM - 1; c++) {
		arguments[c].start = step * c;
		arguments[c].end = step * (c + 1);
		arguments[c].threadId = c;
		pthread_create(threads + c, NULL, &calculateResults, (void*) (arguments + c));
	}

	arguments[c].start = step * c;
	arguments[c].end = C;
	arguments[c].threadId = c;
	pthread_create(threads + c, NULL, &calculateResults, (void*) (arguments + c));

	pthread_create(&printThread, NULL, &printResults, NULL);

	for(c = 0; c < WORKER_THREADS_NUM; c++)
		pthread_join(threads[c], NULL);

	pthread_join(printThread, NULL);

	for(Statement* &statement : statements) {
		delete statement;
	}

	for(Command* &command : commands)
		delete command;
	
	return 0;
}