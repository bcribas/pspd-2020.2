CC = g++
REQ_FLAGS = -O2 -static
ADD_FLAGS = -lpthread -std=c++14
FLAGS = $(REQ_FLAGS) $(ADD_FLAGS)
MAIN = versao_paralelismo.cpp
EXE = exec 

all: clean 
	$(CC) $(MAIN) $(FLAGS) -o $(EXE)  


clean:
	-rm -f $(EXE)

run:
	./$(EXE)