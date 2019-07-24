#some parts of this script were adapted from internet sources

cd ../src
make
cd ../example
./trip_maker.py
#avg_time 100 ../src/new_main -g network-links.txt -c network-nodes.txt -N nfa_main.txt -t 12 -f test-trip-file.txt
start=$(date +%s)
time for i in {1..100}; do ../src/new_main -g network-links.txt -c network-nodes.txt -N nfa_main.txt -t 12 -f test-trip-file.txt ; done
end=$(date +%s)
echo average time:
bc -l <<< "($end -$start) / 10" | sed 's/[0]*$//g'

