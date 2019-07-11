#!/usr/bin/env python3
import sys
import random
import time
import subprocess
from statistics import mean

#trip_num = input("How many trips?")




def write_test():
    f = open("test-trip-file.txt", "+w")
    print("flag1")
    node_num1 = random.randint(1,10)
    print("flag2")
    node_num2 = random.randint(1,10)
    print("flag3")
    while(node_num2 == node_num1):
        node_num2 = random.randint(1,10)
    print("flag4")
    for i in range(100):
        f.write(str(i) + " " + str(node_num1) + " " + str(node_num2) + " " + str(10) + " " + str(0) + "\n")
    print("flag5")
    f.close
    print("flag6")

def subprocess_cmd(command): #Adapted from stackoverflow example
    process = subprocess.Popen(command,stdout=subprocess.PIPE, shell=True)
    proc_stdout = process.communicate()[0].strip()
    print(proc_stdout)

write_test()
subprocess_cmd("cd ../src; make; cd ../example;")
result_list = []
for i  in range(1):
    start = time.time()
    subprocess_cmd("../src/new_main -g network-links.txt -c network-nodes.txt -N nfa_main.txt -t 12 -f test-trip-file.txt")
    test = time.time() - start
    result_list.append(test)

print(mean(result_list))
