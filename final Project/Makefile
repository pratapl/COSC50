# Maze Project makefile
CC=gcc
CFLAGS=-Wall -pedantic -std=c11 -g -ggdb
SRC_DIR=./src/
LIBFLAG = -pthread `pkg-config --cflags --libs gtk+-3.0` 

SRCC=$(SRC_DIR)avatar.c $(SRC_DIR)AMStartup.c 
SRCH=$(SRCC:.c=.h)

all: AMStartup

AMStartup: AMStartup.o avatar.o
	$(CC) $(CFLAGS) -o AMStartup AMStartup.o avatar.o $(LIBFLAG)

AMStartup.o: $(SRCC) $(SRCH)
	$(CC) $(CFLAGS) -c $(SRC_DIR)AMStartup.c $(SRC_DIR)avatar.c $(LIBFLAG)

avatar.o: $(SRCC) $(SRCH)
	$(CC) $(CFLAGS) -c $(SRC_DIR)avatar.c 

clean:
	rm -f *~
	rm -f *.o
	rm -f *#
	rm -f AMStartup
	rm -f core*
