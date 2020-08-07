# Makefile for indexer
#
# CS50 Summer 2020

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
VFLAGS = --leak-check=full --show-leak-kinds=all
PROG = indexer
PROGTEST = indextest
OBJS = indexer.o
OBJSTEST = indextest.o
LIBS =  ../common/common.a ../libcs50/libcs50.a 

MAKE = make

.PHONY: all test valgrind clean

all: $(PROG) $(PROGTEST)

# executable depends on object files
$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

$(PROGTEST): $(OBJSTEST)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

test: $(PROG) $(PROGTEST)
	bash testing.sh

valgrind: $(PROG)
	valgrind $(VFLAGS) ./indexer ../crawler/dir test
	valgrind $(VFLAGS) ./indextest test testout

clean:
	rm -f $(PROG) $(PROGTEST)
	rm -f *~ *.o *.dSYM
	rm -fr test
	rm -fr testout
	rm -fr testsort
	rm -fr testoutsort
