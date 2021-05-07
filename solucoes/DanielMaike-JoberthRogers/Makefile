CC = g++
BLDDIR = .
CFLAGS = -std=c++11 -O2 -Wall -static
MAINFILE = sat.cpp
EXE = bin

all: 
	$(CC) ${MAINFILE} -o ${EXE} ${CFLAGS}

clean:
	-rm -f $(EXE)

run:
	${BLDDIR}/${EXE}
