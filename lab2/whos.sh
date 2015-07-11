#!/bin/bash

            
#              (e.g., cs50) and the fifth field contains the users full
#              name (e.g., Peter C. Johnson). You can provide a username or full name and
#              whos will return you the full name or username, respectively. This is
#              similar in spirit to the finger utility (check it out).
#
# Input:       List of arguments can be usernames or login names.





if [ $# -eq 0 ]
      then
           echo "Usage: whos arg needs to be [list of username or full name]" 1>&2
           exit 1
fi

for arg
do
        awk -F: ‘{print $1, $5}‘ /etc/passwd | grep -i "$arg"

done

exit 0
