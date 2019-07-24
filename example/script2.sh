cd ../src
make clean
make
cd ../example
./trip-maker.py

time for i in {1..3}; do ../src/new_main -g network-links.txt -c network-nodes.txt -N nfa_main.txt -t 12 -f test-trip-file.txt ; done