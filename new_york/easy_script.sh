#!/bin/bash

cd ../src
make
cd ../new_main
../src/new_main -g UA-New-York-router-link-file.txt -c UA-New-York-router-node-file.txt -N nfa_main.txt -t 12 -f test-trip-file.txt -s 4
#cat *[_out].txt > full.txt
#rm *[_out].txt
