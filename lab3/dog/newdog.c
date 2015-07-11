/*
 * Pratap Luitel
 * 4/20/15
 *
 * dog allows various flags and stuff
 *
 *
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define LIMIT 1000


extern char *optarg;//stores input file name
extern int optind;//
int c,err=0;
int nFlag,tFlag, encryptFlag, decryptFlag;
char *encryptFile=NULL, *decryptFile = NULL;
int printToFile = 0;


int getopt(int argc, char *const argv[], const char *optstring);
void printHelp();
void printVersion();
void echoInput(int nFlag);
void echoFiles(int nFlag, int encryptFlag,char *encryptFile,int decryptFlag,char *decryptFile);

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        echoInput(nFlag);
        exit(0);
    }
    if (argc == 2)
    {
        int wordLength = strlen(argv[1]);
        char inputWord[wordLength + 1];
        inputWord[0]='\0';
        strcat(inputWord,argv[1]); //inputWord now ends with '\0'

        if (strcmp(inputWord,"--help") == 0){
            printHelp();
            exit(0);
        }
        if (strcmp(inputWord,"--version") == 0){
            printVersion();
            exit(0);
        }
        if (strcmp(inputWord,"-") == 0){
            echoInput(nFlag);
            exit(0);
        }
     }

    while ((c=getopt(argc,argv,"ntM:m:")) != -1)
        switch(c)
        {
            case 'n':
                nFlag += 1;
                break;
            case 't':
                tFlag +=1;
                break;
            case 'm':
                encryptFlag += 1;
                encryptFile = optarg;
                break;
            case 'M':
                decryptFlag += 1;
                decryptFile = optarg;
                break;
            case '?':
                err = 1;
                break;
            case '>':
                printToFile += 1;
                break;
            default:
                break;
        }
        
        if (nFlag > 1 || encryptFlag > 1 || decryptFlag > 1)
        {
            fprintf(stderr, "Error! One or mor swichtes was set multiple times. \n");
            exit(1);
        }
        else if(err)
        {
            fprintf(stderr,"Invalid input.\n");
            fprintf(stderr,"Use -n, -m, -M for switches.\n");
            exit(1);
        }
        else
        {
            for (int i = optind; i < argc; i++)
            {
                echoFiles(nFlag,encryptFlag,encryptFile,decryptFlag,decryptFile);
                printf("Optional argument: %s\n",argv[i]);
            }
            printf("done\n");
        }
            
            
return 0;
}

void printHelp()
{
    printf("Command: dog\n");
    printf("Usage: dog [-n] [-m filename] [-M filename] [filename]\n");
    printf("switch: --version for version info\n");
    printf("switch: --help for this option\n");
}

void printVersion()
{
    printf("Version: 0.1\n");
    printf("Author: Pratap Luitel\n");
    printf("CS50, Dartmouth College\n");
}

void echoInput(int nFlag)
{
    int lineNum = 1;
    char inputStr[LIMIT];
    while((fgets(inputStr,LIMIT,stdin))){
        if(strcmp(inputStr,"") == 0){
            break;}
        if (nFlag){
            printf("\t%d %s",lineNum,inputStr);
            lineNum++;
        }
        else{
            printf("\t %s",inputStr);
        }
    }
}

void echoFiles(int nFlag, int encryptFlag, char *encryptFile,int decryptFlag, char *decryptFile)
{
    //do sth
}
