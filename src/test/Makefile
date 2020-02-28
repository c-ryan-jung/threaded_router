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
FLAGS=-g -Wall -std=c++1z
OPT=-O2

INCDIR=-I$(HOME)/include
INCDIR+=-I/usr/local/include
LIBDIR=-L$(HOME)/lib
LIBDIR+=-L/usr/local/lib

OBJS=RRRFtest.o ../ReadRouteRequestFile.o

.cpp.o:
	$(CC) -c $(FLAGS) $(OPT) $(INCDIR) -o $@ $<

RRRF: $(OBJS)
	$(CC) -o $@ $(OBJS)

clean:
	rm -f *~ *.o new_main test *.dot
