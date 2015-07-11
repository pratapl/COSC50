#!/bin/bash
#
# File: test_and_start_crawl.sh
#
# Description: This bash test script runs a number of tests against
# crawler. First, we do a clean build as a sanity check. Next,
# we check all input arguments checking for defensive coding.
# Third, we run the crawler at depth 1 and 2. We write all output
# to a log file.
#

author="Pratap Luitel"
filename=`date`
filename="crawlerTest.log"
echo "Author:"$author 
echo "The results of testing is saved in: $filename"
echo "After executing this file, it will terminate automatically"

#empty file contents
echo >"$filename"
echo "Author: $author" >> "$filename"
echo "Log created on:" `date` >> "$filename"
echo "" >> "$filename"
echo "This file is a log of various tests, implemented in crawlerTest.sh" >> "$filename"


#building
echo "*****Cleaning*****" >> "$filename" 
make clean >> "$filename"
echo "" >> "$filename"

echo "*****Building*****" >> "$filename"
make crawler  >> "$filename"
if [ $? -ne 0 ]; then
    echo "Sorry, the build failed" >> "$filename" 
    exit 1
fi
echo "" >> "$filename"

# argument testing
echo "" >> "$filename"
echo "*****Checking number of arguments****" >> "$filename" 
echo "Input: ./crawler http://www.dartmouth.edu directory" >> "$filename"
echo "Output is listed below, " >> "$filename"
./crawler http://www.dartmouth.edu directory >> "$filename" 
if [ $? -ne 1 ]; then
    echo "No. of arguments test: Crawler could not be executed" >> "$filename"  
    exit 1
fi

echo "" >> "$filename"
echo "*****Checking for directory*****" >> "$filename"
echo "Input: ./crawler http://old-www.cs.dartmouth.edu/~cs50/tse folderDoesnotExist 2" >> "$filename"
echo "Output is listed below, " >> "$filename"
./crawler http://old-www.cs.dartmouth.edu/~cs50/tse folderDoesNotExit 2 >> "$filename"
if [ $? -ne 1 ]; then
    echo "Directory test: Crawler could not be executed" >> "$filename"
    exit 1
fi


echo "" >> "$filename"
echo "*****Checking for depth validity*****" >> "$filename"
echo "Input: ./crawler http://old-www.cs.dartmout.edu/~cs50/tse data -2" >> "$filename"
echo "Output is listed below, " >> "$filename"
./crawler http://old-www.cs.dartmouth.edu/~cs50/tse data -2 >> "$filename"
if [ $? -ne 1 ]; then
    echo "Depth test: crawler could not be executed" >> "$filename" 
    exit 1
fi


echo "" >> "$filename"
echo "*****Run crawler for valid arguments- seedUrl data 1*****" >> "$filename"

./crawler http://old-www.cs.dartmouth.edu/~cs50/tse data 1 >> "$filename"

echo "" >> "$filename"
echo "*****Cleaning*****" >> "$filename"
make clean >> "$filename"

