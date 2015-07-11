#!/bin/bash
#
#
#Author: Pratap Luitel
#Date: 4/12/2015
#
#
#INPUT:no arguments required
#OUTPUT: list of files extension with their occurence,
#       files without extension or ending in a "." are grouped as
#       noext files. 
#
#
#This script displays the number of types of file in a folder.
#Files without a valid extension (those ending in a "." or those
#without an extension at all are grouped as noext files. 
#
#

noExtString="noext"
#loop through each regular file found by the find command
while read sampleFile; do
   
    #extract .extension for files having an extension or ending with a "." 
    actualFile=`echo $sampleFile| sed -e 's|.*\./||' | sed -e 's|.*\.|.|'`
    
    #extract the full filenames for files without extension
    actualFile=`echo $actualFile| sed -e 's|.*/||'`

    #echo -e "preprocess file: $sampleFile"
    #echo -e "processed file: $actualFile"

    #if the file has extension
    if [[ $actualFile == *\.* ]];then
      
        lastLetter=`echo $actualFile | grep -o \.$`
          
        #if it is not a proper extension, i.e ends in a '.'
        if [[ $lastLetter == "." ]];then
            extensionArray+=($noExtString)
        else
            extension=`echo $actualFile | sed 's|\.||'`
            extensionArray+=($extension)
        fi
    else
        extensionArray+=($noExtString)
    fi

    #if the file does not have extension
done < <(find . -type f)

#sort and print the extensions with occurence numbers
echo -e "List of file extension and their occurences."
echo -e "Files with an invalid extension are grouped as noext."
echo -e "-------------------------------------------------------"
printf "%s\n" "${extensionArray[@]}" | sort | uniq -c
exit 0

