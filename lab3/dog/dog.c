/*
 * Pratap Luitel
 * 4/20/15
 *
 *
 * Dog mimicks behaviour of the unix command cat while adding 
 * some extra features like the ability to encrypt or decrypt a file. 
 * Various switches are available to users to print number line, encrypt, 
 * and decrypt the output. The output can be redirected to another file, 
 * or sent to stdout. 
 *
 *
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<ctype.h>

#define LIMIT 1000/* Buffer limit for a line */

/*Variables for getopt*/
extern char *optarg;/* stores input file name */
extern int optind;/* stores index of input file */

/*Global Variables*/
int c;
int nFlag = 0; /* keeps track of (-n) number switch */
int encryptState = 0; /* 1 for encrypt, -1 for decrypt, 0 else */
int err = 0;
char *encryptInfo=NULL; /*file containing encryption or decryption info */
int encryptArray[26][2] = {{0}}; /* stores encryption or decryption information */
int lineNum = 1; 

/*Sub Functions*/
int getopt(int argc, char *const argv[], const char *optstring);
void printHelp();
void printVersion();
void printFile(char *fileName,int printToStdOut,int nFlag,
                        int encryptState,char *encryptFile);
void encrypt(const char *encryptFile, int encryptState);
void printEncryptionError(); 
//main
int main(int argc, char *argv[])
{
        while ((c=getopt(argc,argv,"ntM:m:")) != -1)
        switch(c){
            case 'n':
                nFlag += 1;
                break;
            case 'm':
                if (encryptState == -1)
                    encryptState = 0;
                else
                    encryptState = 1;
                encryptInfo = optarg;
                break;
            case 'M':
                if (encryptState == 1)
                    encryptState = 0;
                else
                    encryptState = -1;
                encryptInfo = optarg;
                break;
            case '?':
                err = 1;
                break;
            default:
                break;
        }
        /* if user input invalid inpur argument */
        if(err){
            fprintf(stderr,"Error, invalid input.\n");
            printHelp();
            exit(1);
        }
        /*if there is no input file*/
        else if(optind + 1 > argc){

            char *fileName = NULL; /*no file to input from*/
            int printFromFile = 0; /*take input from stdin*/
            printFile(fileName,printFromFile,nFlag,encryptState,encryptInfo);

        }
        else{ /*if there is at least an input file*/
            for (int i = optind; i < argc; i++){ 
                
                int printFromFile = 1; /*input text from fileName*/

                /*convert input file names to C style strings*/
                int wordLength = strlen(argv[i]);
                char fileName[wordLength + 1];
                fileName[0]='\0';
                strcat(fileName,argv[i]);
                 
                if(strcmp(fileName,"-") == 0){
                     printFile(NULL,0,nFlag,encryptState,encryptInfo);/*print to stdout*/
                     exit(0);
                }
                
                if (strcmp(fileName,"--help") == 0){
                    printVersion();
                    fprintf(stdout,"----------------------\n");
                    printHelp();
                    exit(0);
                }   
                
                printFile(fileName, printFromFile, nFlag,encryptState, encryptInfo);
            }
            printf("done\n");
        }
            
            
return 0;
}
/* prints help option */
void printHelp()
{
    printVersion();
    printf("----------------------\n");
    printf("Command: dog\n");
    printf("Usage: dog [-n] [-m filename] [-M filename] [filename]\n");
    printf("switch: --help for this option\n");
    
}

/* prints the version information */
void printVersion()
{
    printf("Version: 0.1\n");
    printf("Author: Pratap Luitel\n");
    printf("CS50, Dartmouth College\n");
}

/**
 * This function prints output to stdout. 
 * The input text is either from stdin or from given file/s. 
 * Other arguments reflect various switches of the dog command. 
 * 
 *fileName: ptr of input file, null if stdin
 *printFromFile: int, 1 if printing from a given file, else 0
 *nFlag: int, 1 if numberline is to be printed, else 0
 *encryptState: int, -1 for decrypt, 1 for encrypt, 0 for neither or both
 *encryptInfo: *char to the file containing encrypt/decrypt info
 * */
void printFile(char *fileName, int printFromFile,int nFlag, 
        int encryptState,char * encryptInfo){
    
    
    FILE *fp = fopen(fileName,"r");
    if (fp == NULL){
        if (!printFromFile)
            fp = stdin; /*input is from stdin*/
        else{
            fprintf(stdout,"Input File does not exist.\n");
            exit(1);
        }
    }
    
    /* if encrypt/decrypt flag is on */ 
    if (encryptState == 1 || encryptState == -1)
        encrypt(encryptInfo,encryptState); 
    
    int ch; /* stores int from fgetc*/
    int offset = 0; /* the difference in ascii value for encryption */
    int newLine = 1; /*flag for new line*/
    
    while((ch = fgetc(fp)) != EOF){
        /* if encrypt or decrypt flag is on */
        if(encryptState == 1 || encryptState == -1){ 
            /* if ascii of character is in a-z */
            if (ch > 96 && ch < 123){
                /* +ve offset for encrypt, -ve for decrypt, 0 else */
                offset  =  encryptState * encryptArray[ch-97][1];
            }
            else
                offset = 0;
        }

        /* if number flag is on and its a new line */
        if (nFlag && newLine){
            fprintf(stdout,"%d %c",lineNum,ch + offset);
            lineNum++; 
        }
        else{
            fprintf(stdout,"%c",ch + offset);
        }

        //update newline
        if (ch == 10)
            newLine = 1;
        else
            newLine = 0; 
    }
    fclose(fp);

}
/* This function stores encryption or decryption information
 * in the global encryptArray. The first column of the array stores
 * ascii equivalenct of original character and the second column stores
 * ascii equivalent of the destination characters. 
 *
 *
 * encryptFile: The filename containing encryption or decryption pair.
 *              The file should have 26 pairs of characater mapped one-to-one.
 *              The ascii values of the character can range from 97-122(a to z)
 * */
void encrypt(const char *encryptFile, int encryptState){
    printf("-------Encryption Happeninng-------\n");

    FILE *ep = fopen(encryptFile,"r");
    /* check if the file exists */
    if(ep == NULL){
        fprintf(stdout,"Encryption file does not exist\n");
        exit(1);
    }
                  
    /* store encryptions in an array */
    char singleLine[1000];
    int index = 0;

    /* reinitialize the encrypt array as they are used for both encryption 
     * and decryption */
    for (int i = 0; i<26; i++){
            encryptArray[i][0] = 0;
            encryptArray[i][1] = 0;
    }

    while (fgets(singleLine,1000,ep)){ 
        
        /* encryption */    
        int sourceChar = (int)singleLine[0];
        /* decryption */
        if (encryptState == -1)
            sourceChar = (int)singleLine[2];
        int destChar = (int)singleLine[2] - (int)singleLine[0];
        
        /* if there are any characters outside a to z */
        if (sourceChar < 97 || sourceChar > 122){
            printEncryptionError();
            exit(1);
        }
        /* if one to one mapping is violated */
        if ((encryptArray[sourceChar-97][0] != 0 ) || encryptArray[sourceChar-97][1] != 0){
            printEncryptionError();
            exit(1);
        }
        if (index < 26){
             /* update the encryption/decryption array */
            encryptArray[sourceChar-97][0] = sourceChar;
            encryptArray[sourceChar-97][1] = destChar;
        }
        index++;
    }
    /* if there are less or more than 26 encryption pairs */
    if (index != 26){
        printEncryptionError();
        exit(1);
    }
    /* close the file pointers */
    fclose(ep);
}

void printEncryptionError(){
    printf("Encryption/Decryption file contains invalid characters.\n");
    printf("Encryption/Decryption file should have one-to-one mapping,\n");
    printf("between source character and destination characters.\n");
    printf("The source and destination characters should be between a and z.\n");
}
