#
# Makefile for util directory
# Author: Pratap Luitel
# CS50, Query
#
OUTP = libtseutil.a

CC = gcc
CFLAGS = -g -Wall -pedantic -std=c11

all:obj 
	ar -cvq $(OUTP) *.o
obj:
	$(CC) $(CFLAGS) -c *.c
.SILENT:

clean:
	rm -f core.*
	rm -rf *.o
	rm -f *.a
