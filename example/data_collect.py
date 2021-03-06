#!/usr/bin/env python3
#import os
#import glob
#import pandas as pd
import sys
import random
import time
import subprocess
import csv
from statistics import mean

#Creates trip request files
def write_test(num):
    f = open("test-trip-file.txt", "+w")
    for i in range(int(num)):
        node_num1 = random.randint(1,10)
        node_num2 = random.randint(1,10)
        while(node_num2 == node_num1):
            node_num2 = random.randint(1,10)
        f.write(str(i) + " " + str(node_num1) + " " + str(node_num2) + " " + str(10) + " " + str(0) + "\n")
    f.close
#Allows commandline usage
def subprocess_cmd(command): #Adapted from stackoverflow example
    process = subprocess.Popen(command,stdout=subprocess.PIPE, shell=True)
    proc_stdout = process.communicate()[0].strip()
    #print(proc_stdout)

#Actual test
def master_test(requests, number):
    write_test(requests)
    master_list = []
    avg_list = []
    total_list = []
    core_c = [1,2,4,8]
    print("Timing test " + str(number))
    print("Trials: 100")
    print("Trip Requests: " + str(requests))
    string = "test_data" + str(number) + ".csv"
    with open(string, 'w') as f1:
        writer = csv.writer(f1, dialect='excel')
        writer.writerow(["Test" + str(number)])
        writer.writerow(["1 Core", "2 Cores", "4 Cores", "8 Cores"])
        for core in core_c:
            result_list = []
            for j in range(100):
                comm = ("../src/new_main -g network-links.txt -c network-nodes.txt -N nfa_main.txt -t 12 -f test-trip-file.txt -s" + " " + str(core))
                start = time.time()
                subprocess_cmd(comm)
                test = time.time() - start
                result_list.append(test)
            
            total_list.append(sum(result_list))
            avg_list.append(mean(result_list))
            print("Average time of " + str(core) + " Core(s):" + str(mean(result_list)) + " secs.")
            master_list.append(result_list)
        
        writer.writerows(zip(*master_list))
        writer.writerow(["Averages"])
        writer.writerow(avg_list)
    print("Total time: " + str(sum(total_list)) + " secs")

#main
subprocess_cmd("cd ../src; make; cd ../example;")
master_test(1000,1)
master_test(10000,2)
master_test(100000, 3)

#Merge produced CSV files
#extension = 'csv'
#all_filenames = [i for i in glob.glob('*.{}'.format(extension))]
#combined_csv = pd.concat([pd.read_csv(f) for f in all_filenames])
#combined_csv.to_csv( "combined_csv.csv", index=False, encoding='utf-8-sig')

exit()