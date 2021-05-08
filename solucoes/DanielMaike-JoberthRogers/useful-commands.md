# Comandos Úteis do Projeto

## Controle do Número de Fulls e Flips

Todos os arquivos da pasta fórmulas são gerados com a mesma quantidade de **Cláusulas** (*C*) e **Variáveis** (*V*), então para variar somente o número de ```full``` e ```flip``` é necessário:

* Ir até a linha 4 do ```Makefile``` original e adicionar uma interrogação (```?```) antes do sinal de igual (```=```)

``` makefile
# Arquivo Makefile em pspd-2020.2
3  else
4 	 FORMULAS?=formulas/simples.cnf\
5 			 formulas/uf20-01.cnf\
```
* Executar o comando de ```make``` passando como parâmetros as variáveis ```TIMELIMIT``` e ```FORMULAS``` na seguinte estrutura:

``` bash
$ make  TIMELIMIT=[NUMERO]  FORMULAS=formulas/[NOME-ARQUIVO].cnf
```

* Ex.:


``` bash
$ make  TIMELIMIT=15  FORMULAS=formulas/pombos-10.cnf
ou
$ make  TIMELIMIT=30  FORMULAS=formulas/pombos-10.cnf
```

## Forçar Saída da Chococino

Basta executar o seguinte comando no terminal em que a conexão ```ssh``` estiver ativa, mesmo que você não visualize os dados sendo escritos no terminal. 

```bash
$ ~ .
```

Em caso de erros, convém apertar a tecla ```enter``` para limpar o buffer e tentar novamente.

## Rodando o Valgrind

É necessário instalar o Valgrind utilizando:

```bash
$ sudo apt-get install valgrind
```

Após isso, é preciso compilar o arquivo sem as flags (```-O2 -static```) e com a seguinte flag: 
```bash
 -ggdb3
```

Após isso, basta executar o arquivo com o comando ```valgrind``` da seguinte forma:

```bash
$ valgrind ./meuprograma
```

## Verificar Espaço Disponível na Chococino

Para isso, basta entrar na máquina virtual desejada e executar o comando:

```bash
$ quota -s
```
## Contagem de Fulls e Flips

É necessário executar o comando ```grep``` com as flags de contagem de palavras e de linhas, da seguinte forma:

```bash
$ grep [COMANDO] [NOME-DO-ARQUIVO] | wc -l
```

* Ex.: Contagem de Fulls

```bash
$ grep full input/pombos-10 | wc -l
```

* Ex.: Contagem de Flips

```bash
$ grep flip input/pombos-10 | wc -l
```