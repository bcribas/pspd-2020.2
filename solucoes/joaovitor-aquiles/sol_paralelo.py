#!/usr/bin/env python3
import sys
import math
from concurrent.futures import ProcessPoolExecutor, ThreadPoolExecutor
import multiprocessing as mp
# import os
import time
import heapq

queue = mp.Manager().Queue()

pipe_producer, pipe_consumer = mp.Pipe()

def verificator(claus: list, var_qtd : dict):
    claus_false = []
    heap = []
    #print(f"var_atuais:  {vars_atual}")
    # print(f"clausulas {claus}")
    while not pipe_consumer.poll() or not queue.empty():
        # queue_lock.acquire()
        try:
            ind , vars_atual = queue.get(timeout=0.5)
        except:
            break
            #queue_lock.release()

        qtd_false = 0
        for i, x in enumerate(claus):
            #list compreension
            # print("clausula atual:", x)
            n=1
            bool_claus = []
            for y in x:
                print("y: ",y)
                print("vars_atual: ", vars_atual)
                # retirar um for otimizou bastante o algoritmo
                if abs(y) in vars_atual:
                    if y>0:
                        bool_claus.append(vars_atual[abs(y)])
                    else:
                        bool_claus.append(not vars_atual[abs(y)])
                elif y in vars_atual:
                    if y>0:
                        bool_claus.append(vars_atual[y])
                    else:
                        bool_claus.append(not vars_atual[y])
                print('bool_claus',n,': ',bool_claus)
                n=n+1
            # a clausula é falsa
            if not any(bool_claus):
                # adiciona o indice da clausula em uma lista
                claus_false.append(i)
                qtd_false += 1
                for y in x:
                    # soma mais um no dict para poder ordenar depois no lits
                    var_qtd[y] += 1
        # print_lock.acquire()
        if not claus_false:
            string = 'SAT\n'
        else:
            string = f'[{qtd_false} clausulas falsas] '
            string += str(claus_false)[1:-1].replace(',','') + '\n'
            string += '[lits] '
            lvar_qtd = list(filter(lambda x: var_qtd[x] != 0,var_qtd))

            lvar_qtd = sorted(lvar_qtd, key= lambda x: (var_qtd[x], abs(x)) , reverse=True)
            string += str(lvar_qtd)[1:-1].replace(',','') + '\n'
            
        # print_lock.release()
        # resetando os objetos
        bool_claus.clear()
        var_qtd = dict.fromkeys(var_qtd, 0)
        claus_false.clear()
        heapq.heappush(heap,(ind,string))
    return heap

def put_queue(data):
    # talvez tirar alguns mutex pode melhorar performance
    # queue_lock.acquire()
    queue.put(data)
    # queue_lock.release()

def producer(all_var: dict):
    dict_var = {}
    for ind,line in enumerate(sys.stdin):

        cmd, *val = line.split()
        #print('cmd', cmd)
        #print('val', val)

        if cmd == "full":
            # para usar o full mais de uma vez tem que limpar o dicionário
            dict_var.clear()
            for x in val: # enumerate(val):
                i = int(x)
                dict_var.update({i : all_var[i]})
            print(dict_var)
            
            # aqui coloca na fila ao invés de chamar a função
            # verificator(list_claus, dict_var, var_qtd)
            put_queue((ind,dict_var))
            #zera a contagm
            # var_qtd = dict.fromkeys(var_qtd, 0)

        elif cmd == 'flip':
            i = int(val[0])
            #print("valor i:", i)
            #dict_var[abs(i) - 1] = not dict_var[abs(i) - 1]

            # retira o que tinha no dicionário e depois reinsere com valor e indice trocado
            pos = dict_var.pop(i, math.inf)
            neg = dict_var.pop(-i, math.inf)
            dic_value = not min(pos, neg)
            dict_var.update({ i if dic_value else -i : dic_value } )
            
            # aqui coloca na fila ao invés de chamar a função
            # verificator(list_claus, dict_var, var_qtd)
            put_queue((ind,dict_var))
            #zera a contagem
            # var_qtd = dict.fromkeys(var_qtd, 0)

            # print(dict_var)
            # print('')
    # avisa aos processos filhos que o producer terminou
    pipe_producer.send(True)
    return 'tred_return'

def main():
    num_cores = mp.cpu_count() if len(sys.argv) <= 1 else int(sys.argv[1])
    Var, Claus = input().split()
    Var = int(Var)
    Claus = int(Claus)

    var_qtd = {}
    #dict compreension
    all_var = {x: x>0 for x in range(-Var,Var+1)}

    list_claus = []

    # inserindo as clausulas em uma lista
    for _ in range(Claus):
        inp = input().split()
        var_atuais = []
        # var_atuais = sorted(var_atuais, key=sort_func)
        for y in inp:
            if y == '0':
                break
            # inicializa a quantidade em zero para ordenar no lits
            var_qtd.update({int(y):0})

            # cria uma lista de como foi escrito no input
            var_atuais.append(int(y))
        list_claus.append(var_atuais)
    with ProcessPoolExecutor(max_workers=num_cores) as PExecutor:
        procs = [
            PExecutor.submit(verificator, list_claus, var_qtd)
            for _ in range(num_cores - 1)
        ]
        producer(all_var)
        res = []
        for x in procs:
            res = heapq.merge(res,x.result())

    for ind, string in res:
        print(string, end='')
    
    pipe_producer.close()
    pipe_consumer.close()

if __name__ == '__main__':
    main()