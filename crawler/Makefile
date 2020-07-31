# Makefile for crawler
#
# CS50 Summer 2020

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
VFLAGS = --leak-check=full --show-leak-kinds=all
PROG = crawler
OBJS = crawler.o
LIBS =  ../libcs50/libcs50.a ../common/common.a


MAKE = make

.PHONY: all test valgrind clean

all: $(PROG)

# executable depends on object files
$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

test: $(PROG)
	rm -fr dir
	mkdir dir
	bash testing.sh

valgrind:
	rm -fr dir
	mkdir dir
	valgrind $(VFLAGS) ./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html dir 1

clean:
	rm -f $(PROG)
	rm -f *~ *.o *.dSYM
	rm -fr dir