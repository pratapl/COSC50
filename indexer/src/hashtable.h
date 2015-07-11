/* ========================================================================== */
/* File: hashtable.h
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Indexer
 *
 * This file contains the definitions for a hashtable of urls.
 *
 */
/* ========================================================================== */
#ifndef HASHTABLE_H
#define HASHTABLE_H
// ---------------- Prerequisites e.g., Requires "math.h"
#include "common.h"

// ---------------- Constants
#define MAX_HASH_SLOT 10000                  // number of "buckets"

// ---------------- Structures/Types

//Equivalent to a word node.
typedef struct HashTableNode {
    char *word;                           //key for the dictionary key-value pairs.
    struct HashTableNode *next;            	     // points to the next word for collisions.
    DocumentNode *page;			//pointer to the first element of the page list.
} HashTableNode;

typedef struct HashTable {
    HashTableNode *table[MAX_HASH_SLOT];     // actual hashtable
} HashTable;

// ---------------- Public Variables

// ---------------- Prototypes/Macros

/*
 * jenkins_hash - Bob Jenkins' one_at_a_time hash function
 * @str: char buffer to hash
 * @mod: desired hash modulus
 *
 * Returns hash(str) % mod. Depends on str being null terminated.
 * Implementation details can be found at:
 *     http://www.burtleburtle.net/bob/hash/doobs.html
 */
 unsigned long JenkinsHash(const char *str, unsigned long mod);
 
//Initialize Hash Table
HashTable * initHashTable();

//Add to HashTable - returns 0 if successful	, 1 otherwise.
int AddToHashTable(HashTable* htable, char *Word, int docId); //added a key value pair

//InHashTable - returns 1 if successful, 0 otherwise.
int InHashTable(HashTable* hTable, char *Word); //look for a key in HashTable.

#endif // HASHTABLE_H
