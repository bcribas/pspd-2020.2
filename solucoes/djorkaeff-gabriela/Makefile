ifndef DISTRIBUIRALUNOS
	FORMULAS=$(wildcard formulas/*cnf)
else
	FORMULAS=formulas/simples.cnf\
			formulas/uf20-01.cnf\
			formulas/flat75-90.cnf
	TIMELIMIT?=5
endif

INPUT=$(patsubst formulas/%.cnf,input/%,${FORMULAS})
TESTRUN=$(patsubst formulas/%.cnf,benchmark/%,${FORMULAS})
VERIFICARUN=$(patsubst benchmark/%,verificar/%,${TESTRUN})
TIMELIMIT?=30
DEFAULTSEED?=3000
GSATGENERATOR=./simple-gsat-io-generator
BENCHMARKBINARY?=$(GSATGENERATOR)

all: $(INPUT)

sequencial:
	g++ -std=c++17 -O2 sequencial.cpp -o bin

parallel:
	g++ -std=c++17 -O2 -lpthread parallel.cpp -o bin

input/%: formulas/%.cnf
	./simple-gsat-io-generator generator $(DEFAULTSEED) $^ $@ $(patsubst input/%,output/%,$@) $(TIMELIMIT)

benchmark/%: input/%
	@echo "== $^"
	time -p $(BENCHMARKBINARY) < $^ > $@

benchmark: benchmarkdir all $(TESTRUN)

benchmarkdir:
	mkdir -p benchmark

verificar/%: output/%
	@echo "==== Comparando $^"
	@diff -q -s $^ $(patsubst verificar/%,benchmark/%,$@)
	@echo

verificar: benchmark $(VERIFICARUN)

clean:
	rm -f benchmark/*
dist-clean: clean
	rm -f input/* output/*

PHONY: all benchmark benchmarkdir clean dist-clean verificar
