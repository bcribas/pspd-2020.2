# Somativa - PSPD


| Aluno | Matricula |
| :--: | :--: |
| Eduardo Vieira Lima | 17/0102343 |
| Samuel de Souza Buters Pereira | 17/0114040 |

## Relatório
### Gráficos
![](https://i.imgur.com/PUnbBbr.png)
Tempo de execução em relação aos arquivos de teste

![](https://i.imgur.com/ZojXUGd.png)
Tempo de execução em relação aos arquivos de teste com diferente número de threads


![](https://i.imgur.com/v7H5g8l.png)
Tempo de execução em relação aos arquivos de teste com diferentes métodos de I/O

### Algoritmos
Desenvolvemos dois algoritmos com estratégias diferentes. Um algoritmo para execução sequencial e outro para a execução paralela. A estratégia de cada algoritmo foi escolhida por seu desempenho em cada cenário (sequencial e paralelo).

### Avaliando o algoritmo
#### Gargalo
O maior gargalo acontece na coordenação de threads para imprimir o resultado na ordem correta, apesar de não usarmos semáforos, a lógica para garantir a ordem correta compromete o desempenho.
As operações de I/O  causam grande impacto no desempenho do algoritmo.

### Complexidade
Nosso algoritmo processa todo cláusulas para cada comando de forma paralelizada, assim para para n comandos e m cláusulas a complexidade é O(n*m)