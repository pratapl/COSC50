Pratap Luitel
CS 50, Project 3
Readme.txt
****************************************************
Compiling Instruction:
mygcc -o filename dog.c

Arguments:
./dog [-n][-m encryptFile] [-M decryptFile][filenames] 


Assumptions:
1)Encryption or decryption files must have 26 lines with
one to one mapping between source and destination characters. 

2)If encryption and decryption switches are both on, they 
cancell each other and encryption or decryption is not executed. 

3)Each line of encryption file stores the source character in
the first column and the destination character in the third 
column with a space in between, and vice versa for decryption. 

