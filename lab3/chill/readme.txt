Author: Pratap Luitel
Program name: chill.c
4/23/15

chill.c should be compiled using the following gcc switches:
gcc -Wall -pedantic -std=c11 -ggdb -o compiledName chill.c 

When running the complied file, user can input upto 2 arguments. 
The user has the option of passing in 0,1 or 2 arguments. The input 
is assumed to be of type double although ints are also accepted. 
If the input argument is outside the accepted range of -50 to 400 for temperature
and 0.5 to 400 for wind velocity, users are displayed a message. 

Exception: Temperature of 0 is not acceptable because of the way the program works. 
The program uses the function atof which converts non numeric characters to 0.




