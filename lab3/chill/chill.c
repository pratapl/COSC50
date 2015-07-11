/*
   Pratap Luitel
   4/19/15
   CS50, Lab 3, Dartmouth College
    
   This program  uses temperature and velocity to calculate windchill.
   Temperature and velocity are optional input parameters.The output will 
   be displayed in a tabulated format.  
   
   Input:
   T:a double  representing the temperature.
     this is an optional input which by default will be in the range of 
     -10 to 40 at intervals of 10 if not entered.    
   V: a double  representing the velocity.
      this is an optional input. values of 5,10 and 15 will be used as default. 
   
   Output:
   A tabulated display of temperature, velocity and corresponding windchill.  
*/    

#include<stdio.h>
#include<math.h>
#include<stdlib.h>

double  maxTemp = 50;  /* max accepted temp */
double  minTemp = -400;/* min accepted temp */
double  maxWind = 400; /* max accepted wind velocity */
double  minWind = 0.5; /*min accepted wind velocity */

int defaultT[6]={-10,0,10,20,30,40};/*default values for temperature*/
int defaultV[3]={5,10,15};/*default values for wind velocity */
int lengthT=sizeof(defaultT)/sizeof(defaultT[0]); 
int lengthV=sizeof(defaultV)/sizeof(defaultV[0]);

/* functions declaration */
void printHead();
double getChill(double t,double v);
int isInRange(int index, double c);
void printError(int index);

int main(int argc, char *argv[])
{   
    /*use defautl T and V values if user does not input any*/
    if(argc == 1){
        printHead();
        for (int i = 0; i < lengthT; i++){
            if(i > 0)
                printf("\n");/*print a blank line between new temperature values*/

            for (int j=0; j<lengthV; j++){
                double temp=defaultT[i];
                double vel=defaultV[j];
                printf("%4.01f%7.01f%8.01f \n",temp,vel,getChill(temp,vel));  
            } 
        }    
    }
    /*if the user inputs one value, assume it to be temperature*/ 
    else if(argc == 2){  
        double temp = atof(argv[argc-1]);
        if( temp != 0 && isInRange(1,temp)){/*check if temperature is in acceptable range*/
            printHead();
            for (int j = 0; j < lengthV; j++){
                double vel=defaultV[j]; 
                printf("%4.01f%7.01f%8.01f\n",temp,vel,getChill(temp,vel));    
            } 
        }
        else{
            printError(1); /* argument 1 for invalid temperature range */
        }
    }
    /*if user inputs two values*/
    else if(argc == 3){
        double temp = atof(argv[argc-2]);
        double vel = atof(argv[argc-1]);
        
        if(temp != 0 && vel != 0 && isInRange(1,temp) && isInRange(2,vel)){
            printHead();  
            printf("%4.01f%7.01f%8.01f\n",temp,vel,getChill(temp,vel));    
        }
        else{
            printError(2);/* argument 2 for invalid temperature and/or velocity */
        }     
    }
    else{
        printError(2);    
    }
    
return 0;
}

/*This function calculates and returns the chill.
 * t: double value for temperature
 * v: double value for velcoity
 *
 * getChill: double, calculated windchill value*/
double getChill(double t,double v)
{
    double chillVal = 35.74+(0.6215 * t) - (35.75 *(pow(v,0.16))) + (0.4275 * t * (pow(v,0.16)));
    return chillVal;
}

/* prints the first two lines,the header */
void printHead()
{
    printf("Temp   Wind   Chill\n");
    printf("----   ----   -----\n");
}
 
/*This function checks if the input double value is within a range.
 * index: int, either 1 or 2. 
 *      1 checks for temperature range.  
 *      2 checks for velocity range.
 *
 * c: double, the value to be checked
 *
 * Return: 1 if is in range, else 0*/
int isInRange(int index,double c)
{
    if (index == 1 && c > minTemp && c < maxTemp ){
        return 1;
    }
    else if(index ==2 && c > minWind && c < maxWind ){
        return 1;
    }
    else{
        return 0;
    }
}

/*This function prints error message. 
 index: int, 1 or anything else
        1 displays error message related to the first arg
        anything else displays error related to both args*/
void printError(int index)
{
   if (index == 1)
   {
        printf("Please enter two arguments.\n"); 
        printf("First Argument should be in range %f to %f .\n",minTemp,maxTemp);
 
   }
   else
   {
        printf("Please enter two arguments.\n"); 
        printf("First Argument should be in range %f to %f .\n",minTemp,maxTemp);
        printf("Second Argument should be in range %f to %f.\n",minWind,maxWind);
   }
}
