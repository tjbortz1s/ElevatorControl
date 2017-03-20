#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"

void floorQueueManager(struct ElevatorData *ed, pthread_mutex_t *mutex, int requestNumber);
void floorLightsManager(struct ElevatorData *ed, pthread_mutex_t *mutex, int requestNumber, int toggle);

void openDoorRoutine(struct ElevatorData *ed);
void closeDoor(struct ElevatorData *ed);

//these functions do as described
//to call them, pass a speed between 0 and 1024 that will
//determine how fast the motor will spin/how the PWM will be output
int toggleMotorUp(int speed);
int toggleMotorDown(int speed);
void toggleMotorOff();
