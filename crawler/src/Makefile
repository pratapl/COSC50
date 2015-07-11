
#MakeFile
#Pratap Luitel

CC=gcc
CFLAGS1=-Wall
CFLAGS=-Wall -pedantic -std=c11 -g -ggdb
SOURCES=list.c list.h web.c web.h hashtable.c hashtable.h common.h utils.h
CFILES=list.c web.c hashtable.c crawler.c
LDFFLAGS=-lcurl
.PRECIOUS: %.c
URL=http://old-www.cs.dartmouth.edu/~cs50/tse
URLInvalid=www.dartmouth.edu

crawler:$(SOURCES)
	$(CC) $(CFLAGS) -o crawler $(CFILES) $(LDFFLAGS)
clean:
	rm -f temp 
	rm -f *~ 
	rm -f *.*~
	rm -f crawler
	rm -f *.o
	rm -f *.swp
	rm -f *#
	rm -f .*swp
	rm -f core.*
	rm -f data/*

run:
	./crawler $(URL) data 1

runInvalid:
	./crawler $(URLInvalid) data 1
