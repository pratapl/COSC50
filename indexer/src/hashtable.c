/* ========================================================================== */
/* File: hashtable.c
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Indexer
 *
 * Author: Pratap Luitel
 * Date: 5/14/2015
 *
 * HashTable data structure is used to maintain the inverted index which is used for the indexer.
 */
/* ========================================================================== */

// ---------------- Open Issues
#define _GNU_SOURCE 1
// ---------------- System includes e.g., <stdio.h>
#include <string.h>                          // strlen
#include <stdlib.h>
#include <stdio.h>
// ---------------- Local includes  e.g., "file.h"
#include "hashtable.h"                       // hashtable functionality
#include "common.h"
// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes

/*
 * Initializes a hashtable with null wordNode.
 */
HashTable* initHashTable(){
	HashTable* hTable = (HashTable*)calloc(1, sizeof(HashTable));
	for(int i = 0; i < MAX_HASH_SLOT; i++){
		hTable->table[i] = (HashTableNode*)calloc(1, sizeof(HashTableNode));
		hTable->table[i]->word = NULL;
		hTable->table[i]->next = NULL;
		hTable->table[i]->page = NULL;
	}
	return hTable;	
}

/*
 * Checks if a word is in a hashtable. 
 * 
 * Input
 * hTable: the hash table to look the word 
 * word: the word to look for
 *
 * Output
 * 1 on success, 0 else
 */
int InHashTable(HashTable* hTable, char* Word){
	unsigned long slot = JenkinsHash(Word, MAX_HASH_SLOT);
	
	//Check for the URL at the slot. If slot is empty, then URL is not in hashtable.
	if(hTable->table[slot]->word == NULL){
		return 0;
	}
	
	else{
		HashTableNode* currNode = hTable->table[slot];
		if(strcmp(currNode->word, Word) == 0){
			return 1;
		}
		while((currNode->next) != NULL){
			currNode = currNode->next;
			if(strcmp(currNode->word, Word) == 0){
				return 1;
			}
			
		}
		return 0;	
	}	
}

int AddToHashTable(HashTable* hTable, char* Word, int docId){

	unsigned long slot = JenkinsHash(Word, MAX_HASH_SLOT);//position of the word in the hashtable
	
    //if the word is not present in the hashtable, add it
	if(hTable->table[slot]->word == NULL){
/*
int main()
{
	HashTable* gt = initHashTable();

	char *pURL = "http://cs.dartmouth.edu";
	char *pURL2 = "Random string";
	printf("%d\n", InHashTable(gt, pURL));
	printf("%d\n", AddToHashTable(gt, pURL, 1));
	printf("%d\n", AddToHashTable(gt, pURL, 2));
	printf("%d\n", AddToHashTable(gt, pURL, 1));
	printf("%d\n", AddToHashTable(gt, pURL2, 1));
	printf("%d\n", AddToHashTable(gt, pURL, 2));		
	printf("done, sizeof GenHashTable=%lu\n", sizeof(HashTable));
	
	return 0;
}*/
		hTable->table[slot]->word = strdup(Word);
		DocumentNode* tempNode = (DocumentNode*)calloc(1, sizeof(DocumentNode));
		hTable->table[slot]->page = tempNode;	
		hTable->table[slot]->page->doc_id = docId;
		hTable->table[slot]->page->freq = 1;
		hTable->table[slot]->page->next = NULL;
		return 0;
	}
	
	else{
		HashTableNode* currNode = hTable->table[slot];
		while((currNode->next)!=NULL){
			if(strcmp(currNode->word, Word) == 0){
				DocumentNode* docNode = currNode->page;
				while((docNode->next)!=NULL){
					if(docNode->doc_id == docId){
						docNode->freq += 1;
						return 0;
					}
					docNode = docNode->next;
				}
				//check for the last node
				if(docNode->doc_id == docId){
					docNode->freq += 1;
					return 0;
				}
				//Add a document Node at the end
				DocumentNode* tempNode = (DocumentNode*)calloc(1, sizeof(DocumentNode));
				tempNode->next = NULL;
				tempNode->doc_id = docId;
				tempNode->freq = 1;
				docNode->next = tempNode;
				return 0;
			}
			currNode = currNode->next;
/*
int main()
{
	HashTable* gt = initHashTable();

	char *pURL = "http://cs.dartmouth.edu";
	char *pURL2 = "Random string";
	printf("%d\n", InHashTable(gt, pURL));
	printf("%d\n", AddToHashTable(gt, pURL, 1));
	printf("%d\n", AddToHashTable(gt, pURL, 2));
	printf("%d\n", AddToHashTable(gt, pURL, 1));
	printf("%d\n", AddToHashTable(gt, pURL2, 1));
	printf("%d\n", AddToHashTable(gt, pURL, 2));		
	printf("done, sizeof GenHashTable=%lu\n", sizeof(HashTable));
	
	return 0;
}*/
		}
		//Check for last word match
		if(strcmp(currNode->word, Word) == 0){
			DocumentNode* docNode = currNode->page;
				while((docNode->next)!=NULL){
					if(docNode->doc_id == docId){
						docNode->freq += 1;//add to the frequency if docid and node id match
						return 0;
					}
					docNode = docNode->next;
				}
				if(docNode->doc_id == docId){//last node
					docNode->freq += 1;
					return 0;
				}
				DocumentNode* tempNode = (DocumentNode*)calloc(1, sizeof(DocumentNode));
				tempNode->next = NULL;
				tempNode->doc_id = docId;
				tempNode->freq = 1;
				docNode->next = tempNode;
				return 0;
		}
		//match not found
        HashTableNode* tempWord = (HashTableNode*) calloc(1, sizeof(HashTableNode));
		tempWord->word = strdup(Word);
		tempWord->next = NULL;
		DocumentNode* docNode = (DocumentNode*) calloc(1, sizeof(DocumentNode));//docNode for wordNode
		tempWord->page = docNode;
		tempWord->page->doc_id = docId;
		tempWord->page->freq = 1;
		tempWord->page->next = NULL;
		currNode->next = tempWord;//add the word node at the end 
		return 0;
	}
	return 1;
}

unsigned long JenkinsHash(const char *str, unsigned long mod)
{
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

