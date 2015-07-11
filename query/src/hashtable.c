/* ========================================================================== */
/* File: hashtable.c
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Indexer
 *
 * Author: Pratap Luitel
 * Date: 5/22/2015
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
#include "../util/common.h"
// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes

//Creates the hashtable with its wordNodes which are currently null.
HashTable* initHashTable(){
	HashTable* hTable = (HashTable*)calloc(1, sizeof(HashTable));
	//MALLOC_CHECK(stderr, hTable);
	//Create slots
	for(int i = 0; i < MAX_HASH_SLOT; i++){
		hTable->table[i] = (HashTableNode*)calloc(1, sizeof(HashTableNode));
		//MALLOC_CHECK(stderr, hTable->table[i]);
		hTable->table[i]->word = NULL;
		hTable->table[i]->next = NULL;
		hTable->table[i]->page = NULL;
	}
	return hTable;	
}

//Checks if a specific word is inside the hashTable.
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

	//Find the slot of the word in the hashtable using JenkinsHash
	unsigned long slot = JenkinsHash(Word, MAX_HASH_SLOT);
	//unsigned long slot = 0;
	//If the slot is empty, simply add the word.
	if(hTable->table[slot]->word == NULL){
		
		hTable->table[slot]->word = strdup(Word);
		DocumentNode* tempNode = (DocumentNode*)calloc(1, sizeof(DocumentNode));
		hTable->table[slot]->page = tempNode;	
		hTable->table[slot]->page->doc_id = docId;
		hTable->table[slot]->page->freq = 1;
		hTable->table[slot]->page->next = NULL;
		//printf("First node at this place\n");	
		return 0;
	}
	
	else{
		HashTableNode* currNode = hTable->table[slot];
		//The current node 
		while((currNode->next)!=NULL){
			//If the word of the node and the given word are the same, then add a docNode or increase frequency.
			if(strcmp(currNode->word, Word) == 0){
				DocumentNode* docNode = currNode->page;
				while((docNode->next)!=NULL){
					//if the document node id and the given id are the same, increment the frequency.
					if(docNode->doc_id == docId){
						docNode->freq += 1;
						//printf("increased freuency\n");
						return 0;
					}
					docNode = docNode->next;
				}
				//check for the last node.
				if(docNode->doc_id == docId){
					docNode->freq += 1;
					//printf("increased frequency2\n");
					return 0;
				}
				//Add a document Node at the end since we didn't find a match.
				DocumentNode* tempNode = (DocumentNode*)calloc(1, sizeof(DocumentNode));
				tempNode->next = NULL;
				tempNode->doc_id = docId;
				tempNode->freq = 1;
				docNode->next = tempNode;
				//printf("added a document node\n");
				return 0;
			}
			currNode = currNode->next;
		}
		//Check if there is a match at the last Word node.
		if(strcmp(currNode->word, Word) == 0){
			DocumentNode* docNode = currNode->page;
				while((docNode->next)!=NULL){
					//if the document node id and the given id are the same, increment the frequency.
					if(docNode->doc_id == docId){
						docNode->freq += 1;
						//printf("increased frequency 3\n");
						return 0;
					}
					docNode = docNode->next;
				}
				//check for the last node.
				if(docNode->doc_id == docId){
					docNode->freq += 1;
					//printf("increased frequency 4\n");
					return 0;
				}
				//Add a document Node at the end since we didn't find a match.
				DocumentNode* tempNode = (DocumentNode*)calloc(1, sizeof(DocumentNode));
				tempNode->next = NULL;
				tempNode->doc_id = docId;
				tempNode->freq = 1;
				docNode->next = tempNode;
				//printf("added a document node\n");
				return 0;
		}
		//Couldn't find a match. Add a new word node at the end of the list.
		HashTableNode* tempWord = (HashTableNode*) calloc(1, sizeof(HashTableNode));
		tempWord->word = strdup(Word);
		tempWord->next = NULL;
		//Create a document node for the newly created wordNode.
		DocumentNode* docNode = (DocumentNode*) calloc(1, sizeof(DocumentNode));
		tempWord->page = docNode;
		tempWord->page->doc_id = docId;
		tempWord->page->freq = 1;
		tempWord->page->next = NULL;
		currNode->next = tempWord;
		//printf("Create a new Wordnode\n");
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
//Free the hash table memory.
void freeHashTable(HashTable* hash){

	for(int slot = 0; slot < MAX_HASH_SLOT; slot++){
		//If the word at the slot is null
		if(hash->table[slot]-> word == NULL){
			free(hash->table[slot]);
		}
		//If there is a word in the table
		else{
			HashTableNode* currNode = hash->table[slot];
			while(currNode->next != NULL){
				HashTableNode* tempCurrNode = currNode;
				
				currNode = currNode->next;
				//Check for the first document for the word node.
				DocumentNode* docNode = tempCurrNode->page;
				
				while(docNode->next != NULL){
					DocumentNode* tempNode = docNode;
					docNode = docNode->next;
					free(tempNode);
				}
				free(docNode);
				
				//Release stuff for the word node.
					
				free(tempCurrNode->word);
				free(tempCurrNode);
			}
			
			DocumentNode* docNode = currNode->page;
			while(docNode->next != NULL){
				DocumentNode* tempNode = docNode;
				docNode = docNode->next;
				free(tempNode);
			}
			free(docNode);
			free(currNode->word);
			//free(currNode->page);
			free(currNode);
		}
	}
	free(hash);
}


 //saves the inverted index to a file. Input: Pointer to the inverted index and to a file where we have to print
//Output: Return 1 if successful, 0 otherwise
// iterates through hash table and prints 
int SaveIndexToFile(HashTable *hash, char *file)
{  

  int wordNum = 0;
  // open the file for writing
  FILE *fpt = fopen(file, "w");
  
  // base for the name and the document frequency
  char *baseName = "%s %d ";
  char *docFreq = "%d %d ";

  // iterate through hash_table index
  for (int hashIndex = 0; hashIndex <= MAX_HASH_SLOT; hashIndex++)
  {
    //loop through every word and for every word every doc
    HashTableNode *actual = hash->table[hashIndex];
    

    while (actual != NULL)
    { 
      // this is mostly for the first worlds relevance
      if (actual->word != NULL)
      { 
        int numberOfDocuments = 0;

        char *buffer = calloc(1, 40000);
        buffer[0] = '\0';
        char *final = calloc(1, 40000);
        buffer[0] = '\0';

        // loop through every document for that word
        DocumentNode *currentDoc = actual->page;       
        while (currentDoc != NULL)
        {
        
          // we need a temporary string to put the doc number and frequency
          char *temp = calloc(1, 100);
          temp[0] = '\0';
        
          // concatenate the doc id and frequency to the buffer
          sprintf(temp, docFreq, currentDoc->doc_id, currentDoc->freq);

          strcat(buffer, temp);
          free(temp);
        
          // move onto next document
          currentDoc = currentDoc->next;
          // update number of documents
          numberOfDocuments+=1;
        }
        
        
        // put the word and number of files into final
        sprintf(final, baseName, actual->word, numberOfDocuments);
        // concatenate the buffer to final
        strcat(final, buffer);
        strcat(final, "\n");
        // write to file
        fprintf(fpt, final);
        // A
        // free the buffers
        free(buffer);
        free(final); 
        
        wordNum+=1;
      }
      // it is possible that that the word is null at the initial place
      actual = actual->next;
    }
 // close the file
 }
fclose(fpt);
return wordNum;
}    

/*Reconstruct the hastable by reading the index.dat file.
 *
 * Input
 *file: char*, file to be read
 *
 *Output
 *new hashtable from the given file
 */
HashTable* ReadFile(char * file){
	HashTable* hash = initHashTable();
	FILE *pointer = fopen(file, "r");
  if (!pointer)
  {
    fprintf(stderr, "File could not be opened in testing mode\n");
    exit(1);
  }
  char line[40000];
  while(fgets(line, sizeof(line), pointer) != NULL) //get lines
  {
    char *pch;
    pch = strtok(line, " ");
    while (pch != NULL)//loop through each token 
    {
      char *word = pch;
      pch = strtok (NULL, " ");//increment ptr
      int number_of_docs = atoi(pch);

      for(int i = 0; i < number_of_docs; i++)
      {
        
        pch = strtok (NULL, " ");
        int docid = atoi(pch);//doc id
        pch = strtok (NULL, " ");//freq
        int freq = atoi(pch);
        for (int y = 0; y < freq; y++)
        {
          AddToHashTable(hash, word, docid);        
        }        
      }
      pch = strtok (NULL, " ");//next word
      pch = strtok (NULL, " ");//next word
    }    

  }
  return hash;
fclose(pointer);		
}




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
