#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "queuefunctions.h"
#include "threadfunctions.h"
#include "controlfunctions.h"

//prototypes
void initializeData(struct ElevatorData *ed);

int main(){

  //the main function should check this value (reachedfloorFlag)
  //and if it is not 0, it should immidiately set it back
  //and then it will know a floor was reached

  struct ElevatorData x;
  initializeData(&x);
  //int err = wiringPiSetup();
  //if(err == -1) then there is an error and it should be
  //handled

  //now setup the pins
  //pinMode(GIPO_PIN_MOTOR_PWM, PWM_OUTPUT);
  //pinMode(GIPO_PIN_MOTOR_UP, OUTPUT);
  //pinMode(GIPO_PIN_MOTOR_DOWN, OUTPUT);
  //pinMode(GIPO_PIN_REED, INPUT);
  //pinMode(GIPO_PIN_IR_RECIEVE, INPUT);
  //pinMode(GIPO_PIN_IR_SEND, OUTPUT);
  //ONLY PIN ONE (BCM_GIPO_18) SUPPORTS PWM OUTPUT

  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex, NULL);

  //create the threads here

  //the threads that need to run in the background
  pthread_t threads[4];

  //create the argument structure that will be passed to
  struct ArgumentData ad;
  ad.ed = &x;
  ad.mutex = &mutex;

  //create all the background threads
  int error;
  error = pthread_create(&threads[0], NULL, &irTimeoutFunction, &ad);
  error = pthread_create(&threads[1], NULL, &irInterruptFunction, &ad);
  error = pthread_create(&threads[2], NULL, &reachFloorInterruptFunction, &ad);
  error = pthread_create(&threads[3], NULL, &keyInterruptFunction, &ad);

  //start the main loop
  while(1){
	  pthread_mutex_lock(&mutex);
    //if the floor was reached as triggered by the
    //reached floor function
	  if(x.reachedFloorFlag == 1){
      logString("Reached Floor Flag Hit", LOG_LEVEL_DEBUG);
		  //if going up, add 1 to the current floor
		  if(x.nextFloor > x.currentFloor) {
        logString("Moving up a floor", LOG_LEVEL_DEBUG);
			  x.nextFloor = x.nextFloor + 1;
		  }
		  //if going down subtract 1 from the currentfloor
		  else if(x.nextFloor < x.currentFloor) {
        logString("Moving down a floor", LOG_LEVEL_DEBUG);
			  x.nextFloor = x.nextFloor - 1;
		  }
		  //if we just got to the floor we want
		  //go into a loop opening and closing the door
		  //set flag back to 0 after
		  if(x.nextFloor == x.currentFloor) {
  			// open doors, remove nextfloor from queue, close doors
  			//this routine will manage all the open door and close
  			//door flags and so on
        logString("Opening Door", LOG_LEVEL_DEBUG);
  			openDoorRoutine(&x);
  			if(dequeueFloor(&x) == QUEUE_ERROR){
          //log the fact an error happened
          logString("QUEUE_ERROR", LOG_LEVEL_ERROR);
  			}
  			//floorLightsManager(&x,
  			//turn off lights for the floor
		  }
      logString("Reached floor flag end", LOG_LEVEL_DEBUG);
		  x.reachedFloorFlag = 0;
	  }

    //if the elevator needs to move up or down to get to where it is going
	  if(x.nextFloor < x.currentFloor) {
		  //elevator down
      //
      logString("MovingDown", LOG_LEVEL_SUPERDEBUG);
      toggleMotorDown(ELEVATOR_DEFAULT_SPEED_DOWN);
	  }
	  if(x.nextFloor > x.currentFloor) {
		  // elevator up
      logString("MovingUp", LOG_LEVEL_SUPERDEBUG);
      toggleMotorUp(ELEVATOR_DEFAULT_SPEED_UP);
	  }
    if(x.nextFloor == x.currentFloor){
      logString("Not Moving", LOG_LEVEL_SUPERDEBUG);
      toggleMotorOff();
    }
    //unlock the mutex
	  pthread_mutex_unlock(&mutex);
  }
  //not sure if this will ever happen safely
  //elevators never stop being controlled
  //ensure that
  pthread_mutex_destroy(&mutex);
  return 0;
}


void initializeData(struct ElevatorData *ed){
  int i = 0;
  //initialize internalFloorRequests array
  ed->internalFloorRequests = (int*) malloc(sizeof(int) * (2 + ((NUMBER_OF_FLOORS - 2) * 2)));
  for(i = 0; i < (2 + ((NUMBER_OF_FLOORS - 2) * 2)); i++){
    ed->internalFloorRequests[i] = 0;
  }
  //initialize externalFloorRquests
  ed->externalFloorRequests = (int*) malloc(sizeof(int) * NUMBER_OF_FLOORS);
  for(i = 0; i < NUMBER_OF_FLOORS; i++){
    ed->externalFloorRequests[i] = 0;
  }
  //initialize the floorQueue array
  //initialize the size of the queue
  ed->queueSize = INITIAL_QUEUE_SIZE;
  ed->floorQueue = (int*) malloc(sizeof(int) * ed->queueSize);
  for(i = 0; i < ed->queueSize; i++){
    ed->floorQueue[i] = QUEUE_EMPTY_FLAG;
  }


  ed->reachedFloorFlag = 0;
  ed->stopProgramFlag = 0;
  ed->doorFlag = 0;
  ed->lastIRTime = 0;
  ed->doorOpenFlag = 0;
  ed->initialDoorWaitOverFlag = 0;
  ed->currentFloor = 0;
  ed->nextFloor = 0;
}
