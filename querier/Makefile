# Makefile for querier
#
# CS50 Summer 2020

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
VFLAGS = --leak-check=full --show-leak-kinds=all
PROG = querier
OBJS = querier.o
LIBS =  ../common/common.a ../libcs50/libcs50.a 

MAKE = make

.PHONY: all test valgrind clean

all: $(PROG)

# executable depends on object files
$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

test: $(PROG)
	bash testing.sh

valgrind: $(PROG)
	valgrind $(VFLAGS) ./querier ../crawler/dir ../indexer/test

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
