CC = gcc
CC_FLAGS = -O2 -static
BIN = ./bin

sequencial:
	$(CC) -o ${BIN}/seq.bin versao_sequencial.c ${CC_FLAGS} -lpthread

paralelo:
	$(CC) -o ${BIN}/paralelo.bin versao_paralelizada.c ${CC_FLAGS} -lpthread

all: sequencial paralelo

clean:
	rm ./bin/*.bin