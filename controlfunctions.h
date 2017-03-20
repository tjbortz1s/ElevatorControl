#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"

void floorQueueManager(struct ElevatorData *ed, pthread_mutex_t *mutex, int requestNumber);
void floorLightsManager(struct ElevatorData *ed, pthread_mutex_t *mutex, int requestNumber, int toggle);

void openDoorRoutine(struct ElevatorData *ed);
void closeDoor(struct ElevatorData *ed);
