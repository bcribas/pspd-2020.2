CC = g++
REQ_FLAGS = -O2 -static
WAR_FLAGS = -Wshadow -Wall 
ADD_FLAGS = -pedantic -pthread -lm
FLAGS = $(REQ_FLAGS) $(WAR_FLAGS) $(ADD_FLAGS)
MAIN = main.cpp
EXE = prog 

all: clean 
	$(CC) -o $(EXE) $(FLAGS) $(MAIN)


clean:
	-rm -f $(EXE)

run:
	./$(EXE)