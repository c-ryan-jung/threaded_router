#!/usr/bin/env python3
import sys
import random

trip_num =input("How many trips? ")
def write_test():
    f = open("test-trip-file.txt", "+w")
    for i in range(int(trip_num)):
        node_num1 = random.randint(1,10)
        node_num2 = random.randint(1,10)
        while(node_num2 == node_num1):
            node_num2 = random.randint(1,10)
        f.write(str(i) + " " + str(node_num1) + " " + str(node_num2) + " " + str(10) + " " + str(0) + "\n")
    f.close

write_test()