#!/bin/bash
#
# Author: Pratap Luitel
# CS50, Query
#
#Note: Output saved to a log file

filename=`date +"%a_%b_%d_%T_%Y"`
file_name="Testlog."$filename
echo "Filname: "$filename >> $file_name
echo "" >> $file_name

echo "Cleaning folder..."
echo "*****Building*****" >> $file_name
echo "Make Clean" >> $file_name
make clean  >> "$file_name"


echo "Compling Indexer..."
echo "Compile indexer" >> $file_name
make compileIndexer >> $file_name
echo "Compliing Query..."
echo "Compile Query" >>$file_name
make  >> "$file_name"

if [ $? -ne 0 ]; then
    echo "Build Failed."  >> "$file_name"
    exit 1
fi
echo "Status: Query Build Successful" >> $file_name


# start checking args
echo "" >> $file_name
echo "*****Argument Testing*****" >> $file_name

# argc = 1
echo "Testing Argument(argc = 1)"
echo "Input: ./query " >> $file_name
echo "Output," >> $file_name
./query >> "$file_name"
if [ $? -ne 1 ]; then
    echo "Argument test Fail"  >> "$file_name"
    exit 1
fi
echo "Status: Argument Test Successful" >> $file_name

# argc = 5
echo "Testing Argument (argc = 5)"
echo "" >> $file_name
echo "Input: ./query a b c d " >> $file_name
echo "Output," >> $file_name
./query a b c d>> "$file_name"
if [ $? -ne 1 ]; then
    echo "Argument test Fail"  >> "$file_name"
    exit 1
fi
echo "Status: Argument Test Successful" >> $file_name


#directory validity
echo "Checking invalid directory.."
echo "" >> $file_name
echo "*****Directory*****" >> $file_name
echo "Input: ./query data/indexer.dat directoryDNE " >> $file_name
echo "Output," >> $file_name
./query data/indexer.dat directoryDNE >> $file_name
if [ $? -ne 1 ]; then
    echo "Incorrect return"  >> $file_name
    exit 1
fi
echo "Status: Invalid directory Test successful" >> $file_name


#check for incorrect file names
echo "Checking invalid filename(for indexer.dat)..."
echo "" >> $file_name
echo "*****Filename*****">> $file_name

echo "Input: ./query data/fileDNE.dat data/htmlData" >> $file_name
echo "Output," >> $file_name
./query data/fileDNE.dat data/htmlData>> $file_name
if [ $? -ne 1 ]; then
    echo "Incorrect return"  >> $file_name
    exit 1
fi
echo "Status: Invalid File Test Successful" >> $file_name
echo "" >> $file_name

#correct argument testing
echo "Correct call(input word, press enter, press Ctrl+d"
echo "" >> $file_name
echo "*****Normal Mode*****">> $file_name

echo "Input: ./query data/indexer.dat data/htmlData" >> $file_name
echo "Output," >> $file_name
./query data/indexer.dat data/htmlData>> $file_name
echo "Valid query call successful testing" >> $file_name
echo "" >> $file_name


echo "" >> $file_name
make clean  >> "$file_name"

echo "*****Completing Testing*****" >> $file_name
echo "" >> $file_name
finishingTouch=`date +"%a_%b_%d_%T_%Y"`
echo "Testing Complete Successfully." >> $file_name
echo "Time: "$finishingTouch>> $file_name
echo "" >> $file_name

