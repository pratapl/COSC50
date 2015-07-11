/* ========================================================================== */
/* File: hashtable.h
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Crawler
 *
 * This file contains the definitions for a hashtable of urls.
 *
 */
/* ========================================================================== */
#ifndef HASHTABLE_H
#define HASHTABLE_H

// ---------------- Prerequisites e.g., Requires "math.h"
#include "common.h"                          // common functionality

// ---------------- Constants
#define MAX_HASH_SLOT 10000                  // number of "buckets"

// ---------------- Structures/Types

typedef struct HashTableNode {
    char *url;                               // url previously seen
    struct HashTableNode *next;              // pointer to next node
} HashTableNode;

typedef struct HashTable {
    HashTableNode *table[MAX_HASH_SLOT];     // actual hashtable
} HashTable;

// ---------------- Public Variables

// ---------------- Prototypes/Macros



/* looks up URL in HashTable. 
 * URL: url to look up 
 * hTable: hashtable to search url in
 *
 * returns: 1 if url is in hashtable, else 0
 */
int HashTableLookup(HashTable* hTable, char* URL);

/* initialize a hashtable with MAX_SLOT_HASHTABLE slots in the table. 
 * MAX_SLOT_HASHTABLE is defined in utils.h and equals 4000
 * */
HashTable* initHashTable();


/*Insert URL into a hashtable
 * hTable: hashtable to insert URL 
 * URL: url to insert into the table
 *
 * returns: 1 on success, 0 on failure
 *
 * Note: if the URL already exists in the table, 
 * returns 1. 
 */

int HashTableInsert(HashTable* hTable, char* URL);

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

#endif // HASHTABLE_H
