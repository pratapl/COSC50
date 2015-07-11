/* ========================================================================== */
/* File: helper.h - Tiny Search Engine
 *
 * Author: Pratap Luitel
 * Date: 5/22/2015
 *
 * Helper function definitions for query
 *
 * Error Conditions: nothing known
 * 
 *
 * Special Considerations:Code is working, no seg faults, should be bug free
 *
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
// ---------------- Constant definitions:
#define MAX_INPUT 1001
//---------------- Macro definitions: n/a

//---------------- Structures/Types HashTableNode, HashTable, ListNode List

//---------------- Private variables: n/a

//---------------- Private prototypes: n/a

//---------------- Global Variables
/* ========================================================================== */


//normalize word
void NormalizeWord(char *word);

// method to free the list
void freeDocumentList(DocumentNode *current_document);

//inHashTable method, returns pointer to document list if found, else returns NULL
DocumentNode* IsInHashTable(char *word_arg, HashTable *my_hash_table);

// get the document list of a word
DocumentNode* getDocumentList(char *word, HashTable *my_hashtable, DocumentNode *final);

// method to AND doc lists
DocumentNode* ANDMethod(char *word, HashTable *my_hashtable, DocumentNode *final);

// the method to OR the list
DocumentNode* ORMethod(DocumentNode *final, DocumentNode *curr);
     
// recursively sort the linked list of document nodes passed in
DocumentNode* querySort(DocumentNode *final);

//print the results
void printResult(DocumentNode *final, char *raw_path);



