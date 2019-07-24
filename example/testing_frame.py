#!/usr/bin/env python3
import sys
import random
import time
import subprocess
from statistics import mean



def write_test(num):
    f = open("test-trip-file.txt", "+w")
    for i in range(int(num)):
        node_num1 = random.randint(1,10)
        node_num2 = random.randint(1,10)
        while(node_num2 == node_num1):
            node_num2 = random.randint(1,10)
        f.write(str(i) + " " + str(node_num1) + " " + str(node_num2) + " " + str(10) + " " + str(0) + "\n")
    f.close

def subprocess_cmd(command): #Adapted from stackoverflow example
    process = subprocess.Popen(command,stdout=subprocess.PIPE, shell=True)
    proc_stdout = process.communicate()[0].strip()
    #print(proc_stdout)

trip_num =input("How many trips? ")
write_test(trip_num)
core_c =input("How many cores? ")
subprocess_cmd("cd ../src; make; cd ../example;")
result_list = []
for i  in range(100):
    comm = ("../src/new_main -g network-links.txt -c network-nodes.txt -N nfa_main.txt -t 12 -f test-trip-file.txt -s" + " " + str(core_c))
    start = time.time()
    subprocess_cmd(comm)
    test = time.time() - start
    result_list.append(test)
    print(test)

print("Trip Requests: " + str(trip_num))
print("Thread/core count: " + str(core_c))
print("Average time(sec): " + str(mean(result_list)))
print("Total time: " + str(sum(result_list)))
