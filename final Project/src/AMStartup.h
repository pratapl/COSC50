/* ========================================================================== */
/* File: AMStartup.h
 *
 * Project name: Amazing Project
 * Component name: AMStartup
 *
 * This file contains the structs and definitions for the AMStartup program
 *
 */
/* ========================================================================== */

#ifndef AMSTARTUP_H
#define AMSTARTUP_H

// ---------------- Prerequisites e.g., Requires "math.h"
#include <stdio.h>   

#include "amazing.h"         
// ---------------- Constants
struct MazeTile* learn_maze[100][100]; 

// ---------------- Structures/Types
typedef struct MazeTile {
    int northBlocked;
    int eastBlocked;
    int southBlocked;
    int westBlocked;
    int visited;
    int deadEnd;
    int lastDir;                            
    int lastAvatar;
    int hasVisited[10];
    int mazeWidth;
    int mazeHeight;
    int gNorth;
    int gEast;
    int gSouth;
    int gWest;
    int occupied;
} MazeTile;

int globalZeroX;
int globalZeroY;

// ---------------- Public Variables

// ---------------- Prototypes/Macros


#endif // AMSTARTUP_H

