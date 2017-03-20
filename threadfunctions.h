#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "controlfunctions.h"

void* irTimeoutFunction(void* args);
void* irInterruptFunction(void* args);
void* reachFloorFunction(void* args);

//this function should listen for key entry
//then when a key has been pressed
//call functions as is appropriate
void* keyListener(void* args);
