#indexer.makefile

UTILDIR=util/
SRCDIR=src/
DATADIR=data/
SRC = $(SRCDIR)hashtable.c $(SRCDIR)hashtable.h $(SRCDIR)indexer.c  
QSRC = $(SRCDIR)queryHelper.c $(SRCDIR)queryHelper.h $(SRCDIR)hashtable.c $(SRCDIR)hashtable.h $(UTILDIR)file.c $(UTILDIR)file.h 
CFILES = $(SRCDIR)indexer.c $(SRCDIR)hashtable.c
QCFILES = $(SRCDIR)query.c $(SRCDIR)queryHelper.c $(SRCDIR)hashtable.c $(UTILDIR)file.c

CC = gcc
CFLAGS = -Wall -pedantic -std=c11

UTILFLAG=-ltseutil
UTILLIB=$(UTILDIR)libtseutil.a
UTILC=$(UTILDIR)web.c $(UTILDIR)file.c
UTILH=$(UTILDIR)web.h $(UTILDIR)file.h $(UTILDIR)common.h

query: $(QSRC)$(UTILLIB)
	$(CC) $(CFLAGS) -o $@ $(QCFILES) 

$(UTILLIB): $(UTILC) $(UTILH)
	cd $(UTILDIR); make;

compileIndexer: indexer runIndexer

indexer: $(SRC) $(UTILDIR)web.h $(UTILDIR)file.h $(UTILDIR)common.h $(UTILLIB) 
	$(CC) $(CFLAGS) -o $@ $(CFILES) -L$(UTILDIR) $(UTILFLAG)

runIndexer:
	./indexer $(DATADIR)htmlData $(DATADIR)indexer.dat



.SILENT:

clean:
	rm -f core.*
	rm -rf *.o
	rm -f *~
	rm -f *#
	rm -f query
	rm -f indexer
	rm -f $(DATADIR)*.dat
	 
