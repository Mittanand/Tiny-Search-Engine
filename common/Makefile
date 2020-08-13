# Makefile for common library used by crawler
#
# CS50 Summer 2020

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb 
OBJS = pagedir.o word.o index.o
LIB = common.a

# executable depends on objects
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

# dependency on header
pagedir.o: pagedir.h
word.o: word.h
index.o: index.h

.PHONY: clean

clean:
	rm -f $(LIB) *~ *.o
