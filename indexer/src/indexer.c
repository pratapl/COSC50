/* ========================================================================== */
/* File: indexer.c - Tiny Search Engine Indexer
 *
 * Author: Pratap Luitel
 * Date: 05/19/2015
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
#include "common.h"                          // common functionality
#include "hashtable.h"                       // hashtable functionality
#include "file.h"
#include "web.h"
// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes

/* ========================================================================== */

char* loadDocument(char* filePath);//loads the document 
int GetDocumentId(char* fileName);//document id
char* createPath(char* dirName, char* fileNum);//creates the path
int SaveIndexToFile(HashTable* , char * );//saves file index
void freeHashTable(HashTable* hashTable);//free the memory occupied by the hash table
void printHelp();//prints the help usage option
HashTable* ReadFile(char* file);


int main(int argc, char* argv[]){
	
	// check input argument length
	if(argc != 3 && argc != 5){
        printf("No. of arguments  = %d.\n",argc);
        printHelp();
		return 1;
	}
	
	//directory name
    	int dirSize = strlen(argv[1]);
    	char dir[dirSize + 1];
    	dir[0] = '\0';
    	strcat(dir, argv[1]);

	//file name
	int fileSize = strlen(argv[2]);
	char fileGiven[fileSize + 1];
	fileGiven[0] = '\0';
	strcat(fileGiven, argv[2]);
	
	//check directory validity
	if(IsDir(dir) == 0){
		printf("Incorrect directory path. \n");
		printHelp();
        return 1;
	}
	
	
	/*Initialize data structures*/

	//allocate Inverted_index, zero it, and set links to NULL.
	HashTable* hashTable = initHashTable(); //Inverted_index	
	
	int docId;
	int pos;
	char *word;
	char *doc;
	char **fileNames;
	int numFiles = 0;
	numFiles = GetFilenamesInDir(dir, &fileNames);
	if(numFiles < 0) { 
		printf("Couldn't get any files\n");
		exit(1);
	} 
	else {
		for(int i = 0; i < numFiles; i++) {
			/*if (fileNames[i] != NULL)
                printf("filename: %s\n",fileNames[i]);
            else
                fprintf(stderr,"Invalid filename\n");*/
            
            char* pathVar = createPath(dir, fileNames[i]);

			doc = loadDocument(pathVar);
			free(pathVar);
			docId = GetDocumentId(fileNames[i]);
			
			if(docId != 0){
				pos = 0;
				while ((pos = GetNextWord(doc, pos, &word)) > 0){
					NormalizeWord(word);
					AddToHashTable(hashTable, word, docId);
					free(word);
				}
				free(fileNames[i]);
			}
			free(doc);
		}
		free(fileNames);
	}

	SaveIndexToFile(hashTable, fileGiven);	
    freeHashTable(hashTable);
	
    //testing mode
	if(argc == 5){
		int argSize = strlen(argv[3]);//third argument
		char argThird[argSize + 1];
		argThird[0] = '\0';
		strcat(argThird, argv[3]);

		if(strcmp(fileGiven, argThird) != 0){//compare third and fourth arg
			printf("Test Mode: Arg 3 and Arg 4 must be same");
			exit(1);
		}
		
		int argSize1 = strlen(argv[4]);//fourth argument
		char finalArg[argSize1 + 1];
		finalArg[0] = '\0';
		strcat(finalArg, argv[4]);

		if(strcmp(argThird, finalArg) == 0){//compare 4th and 5th arg
			printf("Test Mode: Arg 4 and Arg 5 are same");
			exit(1);
		}
		
		printf("Entering test mode\n");
		HashTable* hashTemp = ReadFile(argThird);
		SaveIndexToFile(hashTemp, finalArg);
		freeHashTable(hashTemp);
	}	

	return 0;
}

/*Loads the html document. 
 *
 * Input
 * filePath: char*, path of the file to be loaded
 *
 * Output: char*, a ptr to chr containing the loaded document.
 *
 */
char* loadDocument(char* filePath){
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

/*Creates path for a specific fileName.
 *
 *Input
 *dirName: char*, name of the directory 
 *fileNum: char*, name of the file
 *
 * Output
 * char*, path to the file 
 */
char* createPath(char* dirName, char* fileNum){
	char * baseName = "%s/%s";
	char *pPath = calloc(1, strlen(dirName) + strlen(fileNum)+1);
	sprintf(pPath, baseName, dirName, fileNum);
	return pPath;	
}

/*Returns numeric id number from a document name by converting the
 * document from string to numeric. 
 *
 *Input
 *fileName: char*, name of the file
 *
 * Output
 * int, number associated with the document id
 *
 */
int GetDocumentId(char* fileName){
	return atoi(fileName);//return 0 if conversion from string to int unsuccessful.
}

/*Saves the index(inverted) to a file. 
 *
 * Input
 * my_hash_table: 
 * saves the inverted index to a file. Input: Pointer to the inverted index and to a file where we have to print
 *
 *
 *Output: 1 if success, else 0
 *
 */
int SaveIndexToFile(HashTable *my_hash_table, char *log_name)
{  
  int word_num = 0;
  FILE *fpt = fopen(log_name, "w"); //open file
  char *base_name = "%s %d ";//base name
  char *doc_freq = "%d %d ";//document frequency

  // iterate through hash_table index
  for (int hash_index = 0; hash_index <= MAX_HASH_SLOT; hash_index++)
  {
    HashTableNode *actual = my_hash_table->table[hash_index];
    while (actual != NULL)
    { 
      if (actual->word != NULL)
      { 
        int number_of_documents = 0;

        char *buffer = calloc(1, 40000);
        buffer[0] = '\0';
        char *final = calloc(1, 40000);
        buffer[0] = '\0';

        DocumentNode *current_doc = actual->page;   
        while (current_doc != NULL)
        {
        
          char *temp = calloc(1, 100);
          temp[0] = '\0';

          //concatenate doc id and freq to the buf
          sprintf(temp, doc_freq, current_doc->doc_id, current_doc->freq);

          strcat(buffer, temp);
          free(temp);
        
          current_doc = current_doc->next;//next doc
          number_of_documents+=1;//no. of docs
        }
        
        
        sprintf(final, base_name, actual->word, number_of_documents);
        strcat(final, buffer);
        strcat(final, "\n");
        fprintf(fpt, final);
        free(buffer);
        free(final); 
        
        word_num+=1;
      }
      actual = actual->next;
    }
 }
fclose(fpt);
return word_num;
}

/*Free the memory allocated to the hashtable.
 *
 *Input
 *hash: hashtable
 *
 */
void freeHashTable(HashTable* hash){

	for(int slot = 0; slot < MAX_HASH_SLOT; slot++){
		//If the word at the slot is null
		if(hash->table[slot]-> word == NULL){
			free(hash->table[slot]);
		}
		else{//if there is a word
			HashTableNode* currNode = hash->table[slot];
			while(currNode->next != NULL){
				HashTableNode* tempCurrNode = currNode;
				
				currNode = currNode->next;
				DocumentNode* docNode = tempCurrNode->page;
				//check for the document node
				while(docNode->next != NULL){
					DocumentNode* tempNode = docNode;
					docNode = docNode->next;
					free(tempNode);
				}
				free(docNode);
				
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
			free(currNode);
		}
	}
	free(hash);
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
 *prints the help options, including usage for arguments. 
 */
void printHelp()
{
    printf("====================================\n");
    printf("Incorrect Usage - Please enter the argument in one of the following formats.\n");
    printf("Indexing Usage: ./indexer  [TARGET_DIRECTORY] [RESULTS FILE NAME]\n");
    printf("Indexing & Testing Usage:  ./indexer  [TARGET_DIRECTORY] [RESULTS FILENAME] [RESULTS FILENAME] [REWRITEN FILENAME]\n");
}


























