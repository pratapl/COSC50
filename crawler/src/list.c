/*
 * File: list.c
 *
 * Project name: CS50 Tiny Search Engine
 * 
 * Author: Pratap Luitel
 * Date: 5/6/15
 *
 * You should include in this file your functionality for the list as 
 * described in the assignment and lecture.
 */
/* ======================================================================= */

// -------------------- Open Issues

// -------------------- System Includes
#include <string.h>
#include <stdlib.h>

// -------------------- Local Includes
#include "common.h"
#include "list.h"
#include "utils.h"

//initialize the list
List* initializeList(){
	List* list = (List*) malloc(sizeof(List));
	MALLOC_CHECK(stderr, list);
	//Create starting nodes.
	ListNode* firstNode = (ListNode*)malloc(sizeof(ListNode));
	MALLOC_CHECK(stderr, firstNode);
	firstNode->page = NULL;
	firstNode->prev = NULL;
	firstNode->next = NULL;


	//the first node is also the tail
	list->head = firstNode;
	list->tail = firstNode;

	return list;
}


/*this function adds the list to the webpage
*list: list to be added
*webPage: page to add the list to 
*
*The function returns 0 upon successfuly adding the list.
*/
int AppendList(List * list, WebPage * webPage){
	//list node to add
	ListNode* node = (ListNode*) malloc (sizeof(ListNode));
	MALLOC_CHECK(stderr, node);

	//if nodes in the list is not present
	if(list->head == NULL && list->tail == NULL){
		node->page = webPage;
		node->prev = NULL;
		node->next = NULL;
		list->head = node;
		list->tail = node;
		return 0;
	}
	
	//add at the end of the queue
	list->tail->next = node;
	node->page = webPage;
	node->prev = list->tail;
	node->next = NULL;
	list->tail = node;

	return 0;
}

/*This function returns a webPage from the list
*It deletes the popped webPage from the actual list. 
*
*list: webpage corresponding to this list is popped
*
*/
WebPage* PopList(List* list){
	//Temporarily store the webPage from the node that we are about to delete
	WebPage* tempWebPage;
	
    tempWebPage = list->head->page;
	ListNode* node = list->head;	

	//Remove the node at the top of the list.
	if(node->next != NULL){	
		list->head = node->next;
		list->head->prev = NULL;
	}
	else{
		list->head = NULL;
		list->tail = NULL;

    }	
	//Free memory
	free(node);
	return tempWebPage;
}

/*
int  main(){
	List* testList = initializeList();
	WebPage* testPage1 = calloc(1, sizeof(WebPage));
	WebPage* testPage2 = calloc(1, sizeof(WebPage));
	testPage1->url = "www.URL1.com";
	testList->head->page = testPage1;

    printf("=======appended a page=======\n");
	printf("list->tail->page->url: %s\n", testList->tail->page->url);
	printf("list->tail->prev: %p\n", (void*)testList->tail->prev);
	printf("list->tail->next: %p\n", (void*)testList->tail->next);
	

    printf("=======appended a page=======\n");
	testPage2->url = "www.URL2.com";
	AppendList (testList, testPage2);
	printf("List->tail->page->url: %s\n", testList->tail->page->url);
	printf("List->tail->prev: %p\n", (void*)testList->tail->prev);
	printf("List->tail->next: %p\n", (void*)testList->tail->next);
	

    printf("=======popped a page=======\n");
	WebPage* pop = PopList(testList);
	printf("Popped url: %s\n", pop->url);
	printf("List->tail->prev: %p\n", (void*)testList->tail->prev);
	
    printf("=======popped a page=======\n");
	WebPage *pop2 = PopList(testList);
	printf("Popped URL: %s\n", pop2->url);
	printf("List->head: %p", (void*)testList->head);
	
	return 0;
}
*/
