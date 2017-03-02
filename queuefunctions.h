#include <pthread.h>
#include <stdlib.h>
#include "types.h"

//these functions are expected to be called by functions that
//lock the mutex before they call them
//they will not lock or wait for a mutex to run
//as that would cause deadlock.
int enqueueFloor(struct ElevatorData *ed, int floor);
int enqueueFloorToFront(struct ElevatorData *ed, int floor);
int dequeueFloor(struct ElevatorData *ed);
int getQueueSize(struct ElevatorData *ed);
int increaseQueueSize(struct ElevatorData *ed);
