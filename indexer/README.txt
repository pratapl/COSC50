README

Author: Pratap Luitel
CS50, Indexer 
Readme file

Makefile should be used to build and run the indexer. 
Commands:
	make clean
	make
	make run

Compiling without makefile:
	gcc -Wall -pedantic -std=c11 -o indexer indexer.c hashtable.c web.c file.c

option1(Normal Mode): ./indexer [Directory Name] [File Name]
	This mode outputs a file[File Name] containing the frequency map of each word in all the files. 
	data0/data1 contains the files from crawler depth 0/1. They can be used for [Directory Name] 
    data1 is a folder that contains the files from the crawler depth1. 

option2(Test Mode): ./indexer [Directory Name] [File Name] [File Name] [Final File Name]

Assumptions made:
	Files in [Directory Name] must have numeric names
	Argument 3 and 4 should be same in test mode
	Argument 4 and 5 should not be same in test mode

Implementation details can be found in PratapLuitelCOSC50Indexer.pdf 




