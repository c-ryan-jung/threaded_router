#
# Contact:
#   Henning S. Mortveit
#
# Date:
#   16 Apr 2019
#
# Synopsis:
#   Makefile for re-router.
#

CC=g++
FLAGS=-g -Wall -std=c++11
OPT=-O2

INCDIR=-I$(HOME)/include
INCDIR+=-I/usr/local/include
LIBDIR=-L$(HOME)/lib
LIBDIR+=-L/usr/local/lib

new_main: new_main.o
	$(CC) $(OPT) new_main.o  $(LIBDIR) -llog4cplus -lpthread -o new_main

new_main.o: new_main.cpp basics.hpp dijkstra.hpp events.hpp\
           graph.hpp measurements.hpp timer.hpp visualization.hpp\
		   tools.hpp
	$(CC) -c $(FLAGS) $(OPT) $(INCDIR) new_main.cpp -o new_main.o

clean:
	rm -f *~ *.o new_main test *.dot