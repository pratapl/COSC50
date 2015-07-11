#!/bin/bash
#
#
#Author: Pratap Luitel
#Date: 4/12/2015
#
#INPUT: user names to be spied on each seperated by a space
#OUTPUT: log in details of the specified users
#
#------------------------------------------------------------
#
startTime=`date +%s`

loginTime=()
logoutTime=()
totalTime=()

#initialize the elements of the array to zero
iter=0;
for iter in $#
    do
        loginTime[iter]=0;
        logoutTime[iter]=0;
        totalTime[iter]=0;
        let iter++
    done

#echo "Login times at the beginning ${loginTime[*]}"
#echo "Logout times at the beginning ${logoutTime[*]}"
#echo "totalTime at the beginning ${totalTime[*]}"

#if there isnt any input argument
if [[ "$#" -eq 0 ]]; then
    echo "Invalid input. Please enter one or more users seperated by a space."
    exit 1
fi

#seperate the input arguments based on quotes
IFS=""

#make an array of userId, initiate associated .txt to keep track of log records
userNameArray=("$@")    
for user in "${userNameArray[@]}"
    do
        userId=`cat /etc/passwd| grep $user| awk -F ":" '{print $1}'|uniq`
        userIdArray+=($userId)
        touch $userId.txt
    done

#restore the internal field seperator
unset IFS 


function terminate
{
    endTime=`date -d "now"`
    touch spy.log
    echo "Spy.sh Report ">>spy.log
    echo "Started at `date -d "@$startTime"`">>spy.log
    echo "Stopped at $endTime">>spy.log
    echo "Arguments: "${userNameArray[@]}"">>spy.log
    
    i=0
    for userId in ${userIdArray[@]}
        do
            #if user is logged in
            if [[ `who | grep $userId` ]]&&[[ ${loginTime[$i]} != 0 ]];then
                
                logoutTime[$i]=`date +%s`
                totalTime[$i]=$(( ${totalTime[$i]} + ${logoutTime[$i]} -${loginTime[$i]} )) 
                echo "Logged on `date -d "@${loginTime[$i]}"`; Logged off `date -d "@${logoutTime[$i]}"`">>$userId.txt
              
            fi
        let i++
        done
    i=0
    for userId in ${userIdArray[@]}
        do
            mins=$(( ${totalTime[$i]}/60 ))
            echo "$userId logged on `cat $userId.txt | wc -l` times for a total period of $mins minutes for all sessions.Breakdown is as follows: ">>spy.log
            echo "`cat $userId.txt`">>spy.log
        let i++
        done

    exit 0

}


while [[ true ]]
do
    i=0
    #loop through each user
    for userId in ${userIdArray[@]}
        
        do
        #check if the user is logged in
        if [[ `who | grep $userId` ]];then
            
            #update login time if not recorded yet
            if [[ ${loginTime[$i]} -eq 0 ]];then
                loginTime[$i]=`date +%s`
                #echo "Login time set for $userId to ${loginTime[$i]}"
            fi

        #if the user isnt logged in
        else
            #if the user was logged in 
            if [[ ${loginTime[$i]} != 0 ]];then
                #record the logout time,update the total time and record the logout    
                logoutTime[$i]=`date +%s`
                totalTime[$i]=$(( ${totalTime[$i]} + ${logoutTime[$i]} -${loginTime[$i]} )) 
                echo "Logged on `date -d "@${loginTime[$i]}"`; Logged off `date -d "@${logoutTime[$i]}"`">>$userId.txt
                loginTime[$i]=0
                
            fi 
        fi
        let i++
        done

    sleep 2 
    trap "terminate" SIGINT
done
exit 0

