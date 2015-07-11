/* ========================================================================== */
/* File: list.h
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Crawler
 * Secondary Author: Pratap Luitel
 *
 * This file contains the definitions for a doubly-linked list of WebPages.
 *
 */
/* ========================================================================== */
#ifndef LIST_H
#define LIST_H

// ---------------- Prerequisites e.g., Requires "math.h"
#include "common.h"                          // common functionality

// ---------------- Constants

// ---------------- Structures/Types

typedef struct ListNode {
    WebPage *page;                           // the data for a given page
    struct ListNode *prev;                   // pointer to previous node
    struct ListNode *next;                   // pointer to next node
} ListNode;

typedef struct List {
    ListNode *head;                          // "beginning" of the list
    ListNode *tail;                          // "end" of the list
} List;

// ---------------- Public Variables

// ---------------- Prototypes/Macros



#endif // LIST_H

/*Constructor for the list*/
List* initializeList();

      
/*
*This function adds a webpage to the list
*list: list to be added
*webPage: page to add the list to 
*
*The function returns 1 upon successfuly adding the list.
*/
int AppendList(List *list, WebPage *webPage);


/*This function returns a webPage from the list
*It deletes the popped webPage from the actual list. 
*
*list: webpage corresponding to this list is popped
*
*/
WebPage* PopList(List* list);


