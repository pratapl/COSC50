#Pratap Luitel
#CS50 - indexer
#indexer.makefile

CC = gcc

CFLAGS = -g -Wall -pedantic -std=c11

all: indexer

indexer: indexer.o hashtable.o file.o web.o 
	$(CC) $(CFLAGS) -o indexer indexer.o hashtable.o file.o web.o

indexer.o: indexer.c file.h web.h hashtable.h
	$(CC) $(CFLAGS) -o indexer.o -c indexer.c

hashtable.o: hashtable.c hashtable.h web.h
	$(CC) $(CFLAGS) -c hashtable.c -o hashtable.o

file.o: file.c file.h 
	$(CC) $(CFLAGS) -o file.o -c file.c

web.o: web.c web.h
	$(CC) $(CFLAGS) -o web.o -c web.c

.SILENT:

clean:
	rm -f core.*
	rm -rf *.o
	rm -f *~
	rm -f *#
	rm -f indexer
	

