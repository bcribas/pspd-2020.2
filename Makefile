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
TIMELIMIT?=30
DEFAULTSEED?=3000
GSATGENERATOR=./simple-gsat-io-generator
BENCHMARKBINARY?=$(GSATGENERATOR)

all: $(INPUT)
	@echo All DONE

input/%: formulas/%.cnf
	./simple-gsat-io-generator generator $(DEFAULTSEED) $^ $@ $(patsubst input/%,output/%,$@) $(TIMELIMIT)

benchmark/%: input/%
	@echo "== $^"
	time -p $(BENCHMARKBINARY) < $^ > $@

benchmark: benchmarkdir all $(TESTRUN)

benchmarkdir:
	mkdir -p benchmark
clean:
	rm -f benchmark/*
dist-clean: clean
	rm -f input/* output/*

PHONY: all benchmark benchmarkdir clean dist-clean
