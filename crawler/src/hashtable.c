/* ========================================================================== */
/* File: hashtable.c
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Crawler
 *
 * Author:Pratap Luitel
 * Date:5/6/2015
 *
 * You should include in this file your functionality for the hashtable as
 * described in the assignment and lecture.
 */
/* ========================================================================== */

// ---------------- Open Issues
#define _GNU_SOURCE 1
// ---------------- System includes e.g., <stdio.h>
#include <string.h>                          // strlen
#include <stdlib.h>
#include <stdio.h>
// ---------------- Local includes  e.g., "file.h"
#include "common.h"                          // common functionality
#include "hashtable.h"                       // hashtable functionality
#include "utils.h"
// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes

/* initialize a hashtable with MAX_SLOT_HASHTABLE slots in the table. 
 * MAX_SLOT_HASHTABLE is defined in utils.h and equals 4000
 * */
HashTable* initHashTable(){
	HashTable* hTable = (HashTable*)malloc(sizeof(HashTable));
	MALLOC_CHECK(stderr, hTable);
	//Create slots
	for(int i = 0; i < MAX_HASH_SLOT; i++){
		hTable->table[i] = (HashTableNode*)malloc(sizeof(HashTableNode));
		MALLOC_CHECK(stderr, hTable->table[i]);
		hTable->table[i]->url = NULL;
		hTable->table[i]->next = NULL;
	}
	return hTable;	
}

/* looks up URL in HashTable. 
 * URL: url to look up 
 * hTable: hashtable to search url in
 *
 * returns: 1 if url is in hashtable, else 0
 */
int HashTableLookup(HashTable* hTable, char* URL){
	ASSERT_FAIL(stderr, URL);
	unsigned long slot = JenkinsHash(URL, MAX_HASH_SLOT);
	//if the slot is empty, URL is not present in the hashtable
	if(hTable->table[slot]->url == NULL){
		return 0;
	}
	
	else{
		HashTableNode* currNode = hTable->table[slot];
		if(strcmp(currNode->url, URL) == 0){
			return 1;
		}
		while((currNode->next) != NULL){
			currNode = currNode->next;
			if(strcmp(currNode->url, URL) == 0){ 
				return 1;
			}
			
		}
		return 0;	
	}	
}

/*Insert URL into a hashtable
 * hTable: hashtable to insert URL 
 * URL: url to insert into the table
 *
 * returns: 1 on success, 0 on failure
 *
 * Note: if the URL already exists in the table, 
 * returns 1. 
 */

int HashTableInsert(HashTable* hTable, char* URL){

    //if the URL exists, dont add and instead return  
	if(HashTableLookup(hTable, URL) == 1){
		return 1;
	}
	
    //add the url to the appropriate slot based on Jenkins hash
	unsigned long slot = JenkinsHash(URL, MAX_HASH_SLOT);
	//check if the slot is empty
	if(hTable->table[slot]->url == NULL){
		hTable->table[slot]->url = strdup(URL);
		return 0;
	}
	else{
		HashTableNode* currNode = hTable->table[slot];
		//create a new node
		HashTableNode* tempNode = (HashTableNode*)malloc(sizeof(HashTableNode));
		tempNode->url = strdup(URL);
		tempNode->next = NULL;
		while((currNode->next) != NULL){
			currNode = currNode->next;
			if(currNode->next == NULL){
				currNode->next = tempNode;
				return 0;
			}
		}
		
		hTable->table[slot]->next = tempNode;
		return 0;	
	}
}

unsigned long JenkinsHash(const char *str, unsigned long mod){
    size_t len = strlen(str);
    unsigned long hash, i;
    
    for(hash = i = 0; i < len; ++i)
    {
        hash += str[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash % mod;
}

// testing
/*
int main(){
	HashTable* testHashTable = initHashTable();
	HashTableInsert(testHashTable, "www.testURL.com");
    printf("%d\n",HashTableLookup(testHashTable,"www.testURL.com"));
	printf("%d\n",HashTableLookup(testHashTable, "www.newURL.com"));
	return 0;
}*/

