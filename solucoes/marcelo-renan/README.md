# Relatório Projeto PSPD

|Nome|Matrícula|
|----|----|
|Marcelo Araújo|16/0035481|
|Renan Cristyan|17/0044386|

## Boolean Constraint Propagator

### Problema
O enunciado do projeto pode ser acessado no link: https://www.brunoribas.com.br/pspd/2020-2/.
O repositório que foi realizado o projeto pode ser acessado no link: https://github.com/RCristyan/projeto-1-pspd-2020.2
    Neste relatório iremos fazer uma análise dos resultados obtidos através de experimentos com o algoritmo implementado para resolver o problema proposto.

### Metodologia
Para realizar os relatórios, nós medimos o tempo de execução de três programas: o código fornecido pelo professor, a solução implementada de forma sequencial e a solução implementada de forma paralela (multithread). Selecionamos um conjunto de arquivos de entrada, e rodamos cada programa 5 vezes, e depois tiramos as médias dos tempos.
Para compilar o programa sequencial, iremos usar o comando:
```g++ -O2 -static variable_checker3.cpp -o sequencial```

e para compilar o programa paralelo:
```g++ -O2 variable_checker_threads.cpp -pthread -o paralelo```

Para obter os tempos de execução dos programas, iremos utilizar o comando:
```time nomeDoPrograma < arquivoDeEntrada```

e iremos registrar os valores exibidos (real, user, sys).

Daqui em diante, iremos nos referenciar aos programas como PROF, SEQ e MULTI.
Os testes foram realizados em uma maquina Lenovo Ideapad S145, com 8GB de memória RAM e processador de 4 núcleos/8 threads Intel Core i5-8265U CPU @ 1.60GHz. A partir daqui, iremos nos referir a máquina apenas por S145.

### Arquivos utilizados
#### simples
2 variáveis, 3 cláusulas
Resultados no S145 (tempos e média)

PROF
|execução / tempo|real|user|sys|
|--|--|--|--|
|1|0m0,002s|0m0,002s|0m0,000s|
|2|0m0,002s|0m0,002s|0m0,000s|
|3|0m0,002s|0m0,002s|0m0,000s|
|4|0m0,002s|0m0,002s|0m0,000s|
|5|0m0,002s|0m0,002s|0m0,000s|
|média|0m0,002s|0m0,002s|0m0,000s|

SEQ
|execução / tempo|real|user|sys|
|--|--|--|--|
|1|0m0,002s|0m0,002s|0m0,001s|
|2|0m0,002s|0m0,002s|0m0,001s|
|3|0m0,002s|0m0,000s|0m0,002s|
|4|0m0,002s|0m0,000s|0m0,002s|
|5|0m0,001s|0m0,000s|0m0,000s|
|média|0m0,0018s|0m0,0008s|0m0,0012s|

MULTI (2 threads)
|execução / tempo|real|user|sys|
|--|--|--|--|
|1|0m0,002s|0m0,002s|0m0,000s|
|2|0m0,002s|0m0,000s|0m0,001s|
|3|0m0,004s|0m0,002s|0m0,000s|
|4|0m0,002s|0m0,000s|0m0,002s|
|5|0m0,002s|0m0,000s|0m0,002s|
|média|0m0,0024s|0m0,0008s|0m0,001s|

MULTI (4 threads)
|execução / tempo|real|user|sys|
|--|--|--|--|
|1|0m0,002s|0m0,002s|0m0,000s|
|2|0m0,002s|0m0,002s|0m0,000s|
|3|0m0,002s|0m0,002s|0m0,000s|
|4|0m0,001s|0m0,002s|0m0,000s|
|5|0m0,002s|0m0,000s|0m0,002s|
|média|0m0,0018s|0m0,0016s|0m0,0004s|

MULTI (6 threads)
|execução / tempo|real|user|sys|
|--|--|--|--|
|1|0m0,002s|0m0,000s|0m0,002s|
|2|0m0,002s|0m0,002s|0m0,000s|
|3|0m0,002s|0m0,002s|0m0,000s|
|4|0m0,002s|0m0,002s|0m0,000s|
|5|0m0,002s|0m0,002s|0m0,000s|
|média|0m0,002s|0m0,0016s|0m0,0004s|

MULTI (8 threads)
|execução / tempo|real|user|sys|
|--|--|--|--|
|1|0m0,002s|0m0,002s|0m0,000s|
|2|0m0,002s|0m0,002s|0m0,000s|
|3|0m0,001s|0m0,002s|0m0,000s|
|4|0m0,003s|0m0,000s|0m0,002s|
|5|0m0,002s|0m0,002s|0m0,000s|
|média|0m0,002s|0m0,0016s|0m0,0004s|

MULTI (10 threads)
|execução / tempo|real|user|sys|
|--|--|--|--|
|1|0m0,005s|0m0,002s|0m0,004s|
|2|0m0,002s|0m0,002s|0m0,000s|
|3|0m0,002s|0m0,002s|0m0,000s|
|4|0m0,002s|0m0,000s|0m0,002s|
|5|0m0,002s|0m0,000s|0m0,002s|
|média|0m0,0026s|0m0,0012s|0m0,0016s|

MULTI (12 threads)
|execução / tempo|real|user|sys|
|--|--|--|--|
|1|0m0,002s|0m0,002s|0m0,000s|
|2|0m0,002s|0m0,002s|0m0,000s|
|3|0m0,002s|0m0,002s|0m0,000s|
|4|0m0,002s|0m0,002s|0m0,000s|
|5|0m0,002s|0m0,002s|0m0,000s|
|média|0m0,002s|0m0,002s|0m0,000s|

#### uf20-01
20 variáveis, 91 cláusulas
Resultados no S145 (tempos e média)

PROF
|execução / tempo|real|user|sys|
|--|--|--|--|
|1|0m0,002s|0m0,000s|0m0,002s|
|2|0m0,002s|0m0,001s|0m0,002s|
|3|0m0,002s|0m0,003s|0m0,000s|
|4|0m0,003s|0m0,003s|0m0,000s|
|5|0m0,002s|0m0,003s|0m0,000s|
|média|0m0,0022s|0m0,002s|0m0,0004s|

SEQ
|execução / tempo|real|user|sys|
|--|--|--|--|
|1|0m0,003s|0m0,001s|0m0,002s|
|2|0m0,003s|0m0,003s|0m0,000s|
|3|0m0,003s|0m0,001s|0m0,003s|
|4|0m0,003s|0m0,001s|0m0,003s|
|5|0m0,003s|0m0,003s|0m0,000s|
|média|0m0,003s|0m0,0018s|0m0,0016s|

MULTI (2 threads)
|execução / tempo|real|user|sys|
|--|--|--|--|
|1|0m0,023s|0m0,004s|0m0,023s|
|2|0m0,024s|0m0,014s|0m0,014s|
|3|0m0,022s|0m0,011s|0m0,015s|
|4|0m0,022s|0m0,007s|0m0,020s|
|5|0m0,022s|0m0,004s|0m0,023s|
|média|0m0,0226s|0m0,008s|0m0,019s|

MULTI (4 threads)
|execução / tempo|real|user|sys|
|--|--|--|--|
|1|0m0,020s|0m0,007s|0m0,023s|
|2|0m0,022s|0m0,022s|0m0,009s|
|3|0m0,022s|0m0,000s|0m0,031s|
|4|0m0,022s|0m0,013s|0m0,017s|
|5|0m0,024s|0m0,027s|0m0,006s|
|média|0m0,022s|0m0,0138s|0m0,0172s|

MULTI (6 threads)
|execução / tempo|real|user|sys|
|--|--|--|--|
|1|0m0,022s|0m0,017s|0m0,017s|
|2|0m0,022s|0m0,026s|0m0,010s|
|3|0m0,024s|0m0,012s|0m0,023s|
|4|0m0,024s|0m0,028s|0m0,010s|
|5|0m0,024s|0m0,011s|0m0,026s|
|média|0m0,0232s|0m0,0188s|0m0,0172s|

MULTI (8 threads)
|execução / tempo|real|user|sys|
|--|--|--|--|
|1|0m0,024s|0m0,018s|0m0,019s|
|2|0m0,024s|0m0,019s|0m0,019s|
|3|0m0,025s|0m0,004s|0m0,032s|
|4|0m0,024s|0m0,029s|0m0,008s|
|5|0m0,024s|0m0,033s|0m0,004s|
|média|0m0,0242s|0m0,0206s|0m0,0164s|

MULTI (10 threads)
|execução / tempo|real|user|sys|
|--|--|--|--|
|1|0m0,024s|0m0,017s|0m0,021s|
|2|0m0,025s|0m0,011s|0m0,027s|
|3|0m0,024s|0m0,009s|0m0,030s|
|4|0m0,026s|0m0,026s|0m0,013s|
|5|0m0,023s|0m0,032s|0m0,006s|
|média|0m0,0244s|0m0,019s|0m0,0194s|

MULTI (12 threads)
|execução / tempo|real|user|sys|
|--|--|--|--|
|1|0m0,024s|0m0,025s|0m0,013s|
|2|0m0,025s|0m0,036s|0m0,004s|
|3|0m0,024s|0m0,024s|0m0,016s|
|4|0m0,025s|0m0,020s|0m0,021s|
|5|0m0,025s|0m0,016s|0m0,021s|
|média|0m0,0246s|0m0,0242s|0m0,015s|

#### flat75-90
225 variáveis, 840 cláusulas

Resultados no S145 (tempos e média)

PROF
|execução / tempo|real|user|sys|
|--|--|--|--|
|1|0m1,907s|0m0,931s|0m0,096s|
|2|0m1,841s|0m0,971s|0m0,109s|
|3|0m1,762s|0m0,823s|0m0,161s|
|4|0m1,789s|0m0,894s|0m0,117s|
|5|0m1,832s|0m0,947s|0m0,097s|
|média|0m1,8262s|0m0,9132s|0m0.116s|

SEQ
|execução / tempo|real|user|sys|
|--|--|--|--|
|1|0m11,204s|0m9,344s|0m0,188s|
|2|0m11,186s|0m9,350s|0m0,217s|
|3|0m11,232s|0m9,422s|0m0,213s|
|4|0m11,091s|0m9,281s|0m0,209s|
|5|0m11,432s|0m9,558s|0m0,233s|
|média|0m11,432s|0m9,558s|0m0,233s|

MULTI
**Nenhuma versão utilizando threads respondeu com este arquivo de entrada.**

### Análise
Quanto tempo a sua implementação demora para os casos fornecidos?

    Para o arquivo simples, não houve grande diferença entre o sequencial e o paralelo, 
    girando em torno de 0,002s.
    Analisando pelos testes com o arquivo uf20-01, o sequencial é cerca de 0,02s mais rápido que o paralelo, ou seja, o paralelo 
    está cerca de 7,5 vezes mais lento, uma possível explicação disso seria a criação constante de threads e/ou uso de mutex.

Como é feita a otimização para paralelizar o algoritmo?

    Para otimizar o algoritmo, a ideia que tivemos foi atribuir uma 
    análise de cláusula por thread. Por exemplo, em um exemplo com
    2 threads, a primeira avalia as cláusulas de indicies pares e a
    segunda avalia as cláusulas de indices impares.

- Como o desempenho é alterado rodando o programa com 2, 4, 6, 8 e 12 threads?

      Como foi possível observar através das tabelas, a quantidade de threads não teve impacto significativo no desempenho.
      Isso é um indício de que a solução não está bem implementada, 
      apesar de que seria necessário testes com arquivos maiores 
      para chegar a uma conclusão.

- O desempenho é modificado quando o arquivo de saída é redirecionado para /dev/null?
  - E quando a impressão é comentada?
  - Qual o impacto da leitura do arquivo de entrada no tempo global?

A partir de qual tamanho de fórmula a paralelização faz diferença?

      Não temos certeza pois arquivos grandes não responderam de maneira
      apropriada.

Avalie o gargalo do seu algoritmo
- O maior problema acontece nas estruturas de controle (ex:semáforos)?
      
      Não. O maior problema acontece porque o código recalcula 
      muitos passos. Por exemplo, não há uma abordagem eficiente
      para tratar os flips.
      A quantidade de loops e chamadas de função é relativamente alta,
      o que impacta negativamente no desempenho.

- O tempo de leitura da entrada é perceptível?
- Isole o tempo de execução da função que avalia a verificação (descontando o tempo de leitura da fórmula e de cada valoração full e flips)

Qual é a complexidade do seu verificador?
- O seu algoritmo aproveita resultados parciais para avaliar flips mais rapidamente?

      Como dito anteriormente, não. Os resultados parciais não são
      reaproveitados para acelerar o processamento do flip.
