#!/bin/bash

cd ../src
make
cd ../Sample
../src/new_main -g UA-Richmond-router-link-file.txt -c UA-Richmond-router-node-file.txt -N nfa-main.txt -t 12 -f trip-requests-0000.txt -s 3
#cat *[_out].txt > full.txt
#rm *[_out].txt