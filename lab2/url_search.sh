#!/bin/bash
#
#
#Author: Pratap Luitel
#Date: 4/12/2015
#
#
#INPUT: a .txt file with urls, followed by words(to be searched) seperated by a space
#OUTPUT: list of occurrences of each input word in each of the urls
#
#
#This script finds the number of occurrences of each input word in each of the urls 
#present in the .txt file which is also passed as the first argument.
#The script assumes that the url's specified in the .txt file are valid and legal. 
#
#if input has  valid number of arguments
if [[ $# -gt 1 ]]; then
    inputArray=($@) 
    fileURL=$1

    #if file is of .txt extension, if it exists and is non empty
    if [[ $fileURL == *.txt ]] && [[ -f "${fileURL}" ]] && [[ -s "${fileURL}" ]]; then
        #echo "Valid input format"
        iter=1
        for arg in $@
        do
            #disregard the first input argument(as it contains urls)
            if [[ iter -gt 1 ]];then
            tempIndex=1;
                echo "-----------------------------------------------------"
                echo "Word: $arg"
                while read webURL
                do
                    #create an html
                    touch $tempIndex.html

                    #save the content from the website 
                    curl -L -f -s -o $tempIndex.html $webURL
                    wordCount=`grep -o $arg $tempIndex.html | wc -w`
                    rm $tempIndex.html
                    let tempIndex++

                    #print
                    echo "$webURL $wordCount"

                done <${inputArray[0]} #pass each web site url
            fi
            let iter++

        done
    #invalid input
    else
        echo "Invalid file input: either does not exist, is not readable, is empty or isnt .txt."
        exit 1   
    fi

else
    echo "Invalid input. Please enter a non empty, readable .txt file followed by words with a space."
fi
exit 0

