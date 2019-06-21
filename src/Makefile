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
FLAGS=-g -Wall
OPT=-O2

INCDIR=-I$(HOME)/include
INCDIR+=-I/usr/local/include
LIBDIR=-L$(HOME)/lib
LIBDIR+=-L/usr/local/lib

re-router: re-router.o
	$(CC) $(OPT) re-router.o  $(LIBDIR) -llog4cplus -lpthread -o re-router

re-router.o: re-router.cpp basics.hpp dijkstra.hpp events.hpp\
           graph.hpp measurements.hpp timer.hpp visualization.hpp
	$(CC) -c $(FLAGS) $(OPT) $(INCDIR) re-router.cpp -o re-router.o

clean:
	rm -f *~ *.o re-router *.dot
