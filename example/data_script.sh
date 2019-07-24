avg_time_alt() { 
    local -i n=$1
    local foo real sys user
    shift
    (($# > 0)) || return;
    { read foo real; read foo user; read foo sys ;} < <(
        { time -p for((;n--;)){ "$@" &>/dev/null ;} ;} 2>&1
    )
    printf "real: %.5f\nuser: %.5f\nsys : %.5f\n" $(
        bc -l <<<"$real/$n;$user/$n;$sys/$n;" )
}

cd ../src
make
cd ../example
./trip_maker.py

avg_time_alt 3 ../src/new_main -g network-links.txt -c network-nodes.txt -N nfa_main.txt -t 12 -f test-trip-file.txt
#time for i in {1..$varname}; do ../src/new_main -g network-links.txt -c network-nodes.txt -N nfa_main.txt -t 12 -f test-trip-file.txt ; done

