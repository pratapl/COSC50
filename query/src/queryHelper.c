/* ========================================================================== */
/* File: queryHelper.c - Tiny Search Engine
 *
 * Author: Pratap Luitel
 * Date: 5/22/2015
 *
 * This is a helper function for query. 
 */
/*============================================================================*/

// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
// ---------------- Local includes
#include "../util/common.h"
#include "hashtable.h"
#include "queryHelper.h"
// ---------------- Constant definitions:

//---------------- Macro definitions: n/a

//---------------- Structures/Types HashTableNode, HashTable, ListNode List

//---------------- Private variables: n/a

//---------------- Private prototypes: n/a

//---------------- Global Variables
/* ========================================================================== */


/*
 * NormalizeWord - lowercases all the alphabetic characters in word
 * 
 * Input
 * word: a ptr to the word to normalize
 *
 */
void NormalizeWord(char *word)
{
    for(char *ptr = word; *ptr; ++ptr)
        *ptr = tolower(*ptr);
    
}

/*
 * This function returns checks for a specific element is in the hashtable. 
 *
 * Input
 * word_arg: a ptr to the word to be checked
 * my_hash_table: the table to search in
 *
 * Output
 * A ptr to the document list if found
 * Null, else
 *
 * this is similar to the InHashTable method in hashtable.c except that this 
 * returns a doc node as opposed to an int. 
 */
 
DocumentNode* IsInHashTable(char *word_arg, HashTable *my_hash_table)
{
  unsigned long index = JenkinsHash(word_arg, MAX_HASH_SLOT);

  if (my_hash_table->table[index]->word == NULL)
  {
    //printf("word: %s not in hash table\n", word_arg);
    return NULL;
  }
  else if ((strcmp(my_hash_table->table[index]->word, word_arg)) == 0)
  {
    //printf("word: %s found in first slut\n", word_arg);
    return my_hash_table->table[index]->page;
  }

  HashTableNode *actual = my_hash_table->table[index];
  while ((actual->next != NULL))
  {
    actual = actual->next;
    if ((strcmp(actual->word, word_arg)) == 0)
    {
      //printf("word %s found in collision list", word_arg);
      return actual->page;
    }
  }
  // else it wasnt found anywhere
  return NULL;
}

/*
 * This function frees a DocumentNode
 *
 * Input
 * current_document: a ptr to the document node to be freed
 *
 */

void freeDocumentList(DocumentNode *current_document)
{
  while (current_document != NULL)
  {
    DocumentNode *next_document = current_document->next;
    free(current_document);
    current_document = next_document;
  }
}

/* This function checks if a documentNode exists in a hashtable
 * and returns the node.
 *
 * Input
 * word: a ptr to the word
 * my_hashTable: a ptr to the hashtable to be checked
 * final:  a ptr to the document node
 *
 * Output:
 * a document node 
 *
 */
DocumentNode* getDocumentList(char *word, HashTable *my_hashtable, DocumentNode *final)
{

  DocumentNode *current = IsInHashTable(word, my_hashtable);

  // check if it exists
  if (current == NULL)
  {
    fprintf(stderr, "word: %s does not exist in any documents\n", word);
    // just return whatever was passes in
    return final;
  }

  //if final is null that means it is the first so just put everything there
  if (final == NULL)
  {
    // allocate memory for final
    final = calloc(1, sizeof(DocumentNode));
    DocumentNode *tail_of_temp = final;
    // loop through current and build the final list
    while (current != NULL)
    {
      // set the doc id and frequency and next NULL by default
      tail_of_temp->doc_id = current->doc_id;
      tail_of_temp->freq = current->freq;
      tail_of_temp->next = NULL;
      // progress the current
      current = current->next;
      if (current != NULL)
      {
        tail_of_temp->next = calloc(1, sizeof(DocumentNode));
        tail_of_temp = tail_of_temp->next;
      }
    }
   return final;
 }
 // else just return what it was
return final;
}


/*
 * This function ANd's the document lists
 *
 * Input
 * word:a ptr to the word
 * my_hashtable:a ptr to the hashtable
 * final:a ptr to the documentnode
 *
 * Output
 * the ANDed document lists
 *
 */

DocumentNode* ANDMethod(char *word, HashTable *my_hashtable, DocumentNode *final)
{

DocumentNode *t = NULL;
DocumentNode *temporary = NULL;
// get the document list from hahstable
DocumentNode *current = IsInHashTable(word, my_hashtable);
    // check if it exists
    if (current == NULL)
    {
      fprintf(stderr, "word: %s does not exist in any documents\n", word);
      // just return whatever was passes in
      if (final != NULL)
      {
        freeDocumentList(final);
        final = NULL;   
      }
      return final; 
    }
    // else it exists so lets AND it
    else
    {
      //if temp is null that means it is the first so just put everything there
      if (final == NULL)
      { 
      
      return NULL;
        
      }
      // we need to find which documents are common
      else
      { 
        // set the tail to final
        DocumentNode *tail_temp = final;
        // loop through both lists simoultaneously to find the first common match
        while (current != NULL && tail_temp != NULL && ((current->doc_id) != (tail_temp->doc_id)))
        {
          if (tail_temp->doc_id < current->doc_id)
          { 
            tail_temp = tail_temp->next;
          }
          else
          {
            current = current->next;
          }
        }
        // when we leave this loop there are 2 possibilities, we found the first common document, or we reached the end of our lists 
        // if we didnt find anything in common:
        if (current == NULL || tail_temp == NULL)
        { 
          // free the final and re-set it for NULL
          freeDocumentList(final);

          return NULL;
        }
        // we found the first common link
        else
        { 
          // we found at least one in common so allocate the temporary list
          temporary = calloc(1, sizeof(DocumentNode));
          temporary->doc_id = tail_temp->doc_id;
          temporary->freq = (tail_temp->freq + current->freq);
          temporary->next = NULL;
          // progress the current
          current = current->next;
          t = temporary;
        }
        // progress the tail
        while ((tail_temp != NULL) && (current != NULL))
        { 
             
          if (current->doc_id > tail_temp->doc_id)
          {
            tail_temp = tail_temp->next;
            continue;
          }
          if (current->doc_id == tail_temp->doc_id)
          { 
            // create a new node
            t->next = calloc(1, sizeof(DocumentNode));
            t = t->next;
            t->doc_id = tail_temp->doc_id;
            t->freq = (tail_temp->freq + current->freq);
            t->next = NULL;	
            
            // progress current and tail temp
            tail_temp = tail_temp->next;
            current = current->next;
            continue;
          }
          current = current->next; 
        }
        // delete the old list and assign the new list
        freeDocumentList(final);
        final = temporary;
        // return the new list
        return final;
      }
    }
}


/*
 * This function OR's the document lists
 *
 * Input
 * word:a ptr to the word
 * my_hashtable:a ptr to the hashtable
 * final:a ptr to the documentnode
 *
 * Output
 * the OR'ed document lists
 *
 */

DocumentNode* ORMethod(DocumentNode *final, DocumentNode *curr)
{

DocumentNode *t = NULL;
DocumentNode *temporary = NULL;
// doc list is passed in
DocumentNode *current = curr;
// check if it exists
if (current == NULL)
{
  fprintf(stderr, "A doc list in the or method was null, thats ok\n");
  // just return whatever was passes in
  return final;
}
// else it exists so let's OR it with final, exciting
else
{
  //if final is null that means it is the first so just put everything there
  if (final == NULL)
  {
    // allocate memory for final
    final = calloc(1, sizeof(DocumentNode));
    DocumentNode *tail_of_temp = final;
    // loop through current and build the final list
    while (current != NULL)
    {
      // set the doc id and frequency and next NULL by default
      tail_of_temp->doc_id = current->doc_id;
      tail_of_temp->freq = current->freq;
      tail_of_temp->next = NULL;
      // progress the current
      current = current->next;
      if (current != NULL)
      {
        tail_of_temp->next = calloc(1, sizeof(DocumentNode));
        tail_of_temp = tail_of_temp->next;
      }
   } 
   return final;
  }
  // else we need to find which new documents in current are in final
  else
  { 
    // allocate space
    temporary = calloc(1, sizeof(DocumentNode));
    t = temporary;      
 
    // set the tail to final
    DocumentNode *tail_temp = final;
    // first loop through the final list
    while (tail_temp != NULL)
    { 
      // as long as we havent reached the end of current
      if (current != NULL)
      { 
        
        if (tail_temp->doc_id < current->doc_id)
        { 
          t->doc_id = tail_temp->doc_id;
          t->freq = tail_temp->freq;
          t->next = NULL;
          // progress 
          tail_temp = tail_temp->next;
          // create node
          if (tail_temp != NULL || current != NULL)
          {
            t->next = calloc(1, sizeof(DocumentNode));
            t = t->next;
          }
        }
        else if (tail_temp->doc_id == current->doc_id)
        { 
          t->doc_id = tail_temp->doc_id;
          t->freq = (tail_temp->freq + current->freq);
          t->next = NULL;
          // progress both
          current = current->next;
          tail_temp = tail_temp->next;

          if (tail_temp != NULL || current != NULL)
          {
            t->next = calloc(1, sizeof(DocumentNode));
            t = t->next;
          }
          
        }
        else if (tail_temp->doc_id > current->doc_id)
        {
          t->doc_id = current->doc_id;
          t->freq = current->freq;
          t->next = NULL;
          // progress
          current = current->next;
          if (tail_temp != NULL || current != NULL)
          {
            t->next = calloc(1, sizeof(DocumentNode));
            t = t->next;
          }
        }
        
     }
      // else current equals null so we must add the rest of our final list
      else
      {
        t->doc_id = tail_temp->doc_id;
        t->freq = tail_temp->freq;
        t->next = NULL;
        // progress tail temp
        tail_temp = tail_temp->next;
        if (tail_temp != NULL)
        {
          t->next = calloc(1, sizeof(DocumentNode));
          t = t->next;
        }
      }
    }
    // if we reached the end of final, but current still has members, append them to t
    while (current != NULL)
    {
      t->doc_id = current->doc_id;
      t->freq = current->freq;
      t->next = NULL;
      // progress
      current = current->next;
      if (current != NULL)
      {
        t->next = calloc(1, sizeof(DocumentNode));
        t = t->next;
      }
    }

   freeDocumentList(final);
   final = temporary;
   return final;        
  }
}
return NULL;
}
/* This function recursively sorts the linked list of nodes. 
 *
 * Input
 * final: a ptr to the document node to be sorted
 * 
 * Output
 * sorted document node
 *
 */
DocumentNode* querySort(DocumentNode *final)
{
  //base case:
  if (final == NULL || final->next == NULL)
  {
    return final;
  }
  DocumentNode *current_node = final;
  DocumentNode *previous = final;
  DocumentNode *largest = final;
  DocumentNode *previous_largest = final;
  
  // find the largest node
  while (current_node != NULL)
  {
    if (current_node->freq > largest->freq)
    {
      // largest is current
      previous_largest = previous;
      largest = current_node;
    }
    // progress in list
    previous = current_node;
    current_node = current_node->next;
  }
  // move the largest to the beginning
  DocumentNode *temp;
  
  if (largest != final)//if the largest isnt head
  {
    previous_largest->next = final;
    temp = final->next;
    final->next = largest->next;
    largest->next = temp;
  } 
  //recursive calls to the remaining elements
  largest->next = querySort(largest->next);
  return largest;
}

void testPrint(DocumentNode *current_document)
{
  while (current_document != NULL)
  {
    DocumentNode *next_document = current_document->next;
    printf("doc_id: %d frequency:%d\n", current_document->doc_id, current_document->freq);
    current_document = next_document;
  }
  printf("DONE\n\n");
}

void printResult(DocumentNode *final, char *raw_path)
{ 
  char *file_path_template = "%s/%d";
  int path_length = strlen(raw_path);
  char path[path_length+6];
  path[0]='\0';  


  DocumentNode *temp_next = final;
  
  if (final == NULL)//query didnt succeed
  {
    printf("Query did not succed. Sorry!\n");
    return; 
  }
  else
  { 
    char line[MAX_INPUT];
    while (temp_next != NULL)
    {           
      sprintf(path, file_path_template, raw_path, temp_next->doc_id);
      FILE *pt = fopen(path, "r");
      if (pt == NULL)
      {
        fprintf(stderr, "File couldnt be open. Incorrect directory.!\n");
        exit(1);
      }
      // print the line
      printf("Document ID:%d, Frequency:%d, URL:%s\n", temp_next->doc_id, temp_next->freq, (fgets(line,MAX_INPUT,pt)));
      fclose(pt);
      strcpy(path, "");//clear the path
      temp_next = temp_next->next;//get to the next node
    }
 
  }
}


