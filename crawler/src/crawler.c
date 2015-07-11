/* ========================================================================== */
/* File: crawler.c - Tiny Search Engine web crawler
 *
 * Author:Pratap Luitel
 * Date:5/6/2015
 *
 * Input:[seedurl] [directory location] [depth]
 *
 * Output:saved data files named in increasing number. i.e 1, 2, etc. 
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
#include <curl/curl.h>                       // curl functionality
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>

// ---------------- Local includes  e.g., "file.h"
#include "common.h"                          // common functionality
#include "web.h"                             // curl and html functionality
#include "list.h"                            // webpage list functionality
#include "hashtable.h"                       // hashtable functionality
#include "utils.h"                           // utility stuffs

// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes

/* ========================================================================== */


char * pathToDirectory(char *, int); //path to directory for saving contents of html
void writeHTMLtoFile(FILE*, WebPage*);//save the contents of html
WebPage* webPageInit(char*, int); //initialize webPage struct
void printHelp();//print help message

int depth = 0;
int fileNumber = 1;

WebPage* tempWebPage; 


int main(int argc, char* argv[])
{
    
    //Check for the number of arguments
    if(argc != 4){
	    printf("Invalid Input Argument\n");
	    printHelp();
        exit(1);
    } 
   
    //direcotry file path
    int dirSize = strlen(argv[2]);
    char dir[dirSize + 1];
    dir[0] = '\0';
    strcat(dir, argv[2]);

    int urlSize = strlen(argv[1]);
    char inputURL[urlSize + 1];
    inputURL[0] = '\0';
    strcat(inputURL, argv[1]);

    //Get the max depth number.
    int inputDepth = atoi(argv[3]);

    //Check if correct depth is provided.
    if(inputDepth > 4 || inputDepth < 0){
        printf("Invalid [depth]\n");
        printHelp();
        exit(1);
    }
    //Check for URL validity 
    if(!strstr(inputURL,URL_PREFIX)){
 	    printf("Invalid input [seed url]\n");
        printHelp();
	    exit(1);
    }
    //checkf for directory location validity
    DIR* directory = opendir(dir);
    if(directory){
	    closedir(directory);
    }
    else if(ENOENT == errno){
	    printf("Directory does not exist\n");
	    printHelp();
        exit(1);
    }
    else{
	    printf("Directory can't be opened\n");
        printHelp();
	    exit(1);
    }

    // init curl
    curl_global_init(CURL_GLOBAL_ALL);

    // setup seed page
    WebPage* seedWebPage = calloc(1, sizeof(WebPage));//Memory allocation for seed webpage
    seedWebPage->url = calloc((strlen(inputURL) + 1), sizeof(char));//Memory allocation to the seedURL
    seedWebPage->url[0] = '\0';
    strcat(seedWebPage->url, inputURL);
    seedWebPage->depth = 0;
    seedWebPage->html = NULL;
    
    //Initialize data structures
    HashTable* visitedURLHash = initHashTable();
    List* webPageList = initializeList();
    webPageList->head->page = seedWebPage;  
 
    //get seed webpage.
    if(GetWebPage(seedWebPage)){	
        // write seed file
        FILE *fPointer;
        char* pathVar1 = pathToDirectory(dir, fileNumber);
        fPointer = fopen(pathVar1, "w");
        free(pathVar1);	
        writeHTMLtoFile(fPointer, seedWebPage);
        //free(fPointer);
        
        if(inputDepth == 0){
            curl_global_cleanup();
            free(seedWebPage->html);
            free(seedWebPage->url);
            free(seedWebPage);

            //free webPageList and hashtable
            free(webPageList);
            for(int i = 0; i < MAX_HASH_SLOT; i++){
                free(visitedURLHash->table[i]->url);
                free(visitedURLHash->table[i]);
            }
            free(visitedURLHash);
            return 0;
        }   
        fileNumber += 1;
        depth += 1;
        HashTableInsert(visitedURLHash, seedWebPage->url); //mark as visited
        
        // extract urls from seed page
        char * result;
        int pos = 0;
        while((pos = GetNextURL(seedWebPage->html, pos, seedWebPage->url, &result))>0){

            if(NormalizeURL(result) && strstr(result,URL_PREFIX)){
                strtok(result, "#");
                //If not in hashtable, add it to the hashtable and add it to the webPageList.
                if(HashTableLookup(visitedURLHash, result) == 0){
                    HashTableInsert(visitedURLHash, result);
                    AppendList(webPageList, webPageInit(result, depth));
                    free(result);
                }
            }
        }
        if(webPageList->head->next->next == NULL){  //seed redirect case
            webPageList->head->next->page->depth = 0;
            fileNumber = 1;		
        }
        tempWebPage = PopList(webPageList); // Get rid of visited seedPage
    }
    else{	
        curl_global_cleanup();
        tempWebPage = PopList(webPageList);
        free(seedWebPage->html);
        free(seedWebPage->url);
        free(seedWebPage);
        //free(tempWebPage);
        free(webPageList);
        for(int i = 0; i < MAX_HASH_SLOT; i++){
            free(visitedURLHash->table[i]->url);
            free(visitedURLHash->table[i]);
        }
        free(visitedURLHash);
        exit(1);
    }

    
    //while there are urls to crawl
    while(webPageList->head != NULL && webPageList->tail != NULL){
        // get webpage for url
        tempWebPage = PopList(webPageList);
        if(GetWebPage(tempWebPage)){ 
            // write page file
            char* pathVar = pathToDirectory(dir, fileNumber);
            FILE *fPointer = fopen(pathVar, "w");
            free(pathVar);
            printf("Found link: %s\n",tempWebPage->url);
            writeHTMLtoFile(fPointer, tempWebPage);
            fileNumber += 1;
                
            if((tempWebPage->depth + 1) <= inputDepth ){
                char * resultTemp;
                int posTemp = 0;
                while((posTemp = GetNextURL(tempWebPage->html, posTemp, tempWebPage->url, &resultTemp))>0){
                    
                    if( NormalizeURL(resultTemp) && strstr(resultTemp,URL_PREFIX) ){
                        strtok(resultTemp, "#");
                        //insert to the hashtable and the webPageList if not already present
                        if(HashTableLookup(visitedURLHash, resultTemp) == 0){
                            HashTableInsert(visitedURLHash, resultTemp);
                            AppendList(webPageList, webPageInit(resultTemp, tempWebPage->depth+1));
                        }
                    }
                        free(resultTemp);
                }
            }
        
            free(tempWebPage->url);
            free(tempWebPage->html);
            free(tempWebPage);
        }
        else{
            free(tempWebPage->url);
            free(tempWebPage->html);
            free(tempWebPage);
        }
        sleep(INTERVAL_PER_FETCH);
    }
    // cleanup curl
    curl_global_cleanup();
    free(seedWebPage->url);
    free(seedWebPage->html);
    free(seedWebPage);
    free(webPageList);

    //free the hashtable
    for(int i = 0; i < MAX_HASH_SLOT; i++){
        if(visitedURLHash->table[i]->url != NULL){
            HashTableNode* currNode = visitedURLHash->table[i];
            while(currNode->next != NULL){
                HashTableNode* tempNode = currNode;
                currNode = currNode->next;
                free(tempNode->url);
                free(tempNode);
            }
            free(currNode->url);
            free(currNode);		
        }
        else{	
            free(visitedURLHash->table[i]);
        }
    }
    free(visitedURLHash);
    return 0;
}

/*this function returns a pointer to the
 * path of the directory where the files with html data
 * are written to. 
 *
 * dirName: directory name input by user
 * fileNum: int, the unique id for file
 */
char* pathToDirectory(char * dirName, int fileNum){
	char * baseName = "./%s/%d";
	int length = snprintf(NULL, 0, baseName, dirName, fileNum);
	char * str = malloc(length+1);
	sprintf(str, baseName, dirName, fileNum);
	return str;	
}

/*this function writes html content from each visited url
 *to a unique file in the location specified by the user
 *
 *fp: file pointer to the file where contents are written to
 *page: the webPage strucuture corresponding to the visited url
 * 
 */
void writeHTMLtoFile(FILE* fp, WebPage* page){
	fprintf(fp, "%s\n%d\n%s\n", page->url, page->depth, page->html);
	fclose(fp);
}

/* older approach - didnt work
 void writeHTMLtoFile(char *dirName,int fileNum, WebPage* page){
    
    char fullPath[1000];
    
    BZERO(fullPath,1000);
    sprintf(fullPath,%s/%d,dirName,fileNum);
    File *fp = fopen(fullPath,"w");
    
    if (fp == NULL){
        printf("Cant write to a null pointer\n");
        exit(1);
    }
    
	fprintf(fp, "%s\n%d\n%s\n", page->url, page->depth, page->html);  
}
*/

/*
this function initializes a webPage strcut
 * 
 * url: the web url of the page
 * depth: int, the depth of this page
 *
 */
WebPage* webPageInit(char* url, int depth){
	WebPage* tempURL = calloc(1, sizeof(WebPage));
	tempURL->url = calloc((strlen(url)+1), sizeof(char));
	tempURL->url[0] = '\0';
	strcat(tempURL->url, url);
	tempURL->depth = depth;
	tempURL->html = NULL;		
	return tempURL;	
}

/*this function prints help option*/
void printHelp(){
    printf("==============================================================\n");
	printf("Usage: The arguments should be [seedURL] [directory] [depth]\n");
    printf("webpage: only the seed page for CS50: %s\n",URL_PREFIX);
    printf("directory: must already exist\n");
    printf("depth: in the range [0-4]\n");
}


