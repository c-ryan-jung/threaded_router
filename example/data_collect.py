#!/usr/bin/env python3
import sys
import random
import time
import subprocess
import csv
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

write_test(1000)
subprocess_cmd("cd ../src; make; cd ../example;")
master_list = []
avg_list = []
total_list = []
core_c = [1,2,4,8]
print("Timing test.")
print("Trials: 100")
print("Trip Requests: 1000")
with open('test_data.csv', 'w') as f1:
    writer = csv.writer(f1, delimiter='\t',lineterminator='\n',)
    writer.writerow(['']+["1 Core"] + ["2 Cores"] + ["4 Cores"] + ["8 Cores"])
    for core in core_c:
        result_list = []
        for j in range(100):
            comm = ("../src/new_main -g network-links.txt -c network-nodes.txt -N nfa_main.txt -t 12 -f test-trip-file.txt -s" + " " + str(core))
            start = time.time()
            subprocess_cmd(comm)
            test = time.time() - start
            result_list.append(test)
        avg = mean(result_list)
        print("Average time of " + str(core) + " Core(s):" + str(avg) + " secs.")
        total_list.append(sum(result_list))
        avg_list.append(avg)
        dummy_list = [(None) + result_list]
        master_list.append[dummy_list]
    writer.writerow(["Averages"] + avg_list)
    writer.writerows(zip(*master_list))
print("Total time: " + str(sum(total_list)) + " secs")
exit()