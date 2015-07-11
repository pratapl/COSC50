/* ========================================================================== */
/* File: avatar.h
 *
 * Project name: Amazing Project
 * Component name: AMStartup
 *
 * This file contains the structs and definitions for the AMStartup program
 *
 */
/* ========================================================================== */

#ifndef AVATAR_H
#define AVATAR_H

// ---------------- Prerequisites e.g., Requires "math.h"
#include <stdio.h>   

#include "amazing.h"         
// ---------------- Constants

// ---------------- Structures/Types

typedef struct AvatarStruct
{
	int id;
	int total_num_avatars;
	int difficulty;
	struct in_addr *address;
	int maze_port;
	char *logfile_name;
	FILE *file;
	int moved;
	int dir;

	
} AvatarStruct;

// ---------------- Public Variables

// ---------------- Prototypes/Macros

int getPrevDir(XYPos* prevPos, int currX, int currY);
int findNextDir(XYPos* prevPos, int currX, int currY, AvatarStruct* avatar, int globalZeroX, int globalZeroY);
 void *avatarFunction(void *threadInput);



#endif // AVATAR_H

