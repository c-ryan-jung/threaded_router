#!/usr/bin/env python
import sys
import random
import time
import subprocess
from statistics import mean

#trip_num = input("How many trips?")




def write_test():
    f = open("test-trip-file.txt", "w")
    node_num1 = random.randint(1,10)
    node_num2 = random.randint(1,10)
    while(node_num2 == node_num1):
        node_num2 = random.randint(1,10)

    for i in range(10):
        f.write(str(100 + i) + " " + str(node_num1) + " " + str(node_num2) + " " + str(1000) + " " + str(0) + "\n")
    f.close

def subprocess_cmd(command):
    process = subprocess.Popen(command,stdout=subprocess.PIPE, shell=True)
    proc_stdout = process.communicate()[0].strip()
    print(proc_stdout)

write_test()
subprocess_cmd("cd ../src; make; cd ../example;")
result_list = []
for i  in range(3):
    start = time.time()
    subprocess_cmd("../src/new_main -g network-links.txt -c network-nodes.txt -N nfa_main.txt -t 12 -f test-trip-file.txt")
    test = start - time.time()
    result_list.append(test)

print(mean(result_list))
