# Somativa 2

| Nome | Matricula | 
|------|-----------| 
|Matheus de Cristo | 15/0141220|
|Lucas Alexandre| 15/0136862|

## Introdução 

Somativa 2 da disciplina de PSPD, cujo o objetivo é o desenvolvimento de um algoritmo que seja capaz que verificar se ocorre a satisfatibilidade booleana(SAT) em determinados conjuntos de variaveis booleanas. Mais detalhes do projeto encontra-se em: [Link do projeto](https://www.brunoribas.com.br/pspd/2020-2/#org5a0c298) 


## Estrutura de dados
![imagem1](https://raw.githubusercontent.com/MatheusEstanislau/PSPD/master/img/estrutura_dados.png?token=AKTBIP5HPAX73NWLD72XQOTAUUJLE)

## Resultados 

Usando computador pessoal, com processador i5-9400f rodando a 4.00ghz com 6 nucleos e 6 threads foram executadas as seguintes entradas:

- flat175-34
- uf100-01
- flat75-90 
- bmc-ibm-3

Tempo de execução em relação as entradas fornecidas (sequencial) com TIMELIMIT = 5
![imagem1](https://raw.githubusercontent.com/MatheusEstanislau/PSPD/master/img/sequencial.png?token=AKTBIP5N67IODTKYNBMAU6TAUUJR4)

## O desempenho é modificado quando o arquivo de saída é redirecionado para /dev/null?
Sim. Ao desviar para /dev/null pode-se notar através do gráfico acima que o tempo de execução foi reduzido.

## E quando a impressão é comentada?
Sim. Quando a impressão é comentada o tempo de execução é reduzido significativamente, indicando um possível gargalo na impressão.

Tempo de execução em relação as entradas fornecidas (paralelo) com TIMELIMIT = 5,
Ambiente: Processador i5-9400f 4Ghz
![imagem2](https://raw.githubusercontent.com/MatheusEstanislau/PSPD/master/img/paralelo_6thread.png?token=AKTBIP6KGKCVCKVIHVL7QLLAUUJTI)

Ambiente: Chococino para 8 e 12 threads com TIMELIMIT = 5 

![imagem2](https://raw.githubusercontent.com/MatheusEstanislau/PSPD/master/img/paralelo_8thread.png?token=AKTBIP7JKKQKCE27344SZ3LAUUJUI)

## Como o desempenho é alterado rodando o programa com 2, 4, 6, 8 e 12 threads?
A parelelização não foi otimizada de acordo com o esperado causando aumento no tempo de execução de alguns casos de teste.

## Gargalo e complexidade 

O gargalo do algoritmo ocorre principalmente na impressão da saída, ordenamento dos lits e no ordenamento das clausulas e criação de threads.


### Qual é a complexidade do verificador?

A complexidade do verificador é O(n*m), pois para verificação foi utilizado um laço dentro do vetor de clausulas falsas e dentro dele foi utilizado outro laço percorrendo o vetor de clausulas nas posições em que se encontravam as clausulas falsas.  


