/* ========================================================================== */
/* File: indexer.c - Tiny Search Engine Indexer
 *
 * Author: Pratap Luitel
 * Date: 5/22/2015
 *
 * Input: 
 *
 * Command line options:
 *
 * Output:
 *
 * Error Conditions:
 *
 * Special Considerations:
 *
 */
/* ========================================================================== */

// ---------------- Open Issues
#define _GNU_SOURCE 1
// ---------------- System includes e.g., <stdio.h>
#include <stdio.h>                           // printf
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// ---------------- Local includes  e.g., "file.h"
#include "../util/common.h"                          // common functionality
#include "hashtable.h"                       // hashtable functionality
#include "../util/file.h"
#include "../util/web.h"
#include "../util/indexer.h"
// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes

/* ========================================================================== */
/*
char* LoadDocument(char* filePath);
int GetDocumentId(char* fileName);
char* createPath(char* dirName, char* fileNum);
int SaveIndexToFile(HashTable* , char * );
void freeHashTable(HashTable* hash);
HashTable* ReadFile(char* file);
*/
int main(int argc, char* argv[]){
	
	// Program parameter processing
	if(argc != 3 && argc != 5){
		printf("Error: Incorrect usage\n");
		printf("Indexing Usage: ./indexer  [TARGET_DIRECTORY] [RESULTS FILE NAME]\n");
		printf("Indexing & Testing Usage:  ./indexer  [TARGET_DIRECTORY] [RESULTS FILENAME] [RESULTS FILENAME] [REWRITEN FILENAME]\n");
		return 1;
	}
	
	//Get the supplied directory name.
    	int dirSize = strlen(argv[1]);
    	char dir[dirSize + 1];
    	dir[0] = '\0';
    	strcat(dir, argv[1]);

	//Get the fileName.
	int fileSize = strlen(argv[2]);
	char fileGiven[fileSize + 1];
	fileGiven[0] = '\0';
	strcat(fileGiven, argv[2]);
	
	//Check if the path provided is a valid directory.
	if(IsDir(dir) == 0){
		printf("Incorrect path for the target directory\n");
		return 1;
	}
	
	/*
	*Not sure if we have to check for the RESULTS FILE NAME since i think we will have to initialize that in the code.
	* Check for it.
	*/
	
	//Initialize data structures
	//allocate Inverted_index, zero it, and set links to NULL.
	HashTable* hash = initHashTable(); //Inverted_index
	
	printf( "Building the index\n" );
	
	
	int docId;
	int pos;
	char *word;
	char *doc;
	char **fileNames;
	int numFiles = 0;

	numFiles = GetFilenamesInDir(dir, &fileNames);
	
	if(numFiles < 0) {
		// failed to get any filenames (various reasons)
		printf("Couldn't get any files\n");
		exit(1);
	} 
	else {
		for(int i = 0; i < numFiles; i++) {
			char* pathVar = createPath(dir, fileNames[i]);
			//printf("%s\n", fileNames[i]);
			//printf("%s\n", pathVar);
			doc = LoadDocument(pathVar);
			//printf("%s", doc);
			free(pathVar);
			docId = GetDocumentId(fileNames[i]);
			
			if(docId != 0){
				pos = 0;
				while ((pos = GetNextWord(doc, pos, &word)) > 0){
					NormalizeWord(word);
					//printf("%s\n", word);
					//printf("%d\n", docId);
					AddToHashTable(hash, word, docId);
					free(word);
				}
				//printf("Almost ending 4\n");	
				free(fileNames[i]);
			}
			//printf("Almost ending 3\n");
			free(doc);
		}
		free(fileNames);
	}

	SaveIndexToFile(hash, fileGiven);	
	printf( "done!\n");
	freeHashTable(hash);
	
	
	if(argc == 5){
		//Get the third argument and compare it to the second
		int argSize = strlen(argv[3]);
		char argThird[argSize + 1];
		argThird[0] = '\0';
		strcat(argThird, argv[3]);

		if(strcmp(fileGiven, argThird)!=0){
			printf("In testing mode, arguments 3 and 4 must be equal");
			exit(1);
		}
		
		//Get the fourth argument
		int argSize1 = strlen(argv[4]);
		char finalArg[argSize1 + 1];
		finalArg[0] = '\0';
		strcat(finalArg, argv[4]);

		if(strcmp(argThird, finalArg) == 0){
			printf("In testing mode, arguments 4 and 5 must be different");
			exit(1);
		}
		
		printf("Entering test mode\n");
		HashTable* hashTemp = ReadFile(argThird);
		SaveIndexToFile(hashTemp, finalArg);
		freeHashTable(hashTemp);
	}	

	return 0;
}

//loads the html document from a file. Input- path of the file to be loaded
//Output- A string containing the loaded document.
char* LoadDocument(char* filePath){
	FILE* ptrFile1;
	
	ptrFile1 = fopen(filePath, "r");
	
	if(ptrFile1 == NULL){
		printf("Can't open the file\n");
		exit(1);
	}
	fseek(ptrFile1, 0, SEEK_END);
	long fsize = ftell(ptrFile1);
	fseek(ptrFile1, 0, SEEK_SET);

	char *doc = calloc(1, fsize + 1);
	int lineCount = 1;
	char line[fsize];
	while(fgets (line, sizeof(line), ptrFile1) != NULL){
		if(lineCount < 3){
			lineCount += 1;
		}
		else{
			strcat(doc, line);
			lineCount += 1;
		}
	}
	fclose(ptrFile1);
	
	doc[fsize] = 0;
	
	return doc;
}

//Creates path for a specific fileName.
char* createPath(char* dirName, char* fileNum){
	char * baseName = "%s/%s";
	//Dynamically getting the length of filePath. 
	//int length = snprintf(NULL, 0, baseName, dirName, fileNum);
	char * str = calloc(1, strlen(dirName) + strlen(fileNum)+1);
	sprintf(str, baseName, dirName, fileNum);
	
	return str;	
}

//generates a document identifier from the name of the file. Input: fileName
//Output: Document ID
int GetDocumentId(char* fileName){
	return atoi(fileName);//return 0 if conversion from string to int unsuccessful.
}
/*
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
*/

/*
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
*/
/*
//Reconstruct the hastable by reading the index.dat file.
HashTable* ReadFile(char * file){
	HashTable* hash = initHashTable();
	FILE *pointer = fopen(file, "r");
 if (!pointer)
  {
    fprintf(stderr, "testing file could not be opened\n");
    exit(1);
  }
  char line[40000];
  // get the lines
  while(fgets(line, sizeof(line), pointer) != NULL)
  {
    char *pch;
    pch = strtok(line, " ");
    // now sart looping through that tokanized line
    while (pch != NULL)
    {
      char *word = pch;
      // move our pointer by one
      pch = strtok (NULL, " ");
      int numberOfDocs = atoi(pch);

      for(int i = 0; i < numberOfDocs; i++)
      {
        
        pch = strtok (NULL, " ");
        // get the doc id
        int docid = atoi(pch);
        // get the frequency
        pch = strtok (NULL, " ");
        int freq = atoi(pch);
        // have a for loop and add in the nodes
        for (int y = 0; y < freq; y++)
        {
          AddToHashTable(hash, word, docid);        
        }        
        //printf("word: %s number of docs: %d doc number: %d freq: %d\n", word, number_of_docs, docid, freq);
      }
      //move onto next word
      pch = strtok (NULL, " ");
      pch = strtok (NULL, " ");
    }    

  } 
// close the file after we are done
fclose(pointer);
return hash;		
}
*/
