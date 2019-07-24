#!/bin/bash

cd ../src
make
cd ../example
../src/new_main -g network-links.txt -c network-nodes.txt -N nfa_main.txt -t 12 -f test-trip-file.txt -z 4
#cat *[_out].txt > full.txt
#rm *[_out].txt
