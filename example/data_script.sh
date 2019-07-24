avgByProc() { 
    local foo start end n=$1 e=$1 values times
    shift;
    export n;
    { 
        read foo;
        read foo;
        read foo foo start foo
    } < /proc/timer_list;
    mapfile values < <(
        for((;n--;)){ "$@" &>/dev/null;}
        read -a endstat < /proc/self/stat
        {
            read foo
            read foo
            read foo foo end foo
        } </proc/timer_list
        printf -v times "%s/100/$e;" ${endstat[@]:13:4}
        bc -l <<<"$[end-start]/10^9/$e;$times"
    )
    printf -v fmt "%-7s: %%.5f\\n" real utime stime cutime cstime
    printf "$fmt" ${values[@]}
}

cd ../src
make
cd ../example
./trip_maker.py
echo How many trials? 
read varname

avgByProc varname ../src/new_main -g network-links.txt -c network-nodes.txt -N nfa_main.txt -t 12 -f test-trip-file.txt
#time for i in {1..$varname}; do ../src/new_main -g network-links.txt -c network-nodes.txt -N nfa_main.txt -t 12 -f test-trip-file.txt ; done

