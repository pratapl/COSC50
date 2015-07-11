/* Pratap Luitel
 * CS50, Dartmouth College
 * Project 3
 * 4/22/2015
 *
 * This program displays weather information based on a valid location
 * code entered by user. It uses the libcurl library and other programs
 * mycurl.h and mycurl.c provided for the assignments. 
 *
 *
 **/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<curl/curl.h>
#include "mycurl.h"
#include "ezxml.h" /*from ezXML online source*/

void printHelp();
//int isValid(char *codeLocation);

int main(int argc, char *argv[])
{
    /*local variables*/

   i /*CURL*/
    CURL *curlHandle;
    CURLcode res;
    struct curlResponse s;

    /*check arguments*/
    if(argc != 2){
        printHelp();
        exit(1);
    }
    
    /*TO DO: check if code location is valid*/
    //if (!isValid(argv[2]))
      //  exit(1);

    /*C standard location code*/
    char locationCode[strlen(argv[1]) + 1];
    locationCode[0] = '\0';
    strcat(locationCode,argv[1]);
    
    /*allocate and initialize output area*/
    /*initializes struct of type curlResponse with pointer *s*/   
    init_curlResponse(&s); 
    
    /*URL format*/
    char *APIurl = "http://w1.weather.gov/xml/current_obs/%s.xml";
    
    /*create the URL string from input argument*/
    char locationURL[strlen(APIurl) + 5];
    locationURL[0] = '\0';
    sprintf(locationURL,APIurl,locationCode);
    
    /*initialize curl*/
    curlHandle = curl_easy_init();
    
    if(curlHandle){
        fprintf(stdout,"built curl handle..\n");
        /* set the url locatin to visit */
        curl_easy_setopt(curlHandle, CURLOPT_URL, locationURL);
        /* set the function to be called with the result */
        curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, writefunc);
        /* set the buffer in which to place the data */
        curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &s);
        /* perform curl request */
        res = curl_easy_perform(curlHandle);
        
        if(res != 0){
            fprintf(stderr,"curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
            exit(1);
        }
        
        int webSize = strlen(s.ptr)+1;
        char *webPage[webSize+1];
        //webPage[0]='\0';
        strncpy(webPage,s.ptr,webSize+1);
        

        char *webPageCopy[webSize+1];
        //webPageCopy[0]='\0';
        strncpy(webPageCopy,s.ptr,webSize+1);

        /*
        char *firstWord = strtok(webPage," ");
        if(strcmp(firstWord,"<?xml") != 0){
            printf("not valid web page");
            exit(1);
        }
        else
            printf("First word is: %s\n",firstWord);*/
         
        /*
        *using ezxml - did not work, gave a lot of errors 
        ezxml_t myXML = ezxml_parse_fp(s.ptr);
        */
        int lineNumber = 1;
        char *lineToken = strtok(webPage,"\n");
     
        /* read and print each line */
        while(lineToken != NULL){
            //printf(" %d: %s \n",lineNumber, lineToken);
            lineNumber++;
            
            char *tempLine[strlen(lineToken)];
            strncpy(tempLine,lineToken,strlen(lineToken)); 
            lineToken = strtok(NULL,"\n");
        }
        
        //printf("length of pLine: %zd \n",strlen(webPage));;
        //printf(webPage);
        
        /*cleanup*/
        curl_easy_cleanup(curlHandle);
    }
    
return 0;
}
void printHelp(){
    fprintf(stdout,"--------------------------------------\n");
    fprintf(stdout,"Please enter ONE valid code location.\n");
    fprintf(stdout,"Code location are 4 characters like KMWN for Mt.Washington.\n");
    fprintf(stdout,"Location codes can be found in the following web url - \n");
    fprintf(stdout,"http://w1.weather.gov/xml/current_obs/seek.php?state=ak&Find=Find\n");
}
