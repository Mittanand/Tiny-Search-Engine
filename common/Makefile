# Makefile for common library used by crawler
#
# CS50 Summer 2020

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb 
OBJS = pagedir.o
LIB = common.a

# executable depends on objects
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

# dependency on header
pagedir.o: pagedir.h

.PHONY: clean

clean:
	rm -f $(LIB) *~ *.o
