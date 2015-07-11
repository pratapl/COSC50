#!/bin/bash
#
# Author: Pratap Luitel
# CS50, Indexer 
#
#Note: Output saved to a log file

filename=`date +"%a_%b_%d_%T_%Y"`
file_name="Testlog."$filename
echo "Filname: "$filename >> $file_name
echo "" >> $file_name


echo "*****Building*****" >> $file_name
make clean  >> "$file_name"
make  >> "$file_name"

if [ $? -ne 0 ]; then
    echo "Build Failed."  >> "$file_name"
    exit 1
fi
echo "Status: Build Successful" >> $file_name

# start checking args
echo "" >> $file_name
echo "*****Argument Testing*****" >> $file_name

# argc = 1
echo "Input: ./indexer ?" >> $file_name
echo "Output," >> $file_name
./indexer ?>> "$file_name"
if [ $? -ne 1 ]; then
    echo "Argument test Fail"  >> "$file_name"
    exit 1
fi
echo "Status: Test Successful" >> $file_name

# argc = 7
echo "" >> $file_name
echo "Input: ./indexer data a b c d e" >> $file_name
echo "Output," >> $file_name
./indexer data a b c d e>> "$file_name"
if [ $? -ne 1 ]; then
    echo "Argument test Fail"  >> "$file_name"
    exit 1
fi
echo "Status: Test Successful" >> $file_name


#directory validity
echo "" >> $file_name
echo "*****Directory*****" >> $file_name
echo "Input: ./indexer directoryDNE index.dat " >> $file_name
echo "Output," >> $file_name
./indexer directoryDNE index.dat >> $file_name
if [ $? -ne 1 ]; then
    echo "Incorrect return"  >> $file_name
    exit 1
fi
echo "Status: Test successful" >> $file_name

#check for incorrect file names
echo "" >> $file_name
echo "*****Filename*****">> $file_name

echo "./indexer /net/class/cs50/public_html/tse/wiki index.dat" >> $file_name
echo "Output," >> $file_name
./indexer /net/class/cs50/public_html/tse/wiki index.dat &>> $file_name
if [ $? -ne 1 ]; then
    echo "Incorrect return"  >> $file_name
    exit 1
fi
echo "Status: Test Successful" >> $file_name
echo "" >> $file_name

# check for testing arguments
echo "" >> $file_name
echo "*****Argument Test*****" >> $file_name
echo "" >> $file_name
echo "+ Filnames do not match" >> $file_name
echo "Input: " >> $file_name

echo "./indexer data index.dat indeeex.dat final.dat" >> $file_name
echo "Output," >> $file_name
./indexer data index.dat indeeex.dat final.dat &>> $file_name

if [ $? -ne 1 ]; then
    echo "Incorrect return"  >> $file_name
    exit 1
fi
echo "Status: Test Successful" >> $file_name
echo "" >> $file_name


# right argument
echo "*****Regular Mode*****" >> $file_name
echo "" >> $file_name
echo "+ Depth 0" >> $file_name
echo "Input: ./index data0 index.dat" >> $file_name
echo "Output," >> $file_name
./indexer data0 index.dat >> $file_name
if [ $? -eq 0 ]; then
    echo "Status: Test Successful"  >> $file_name
fi
echo "" >> $file_name
make clean  >> "$file_name"
rm *.dat
make  >> "$file_name"

echo "" >> $file_name
echo "+ Depth 1" >> $file_name
echo "Input: ./indexer data1 index.dat" >> $file_name
./indexer data1 index.data>> $file_name
if [ $? -eq 0 ]; then
    echo "Status: Test Successful"  >> $file_name
fi
echo "" >> $file_name
make clean  >> "$file_name"
rm *.dat
make  >> "$file_name"

echo "" >> $file_name
echo "+Depth 2" >> $file_name
./indexer /net/class/cs50/public_html/tse/crawler/lvl2 index.dat &>> $file_name
if [ $? -eq 0 ]; then
    echo "Status: Test Successful"  >> $file_name
fi
echo "" >> $file_name
make clean  >> "$file_name"
rm *.dat
make  >> "$file_name"

echo "Completing Testing..." >> $file_name
echo "" >> $file_name

finishingTouch=`date +"%a_%b_%d_%T_%Y"`

echo "Testing Complete Successfully." >> $file_name
echo "Time: "$finishingTouch>> $file_name
echo "" >> $file_name
