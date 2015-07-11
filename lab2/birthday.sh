#!/bin/bash
#
#Author: Pratap Luitel
#Date: 4/12/2015
#This script checks if two dates occurred on the same day of the week. 
#
#INPUT: two birthdays in mm/dd/yyyy format, seperated by a space 
#
#OUTPUT:yes, or no
#

#if there are more or less than two input arguments
if [ $# != 2 ];then
	echo -n "Enter two arguments in mm/dd/yyyy format, sperated by a space."
	exit 1
#check the validity of the input
else 
    
	dayFirst=`date -d $1 +%"A"`
    if [ $? -eq 1 ];then
        echo "invalid input for first argument." 
        echo "required format: mm/dd/yyyy."
        echo "mm should be in [1-12]"
        echo "dd should be in [1-31]"
        echo "yyyy should be in [0000-1999]"
        exit 1
    fi
  

	daySecond=`date -d $2 +%"A"`
    if [ $? -eq 1 ];then 
        echo "invalid input for second argument." 
        echo "required format: mm/dd/yyyy."
        echo "mm should be in [1-12]"
        echo "dd should be in [1-31]"
        echo "yyyy should be in [0001-1999]"
        exit 1
    fi

   #check if the days are the same
    if [ "$dayFirst" = "$daySecond" ]; then
        echo "The first person was born on: $dayFirst"
        echo "The second person was born on: $daySecond"
        echo "Jackpot! you were both born on the same day of the week!"
    else
        echo "The first person was born on: $dayFirst"
        echo "The second person was born on: $daySecond"
        echo "Therefore, you are not born on the same day of the week!"
    fi

fi
exit 0

