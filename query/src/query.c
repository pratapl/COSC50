/* ========================================================================== */
/* File: query.c - Tiny Search Engine
 *
 * Author: Pratap Luitel
 * Date: 5/22/2015
 * Project: TSE, Query
 *
 * Argument options:
 * indexer.dat - built by the indexer
 * data - a folder containing html files info, built by the crawler
 * 
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
// ---------------- Local includes
#include "queryHelper.h"
#include "hashtable.h"
#include "../util/common.h"
#include "../util/file.h"
// ---------------- Constant definitions:

//---------------- Macro definitions: n/a

//---------------- Structures/Types HashTableNode, HashTable, ListNode List

//---------------- Private variables: n/a

//---------------- Private prototypes: n/a

//---------------- Global Variables
/* ========================================================================== */


int main(int argc, char* argv[])
{
	
    // Program parameter processing
	if(argc != 3){
		printf("Error: Incorrect usage\n");
		printf("Query Usage: ./query [indexed data(eg. indexer.dat)] [html data(eg. data)]\n");
		return 1;
	}

	//Get the supplied directory name.
    	int dirSize = strlen(argv[2]);
    	char htmlDirectory[dirSize + 1];
    	htmlDirectory[0] = '\0';
    	strcat(htmlDirectory, argv[2]);

	//Get the fileName.
	int fileSize = strlen(argv[1]);
	char indexedFile[fileSize + 1];
	indexedFile[0] = '\0';
	strcat(indexedFile, argv[1]);
	
    if(IsFile(indexedFile) == 0){
        printf("Incorrect path for indexed file\n");
        return 1;
    }

   	//Check if the path provided is a valid directory.
	if(IsDir(htmlDirectory) == 0){
		printf("Incorrect path for html directory\n");
        return 1;
	}
        
DocumentNode *final = NULL;
DocumentNode* orList[MAX_INPUT]; //OR's
HashTable *tempHashTable = initHashTable(); 

// recreate the inverted index
tempHashTable = ReadFile(indexedFile); 
int revert = SaveIndexToFile(tempHashTable,indexedFile);

if (revert == 0)
    printf("0 Returned from inverting\n");

printf("Satrting query..\n");

//Queries
char inp[MAX_INPUT];
char buff[MAX_INPUT];
int orFlag;
int orIndex;

//loop until user exits
printf("Query:>");

LABEL:while ((fgets(inp,MAX_INPUT,stdin)))
{ 
  
  printf("Query:>");
 
  for (int index = 0; index < MAX_INPUT; index++){
    orList[index] = NULL;//init list elements to null
  }
  
  orFlag = 999;
  orIndex = 0;
  
  // if its a blank enter
  if (strcmp(inp, "\n") == 0){
    fprintf(stderr, "You entered a blank line. Please enter query words!\n");
    continue;
  }  
 
  // remove trailing newline
  char *pos;
  if ((pos=strchr(inp, '\n')) != NULL){
    *pos = '\0';
  }
  
  // check for the last word
  strcpy(buff, inp);
  char *isLast;
  char *lastWord;
  isLast = strtok(buff, " ");
  
  // find the last word
  while (isLast != NULL){
    lastWord = isLast;
    isLast = strtok(NULL, " ");
    
    // check for AND OR and OR AND consecutively
    if (isLast != NULL)
    {
      if ((strcmp(lastWord, "AND") == 0) || (strcmp(lastWord, "OR") == 0))
      {
        if ((strcmp(isLast, "OR") == 0) || (strcmp(isLast, "AND") == 0))
        {
          fprintf(stderr, "Two consecutive query words is invalid. Please try again.\n");
          goto LABEL;
        }
      }
    }
  }
  
  
  if ((strcmp(lastWord, "AND") == 0) || (strcmp(lastWord, "OR") == 0))
  {
    fprintf(stderr, "Last word in query is invalid: %s\n", lastWord);
    continue;
  }

  
  
  char *words;
  words = strtok(inp, " "); //break input on spaces

  //first word validity
  if ((strcmp(words, "AND") == 0) || (strcmp(words, "OR") == 0))
  {
    fprintf(stderr, "First word in query is invalid: %s\n", words);
    continue;
  }
  NormalizeWord(words);//normalize the first valid word
  final = getDocumentList(words, tempHashTable, final);//init doc list
  
  
  words = strtok(NULL, " ");
  // return the list for a one word query(next = null)
  if (words == NULL)
  { 
    final = querySort(final);//recursive sort
    printResult(final, htmlDirectory);//display
           
    freeDocumentList(final);
    final = NULL;     
    continue;
  }
       
  // loop through all words in the tokenized word list
  while (words != NULL)
  {  
    
    // AND 
    if ((strcmp(words, "AND")) == 0) 
    {
      words = strtok(NULL, " ");//next word
      NormalizeWord(words);
      
      // AND with the current word
      final = ANDMethod(words, tempHashTable, final);
      // progress word count
      words = strtok(NULL, " ");
      continue; 
    }
    // if neither AND or OR, rule of precedence dictates an AND
    else if (!(strcmp(words, "OR") == 0))
    {
      // AND with the current word
      NormalizeWord(words);
      final = ANDMethod(words, tempHashTable, final);
      // move the pointer and continue
      words = strtok(NULL, " ");
      continue; 
    }

    // OR cases
    else
    {
      
      words = strtok(NULL, " ");//OR operation for the next word
      orFlag = 0;
      orList[orIndex] = final;//put the final word in the orList
      orIndex++;
      final = NULL;//set finals pointers to null
      NormalizeWord(words);
      // get the list for that word
      final = getDocumentList(words, tempHashTable, final);
      //perm list set up progress word
      words = strtok(NULL, " ");
      continue;
      
    }  
  }
  //checked all the words so far
  if (orFlag != 0)
  { 
    final = querySort(final);//sort the final
    printResult(final, htmlDirectory);
    if (final != NULL)
    {  
      freeDocumentList(final);//free the final
      final = NULL;
    }
    continue;
  }
 
  else if (orFlag == 0)
  { 
    //loop though the orList and or everything with final
    for (int k = 0; k < orIndex; k++)
    {
      final = ORMethod(final, orList[k]);  
    }
    
    final = querySort(final);//sort

    printResult(final, htmlDirectory);//display
    freeDocumentList(final);//free
    final = NULL;
   
    for (int o = 0; o < orIndex; o++)
    {
      freeDocumentList(orList[o]);//free the orList
    }    

  }
  
}
  freeHashTable(tempHashTable);

  if (final != NULL)
  {
    freeDocumentList(final);//free final
  }
  return 0;
}

