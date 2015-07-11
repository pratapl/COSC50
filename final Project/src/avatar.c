/* ========================================================================== */
/* File: avatar.c
 *
 * Author: Cabernet
 * Date: 
 *
 * Input: 
 *
 * Command line options: None
 *
 * Output: 
 *
 * Error Conditions:
 *
 * Special Considerations:
 *
 */
/* ========================================================================== */
// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>	   
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> 
#include <string.h>
#include <getopt.h>	  // Getopt
#include <sys/socket.h>     // Socket function
#include <sys/types.h>      // Sockets
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h> 
#include <math.h> 
#include <ncurses.h>


// ---------------- Local includes  e.g., "file.h"
#include "avatar.h"
#include "AMStartup.h"
//#include "algorithm.h"

//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 


int getPrevDir(XYPos* prevPos, int currX, int currY) {
        // West
    int prevX = prevPos->x;
    int prevY = prevPos->y;
        if (currX < prevX && currY == prevY) {
            return M_WEST;
        }
        // North
        if (currY < prevY && currX == prevX) {
            return M_NORTH;
        }
        // South
        if (currY > prevY && currX == prevX) {
            return M_SOUTH;
        }
        // East
        if (currX > prevX && currY == prevY) {
            return M_EAST;
        }
        return M_NULL_MOVE;
}

int findNextDir(XYPos* prevPos, int currX, int currY, AvatarStruct* avatar, int zeroX, int zeroY){
    int newX = currX;
    int newY = currY;
    int oldX = prevPos->x;
    int oldY = prevPos->y;
    // if(avatar->id == 0)
    //     return M_NULL_MOVE;
    if(newX == zeroX && newY == zeroY){
        return M_NULL_MOVE;
    }
    //printf("%d, %d, %d, %d\n", newX, newY, oldX, oldY);
    int prevDir = avatar->dir;
    //printf("%d\n", prevDir);
    if(avatar->moved == 0){
        avatar->moved = 1;
        learn_maze[oldX][oldY]->occupied = 1;
        //printf("in findNext1 \n");
        return prevDir;
    }
    if (oldX != newX || oldY != newY) {         //Check if moved, if yes, turn left
        prevDir = getPrevDir(prevPos, newX, newY);
        learn_maze[oldX][oldY]->occupied = 0;
        learn_maze[newX][newY]->occupied = 1;
        // if(learn_maze[oldX][oldY]->lastAvatar >= avatar->id)
        //     learn_maze[oldX][oldY]->lastAvatar = avatar->id;
        learn_maze[oldX][oldY]->hasVisited[avatar->id] = 1;
        if(learn_maze[newX][newY]->hasVisited[avatar->id] == 1){
            switch (prevDir){
                case M_NORTH:
                    //learn_maze[oldX][oldY]->northBlocked = 1;
                    learn_maze[newX][newY]->southBlocked = 1;
                    break;
                case M_EAST:
                    //learn_maze[oldX][oldY]->eastBlocked = 1;
                    learn_maze[newX][newY]->westBlocked = 1;
                    break;
                case M_SOUTH:
                    //learn_maze[oldX][oldY]->southBlocked = 1;
                    learn_maze[newX][newY]->northBlocked = 1;
                    break;
                case M_WEST:
                    //learn_maze[oldX][oldY]->westBlocked = 1;
                    learn_maze[newX][newY]->eastBlocked = 1;
                    break;
            }
        }
        switch (prevDir) {
            case M_NORTH:
                if(learn_maze[newX][newY]->westBlocked != 1){
                    prevDir = M_WEST;
                    learn_maze[oldX][oldY]->lastDir = prevDir;
                    break;
                }
                else if(learn_maze[newX][newY]->northBlocked != 1){
                    prevDir = M_NORTH;
                    break;
                }
                else if(learn_maze[newX][newY]->eastBlocked != 1){
                    prevDir = M_EAST;
                    break;
                }
                else{
                    prevDir = M_SOUTH;
                    break;
                }
            case M_WEST:
                if(learn_maze[newX][newY]->southBlocked != 1){
                    prevDir = M_SOUTH;
                    learn_maze[oldX][oldY]->lastDir = prevDir;
                    break;
                }
                else if(learn_maze[newX][newY]->westBlocked != 1){
                    //printf("WALL AT SOUTH!!!!!!!!!!!!\n");
                    prevDir = M_WEST;
                    break;
                }
                else if(learn_maze[newX][newY]->northBlocked != 1){
                    prevDir = M_NORTH;
                    break;
                }
                else{
                    prevDir = M_EAST;
                    break;
                }
            case M_SOUTH:
                if(learn_maze[newX][newY]->eastBlocked != 1){
                    prevDir = M_EAST;
                    learn_maze[oldX][oldY]->lastDir = prevDir;
                    break;
                }
                else if(learn_maze[newX][newY]->southBlocked != 1){
                    prevDir = M_SOUTH;
                    learn_maze[oldX][oldY]->lastDir = prevDir;
                    break;
                }
                else if(learn_maze[newX][newY]->westBlocked != 1){
                    //printf("WALL AT SOUTH!!!!!!!!!!!!\n");
                    prevDir = M_WEST;
                    break;
                }
                else{
                    prevDir = M_NORTH;
                    break;
                }                                
            case M_EAST:
                if(learn_maze[newX][newY]->northBlocked != 1){
                    prevDir = M_NORTH;
                    learn_maze[oldX][oldY]->lastDir = prevDir;
                    break;
                }
                else if(learn_maze[newX][newY]->eastBlocked != 1){
                    prevDir = M_EAST;
                    learn_maze[oldX][oldY]->lastDir = prevDir;
                    break;
                }
                else if(learn_maze[newX][newY]->southBlocked != 1){
                    prevDir = M_SOUTH;
                    learn_maze[oldX][oldY]->lastDir = prevDir;
                    break;
                }
                else{
                    prevDir = M_WEST;
                    break;
                }

        }
    }
    else { //else, avatar ran into a wall and did not move. Update the wall location. Turn right
        //printf("in here\n");
        switch (prevDir) {
            case M_NORTH:
                //printf("MADE north wass at %d, %d \n", oldX, oldY);
                learn_maze[oldX][oldY]->northBlocked = 1;
                learn_maze[oldX][oldY]->gNorth = 1;
                if(oldY != 0)
                    learn_maze[oldX][oldY-1]->southBlocked = 1;
                prevDir = M_EAST;
                break;
            case M_EAST:
                //printf("MADE east wass at %d, %d \n", oldX, oldY);
                learn_maze[oldX][oldY]->eastBlocked = 1;
                learn_maze[oldX][oldY]->gEast = 1;
                if(oldX < learn_maze[oldX][oldY]->mazeWidth-1)
                    learn_maze[oldX+1][oldY]->westBlocked = 1;
                prevDir = M_SOUTH;
                break;
            case M_SOUTH:
                //printf("MADE south wass at %d, %d \n", oldX, oldY);
                learn_maze[oldX][oldY]->southBlocked = 1;
                learn_maze[oldX][oldY]->gSouth = 1;
                if(oldY < learn_maze[oldX][oldY]->mazeHeight-1)
                    learn_maze[oldX][oldY+1]->northBlocked = 1;
                prevDir = M_WEST;
                break;
            case M_WEST:
                //printf("MADE west wass at %d, %d \n", oldX, oldY);
                learn_maze[oldX][oldY]->westBlocked = 1;
                learn_maze[oldX][oldY]->gWest = 1;
                if(oldX != 0)
                    learn_maze[oldX-1][oldY]->eastBlocked = 1;
                prevDir = M_NORTH;
                break;
        }
    }
    //printf("after check dir %d\n",prevDir);
    avatar->dir = prevDir;
    //printf("AVATAR: %d, PREVDIR = %d\n", avatar->id, prevDir);
    return prevDir;
}


void *avatarFunction(void *thread_input)
{
	AvatarStruct *passed_avatar = (AvatarStruct *)thread_input;

	uint32_t passed_id = passed_avatar->id;
	//int passed_total_num_avatars = passed_avatar->total_num_avatars;
	//int passed_difficulty = passed_avatar->difficulty;
	uint32_t passed_maze_port = passed_avatar->maze_port;

	struct sockaddr_in passed_socket;
	passed_socket.sin_family = AF_INET;
	passed_socket.sin_addr = *passed_avatar->address;
	passed_socket.sin_port = htons(passed_maze_port); 

	FILE *passed_log = passed_avatar->file;
	//char *passed_logfile_name = passed_avatar->logfile_name;

	if (passed_log == NULL)
	{
		printf("error: logfile error\n");
		exit(1);
	}
	//write to logfile
	fprintf(passed_log, "Avatar %d ready to move!\n", passed_id);

	//print to stdout
	fprintf(stdout, "Avatar %d ready to move!\n", passed_id);


	//////////////////////////////////// BEGIN SOCKET STUFF //////////////////////////////////// 

	AM_Message avatar_message;
	avatar_message.type = htonl(AM_AVATAR_READY);
	avatar_message.avatar_ready.AvatarId = htonl(passed_id);

	int sockfd;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		fprintf(stdout, "error: couldn't connect avatar to socket");
		exit(1);
	}
	if (connect(sockfd, (struct sockaddr *) &passed_socket, sizeof(passed_socket)) < 0)
	{
        fprintf(stdout, "error: avatar could not connect to server\n");
        exit(1);
    }
    if (send(sockfd, &avatar_message, sizeof(avatar_message), 0) == -1)
    {
        fprintf(stdout, "error: avatar could not send ready message\n");
        exit(1);
    }
    sleep(1);
    if (recv(sockfd, &avatar_message, sizeof(avatar_message), 0) <= 0)
    {
        fprintf(stdout, "error: message could not be received from server\n");
        exit(1);
    }
    if (ntohl(avatar_message.type) != AM_AVATAR_TURN)
    {
        fprintf(stdout, "error: message is not AM_AVATAR_TURN. Instead got: %d\n", ntohl(avatar_message.type));
        exit(1);
    }
    /////////////////////////////////////////// ADD CODE HERE /////////////////////////////////////////
    //determine end point as location of first avatar (avatar 0)
    // XYPos* zeroPos = malloc(sizeof(XYPos*));
    // zeroPos->x = 0;
    // zeroPos->y = 0;
    // printf("in function for avatar %d\n", passed_id);
    if (passed_id == 0)
    {
        // globalZeroX = ntohl(avatar_message.avatar_turn.Pos[passed_id].x);
        // globalZeroY = ntohl(avatar_message.avatar_turn.Pos[passed_id].y);
        //printf("ZERO: %d, %d \n", globalZeroX, globalZeroY);
        //printf("leaving function \n");
        globalZeroX = 5;
        globalZeroY = 5;
    }

    //avatars have now sent the message to server

    XYPos* prevPos = malloc(sizeof(XYPos*));
    prevPos->x = ntohl(avatar_message.avatar_turn.Pos[passed_id].x);
    prevPos->y = ntohl(avatar_message.avatar_turn.Pos[passed_id].y);

    //do solving algorithm while maze is not solved
    while (ntohl(avatar_message.type) != AM_MAZE_SOLVED)
    {
        //pthread_mutex_lock(&mutex);

        //if its this Avatar's turn to move
        int current_avatar_id = ntohl(avatar_message.avatar_turn.TurnId);
        if (current_avatar_id  == passed_id)
        {
            int x = ntohl(avatar_message.avatar_turn.Pos[passed_id].x);
            int y = ntohl(avatar_message.avatar_turn.Pos[passed_id].y);
            learn_maze[x][y]->visited = 1;

            // printf("x coordinate for avatar %d is %d\n", passed_id, x);
            // printf("y coordinate for avatar %d is %d\n", passed_id, y);
            int avatarMove;
            avatarMove = findNextDir(prevPos, x, y, passed_avatar, globalZeroX, globalZeroY);
            //printf("AVATR MOVE IS: %d\n",avatarMove); 

            //have now determined a move, send it to the server
           	//send an AM_AVATAR_MOVE to the server so we can get back a message on whose turn it is to move
            avatar_message.type = htonl(AM_AVATAR_MOVE);
            avatar_message.avatar_move.AvatarId = htonl(passed_id);

            //need to determine a derection and have the avatar move that way
            avatar_message.avatar_move.Direction = htonl(avatarMove);
            prevPos->x = x;
            prevPos->y = y;
            //could not communicate with server
            if (send(sockfd, &avatar_message, sizeof(avatar_message), 0) <= 0)
            {
                fprintf(stderr, "error: avatar couldn't report move\n");
                exit(1);
            }
            //record move in logfile
            fprintf(passed_log, "Move made by: avatar %d, in direction: %d (0 west, 1 north, 2 south, 3 east, 8 null move)\n", passed_id, avatarMove);
        }

        //not this avatar's turn to move
        else
        {
            fprintf(passed_log, "Move not made: not avatar %d's turn to move\n", passed_id);
        }

        //recieve the next message
        if (recv(sockfd, &avatar_message, sizeof(avatar_message), 0) <= 0)
        {
            printf("error: message could not be received from server\n");
            exit(1);
        }
        //if we recieved an error
        if ((ntohl(avatar_message.type) != AM_AVATAR_TURN) && (ntohl(avatar_message.type) != AM_MAZE_SOLVED))
        {
            printf("error: message received was not AM_AVATAR_TURN or AM_MAZE_SOLVED\n");
            exit(1);
        }
    }

    if (ntohl(avatar_message.type) == AM_MAZE_SOLVED) 
    {    
        if (passed_id  == 0)
        {
            fprintf(passed_log, "\nSolved maze!\n");
        }
        printf("avatar %d solved the maze!\n", passed_id);
        free(prevPos);
        free(passed_avatar);

        if(sockfd)
        {
            close(sockfd);
        }
        //free memory
    }

    return 0;
}

