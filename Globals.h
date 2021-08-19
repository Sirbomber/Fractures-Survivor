#ifndef GLOBALS_H
#define GLOBALS_H

#include <windows.h>

#include "odprintf.h"

//#define DEBUGMODE // sets mission type to colony, force creates all player bases
//#define TESTMODE // adds loads of resources, 99 morale, adds spaceport base, blight always visible, other things
//#define SUPERSPEED // speeds up the game loop to run at the maximum possible speed your CPU can handle, don't define this in multiplayer builds
//#define FLATLAND // removes all cliffs, rocks, unpassable tiles
#define SWARMIDING 0 // runununununNNN

#define numof(array) (sizeof(array)/sizeof(array[0]))
#define autosize(array) numof(array), array

bool IsBetaray(int player);

extern HINSTANCE hInstDLL;

#endif