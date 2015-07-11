#!/bin/bash
#
# Author: Team Cabernet
# Date:6/1/2015
# Project: Amazing Project
#
#Note: Output saved to a log file

filename=`date +"%a_%b_%d_%T_%Y"`
file_name="Testlog."$filename

echo "Author: Team Cabernet" >> $file_name
echo "Date: 6/1/2015" >> $file_name
echo "Filname: "$filename >> $file_name
echo "This is a log file of the results of various test cases" >> $file_name
echo "" >> $file_name

echo "***********************************************************" >> $file_name
echo "" >> $file_name
echo "Cleaning folder..."
echo "*****Cleaning*****" >> $file_name

make clean >> $file_name
if [ $? -ne 0 ]; then
    echo "Clean Failed."  >> "$file_name"
    exit 1
fi
echo "Status: Clean Successful" >> $file_name
echo "" >> $file_name
echo "***********************************************************" >> $file_name
echo "" >> $file_name


echo "Building AMStartup and Avatar.."
echo "*****Building*****" >> $file_name
make >> "$file_name"
if [ $? -ne 0 ]; then
    echo "Build Failed."  >> "$file_name"
    exit 1
fi
echo "Status: Build Successful" >> $file_name
echo "" >> $file_name
echo "***********************************************************" >> $file_name
echo "" >> $file_name


# start checking args
echo "*****Argument Testing*****" >> $file_name
echo "" >> $file_name


# argc = 3
echo "Checking for arguments(argc = 3)"
echo "Input: ./AMStartup -n 2 -d 5" >> $file_name
echo "Output," >> $file_name
./AMStartup -n 2 -d 5 >> $file_name 2>&1
if [ $? -ne 1 ]; then
    echo "Argument test Fail"  >> "$file_name"
    exit 1
fi
echo "Status: Argument Test Successful" >> $file_name

# argc = 5
echo "Checking for arguments(argc = 5)"
echo "" >> $file_name
echo "Input: ./AMStartup -n 2 -d 5 -d 10 -h stowe.cs.dartmouth.edu" >> $file_name
echo "Output," >> $file_name
./AMStartup -n 2 -d 5 -d 10 -h stowe.cs.dartmouth.edu >> $file_name 2>&1
if [ $? -ne 1 ]; then
    echo "Argument test Fail"  >> "$file_name"
    exit 1
fi
echo "Status: Argument Test Successful" >> $file_name
echo "" >> $file_name
echo "***********************************************************" >> $file_name

#nAvatar = 20
echo "" >> $file_name
echo "*****nAvatar*****" >> $file_name
echo "" >> $file_name
#nAvatar = 20
echo "Checking for invalid avatar numbers(n = 20)"
echo "Input: ./AMStartup -n 20 -d 5 -h stowe.cs.dartmouth.edu" >> $file_name
echo "Output," >> $file_name
./AMStartup -n 20 -d 5 -h stowe.cs.dartmouth.edu >> $file_name 2>&1
if [ $? -ne 1 ]; then
    echo "nAvatar test failed"  >> $file_name
    exit 1
fi
echo "Status: nAvatar Test Successful" >> $file_name
echo "" >> $file_name

#nAvatar = -20
echo "Checking for invalid avatar numbers(n = -20)"
echo "Input: ./AMStartup -n -20 -d 5 -h stowe.cs.dartmouth.edu" >> $file_name
echo "Output," >> $file_name
./AMStartup -n -20 -d 5 -h stowe.cs.dartmouth.edu >> $file_name 2>&1
if [ $? -ne 1 ]; then
    echo "nAvatar test failed"  >> $file_name
    exit 1
fi
echo "Status: nAvatar Test Successful" >> $file_name
echo "" >> $file_name

#nAvatar = ab(not a number)
echo "Checking for invalid avatar numbers(n = -ab)"
echo "Input: ./AMStartup -n ab -d 5 -h stowe.cs.dartmouth.edu" >> $file_name
echo "Output," >> $file_name
./AMStartup -n ab -d 5 -h stowe.cs.dartmouth.edu >> $file_name 2>&1
if [ $? -ne 1 ]; then
    echo "nAvatar test failed"  >> $file_name
    exit 1
fi
echo "Status: nAvatar Test Successful" >> $file_name
echo "" >> $file_name
echo "***********************************************************" >> $file_name
echo "" >> $file_name


#difficulty level= 50
echo "*****difficulty*****" >> $file_name
echo "" >> $file_name
echo "Checking for invalid difficulty level(d = 50)"
echo "Input: ./AMStartup -n 5 -d 500 -h stowe.cs.dartmouth.edu" >> $file_name
echo "Output," >> $file_name
./AMStartup -n 5 -d 50 -h stowe.cs.dartmouth.edu >> $file_name 2>&1
if [ $? -ne 1 ]; then
    echo "difficulty test failed"  >> $file_name
    exit 1
fi
echo "Status: difficulty Test Successful" >> $file_name
echo "" >> $file_name

#difficulty level= -50
echo "Checking for invalid difficulty level(d = -50)"
echo "Input: ./AMStartup -n 5 -d -50 -h stowe.cs.dartmouth.edu" >> $file_name
echo "Output," >> $file_name
./AMStartup -n 5 -d -50 -h stowe.cs.dartmouth.edu >> $file_name 2>&1
if [ $? -ne 1 ]; then
    echo "difficulty test failed"  >> $file_name
    exit 1
fi
echo "Status: difficulty Test Successful" >> $file_name
echo "" >> $file_name



#difficulty level= abc
echo "Checking for invalid difficulty level(d = abc)"
echo "Input: ./AMStartup -n 5 -d abc -h stowe.cs.dartmouth.edu" >> $file_name
echo "Output," >> $file_name
echo "" >> $file_name
./AMStartup -n 5 -d abc -h stowe.cs.dartmouth.edu >> $file_name 2>&1
if [ $? -ne 1 ]; then
    echo "difficulty test failed"  >> $file_name
    exit 1
fi
echo "Status: difficulty Test Successful" >> $file_name
echo "" >> $file_name


echo "***********************************************************" >> $file_name
echo "" >> $file_name

#valid cases

#n= 2, d= 0
echo "*****Valid Case Testing*****" >> $file_name
echo "" >> $file_name
echo "Checking for n = 2 d = 0 server = stowe.cs.dartmouth.edu)"
echo "#####################################" >>$file_name
echo "Input: ./AMStartup -n 2 -d 0 -h stowe.cs.dartmouth.edu" >> $file_name
echo "Output," >> $file_name
echo "" >> $file_name
./AMStartup -n 2 -d 0 -h stowe.cs.dartmouth.edu >> $file_name 2>&1

# n = 3, d = 3 
echo "" >> $file_name
echo "#####################################" >>$file_name
echo "" >> $file_name
echo "Checking for n = 3 d = 3 server = stowe.cs.dartmouth.edu)"
echo "#####################################" >>$file_name
echo "Input: ./AMStartup -n 3 -d 3 -h stowe.cs.dartmouth.edu" >> $file_name
echo "Output," >> $file_name
echo "" >> $file_name
./AMStartup -n 3 -d 3 -h stowe.cs.dartmouth.edu >> $file_name 2>&1

#n = 6, d= 6
echo "" >> $file_name
echo "#####################################" >>$file_name
echo "" >> $file_name
echo "#####################################" >>$file_name
echo "Checking for n = 6 d = 6 server = stowe.cs.dartmouth.edu)"
echo "Input: ./AMStartup -n 6 -d 6 -h stowe.cs.dartmouth.edu" >> $file_name
echo "Output," >> $file_name
echo "" >> $file_name
./AMStartup -n 6 -d 6 -h stowe.cs.dartmouth.edu >> $file_name 2>&1

# n = 10, d = 9
echo "" >> $file_name
echo "#####################################" >>$file_name
echo "" >> $file_name
echo "#####################################" >>$file_name
echo "Checking for n = 10 d = 9 server = stowe.cs.dartmouth.edu)"
echo "Input: ./AMStartup -n 10 -d 9 -h stowe.cs.dartmouth.edu" >> $file_name
echo "Output," >> $file_name
echo "" >> $file_name
./AMStartup -n 10 -d 9 -h stowe.cs.dartmouth.edu >> $file_name 2>&1

echo "Status: Valid Test Successful" >> $file_name
echo "" >> $file_name
echo "#####################################" >> $file_name
make clean  >> "$file_name"

echo "*****Completing Testing*****" >> $file_name
echo "" >> $file_name
finishingTouch=`date +"%a_%b_%d_%T_%Y"`
echo "Testing Complete Successfully." >> $file_name
echo "Time: "$finishingTouch>> $file_name
echo "" >> $file_name

