#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"

void* irTimeoutFunction(void* args);
void* irInterruptFunction(void* args);
void* reachFloorFunction(void* args);
