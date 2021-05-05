#!/usr/bin/env python3
from sys import stdin
import math

def verificator(claus: list, vars_atual : dict, var_qtd: dict):
    claus_false = []
    qtd_false = 0
    #print(f"var_atuais:  {vars_atual}")
    # print(f"clausulas {claus}")
    for i, x in enumerate(claus):
        #list compreension
        # print("clausula atual:", x)

        bool_claus = []
        for y in x:
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


        # a clausula é falsa
        if not any(bool_claus):
            # adiciona o indice da clausula em uma lista
            claus_false.append(i)
            qtd_false += 1
            for y in x:
                # soma mais um no dict para poder ordenar depois no lits
                var_qtd[y] += 1
    if not claus_false:
        print("SAT")
    else:
        print(f"[{qtd_false} clausulas falsas] ",end="")
        print(*claus_false)
        print(f"[lits] ",end="")
        lvar_qtd = list(filter(lambda x: var_qtd[x] != 0,var_qtd))

        lvar_qtd = sorted(lvar_qtd, key= lambda x: (var_qtd[x], abs(x)) , reverse=True)
        print(*lvar_qtd)
    
    

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
#print(list_claus)
#list_var = [0] * Var
# decidi que um dict é melhor
dict_var = {}
for line in stdin:

    cmd, *val = line.split()
    #print('cmd', cmd)
    #print('val', val)

    if cmd == "full":
        # para usar o full mais de uma vez tem que limpar o dicionário
        dict_var.clear()
        for x in val: # enumerate(val):
            i = int(x)
            dict_var.update({i : all_var[i]})
        # print(dict_var)

        verificator(list_claus, dict_var, var_qtd)
        #zera a contagm
        var_qtd = dict.fromkeys(var_qtd, 0)

    elif cmd == 'flip':
        i = int(val[0])
        #print("valor i:", i)
        #dict_var[abs(i) - 1] = not dict_var[abs(i) - 1]

        # retira o que tinha no dicionário e depois reinsere com valor e indice trocado
        pos = dict_var.pop(i, math.inf)
        neg = dict_var.pop(-i, math.inf)
        dic_value = not min(pos, neg)
        dict_var.update({ i if dic_value else -i : dic_value } )
        verificator(list_claus, dict_var, var_qtd)

        #zera a contagem
        var_qtd = dict.fromkeys(var_qtd, 0)

        # print(dict_var)
        # print('')
