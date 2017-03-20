#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "controlfunctions.h"

void* irTimeoutFunction(void* args);
void irTriggerFunction(struct ElevatorData *ed, pthread_mutex_t *mutex);
void* irInterruptFunction(void* args);

void reachFloorTriggerFunction(struct ElevatorData *ed, pthread_mutex_t *mutex);
void* reachFloorInterruptFunction(void* args);

//this function should listen for key entry
//then when a key has been pressed
//call functions as is appropriate
void* keyInterruptFunction(void* args);
