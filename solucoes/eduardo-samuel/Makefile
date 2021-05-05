CC=g++

CC_FLAGS = -O2 -static

all: sequential parallel

sequential:
	$(CC) -o sequential.bin sequentialSolver.cpp ${CC_FLAGS}

parallel:
	$(CC) -o parallel.bin parallelSolver.cpp ${CC_FLAGS} -pthread

clean:
	rm *.bin