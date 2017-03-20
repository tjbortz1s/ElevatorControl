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
	//we cannot pause the main thread
  //however, we will set a flag in the shared data class
  //and that flag should cause the main thread to stop trying to move

  //the main function should check this value (reachedfloorFlag)
  //and if it is not 0, it should immidiately set it back
  //and then it will know a floor was reached

  struct ElevatorData x;
  initializeData(&x);
  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex, NULL);

  //create the threads here

  //---thread info:---
  //thread has a thread id
  //thread id is unique in the context of the process
  //thread id is not necesarily an int, may be a structure
  //thread id cannot be easily printed
  //thread id type is pthread_t
  //create thread function
  //
  //int pthread_create(pthread_t *restrict tidp,
  // const pthread_attr_t *restrict attr, void *(*start_rtn)(void), void *restrict arg);
  //
  //first var will hold the id of the new thread
  //second var contains thread attributes such as priority
  //third is a function pointer to the function the thread will run
  //fourth is the arguments of the function being called, as a structure
  //return value is for errors/status, not zero means error happened
  //---end thread info:---

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
  error = pthread_create(&threads[2], NULL, &reachFloorFunction, &ad);
  error = pthread_create(&threads[3], NULL, &keyListener, &ad);


  //this is just a test
  pthread_mutex_lock(&mutex);
  openDoorRoutine(&x);
  while(x.doorFlag == 1){
    pthread_mutex_unlock(&mutex);
    //printf("%s\n", "DOOR IS FLAGGED");
    pthread_mutex_lock(&mutex);
  }
  pthread_mutex_unlock(&mutex);
  printf("%s\n", "Door no longer flagged");
  //end the test code

  //start the main loop
  while(1){
	  pthread_mutex_lock(&mutex);
    //if the floor was reached as triggered by the
    //reached floor function
	  if(x.reachedFloorFlag == 1){
		  //if going up, add 1 to the current floor
		  if(x.nextFloor > x.currentFloor) {
			  x.nextFloor = x.nextFloor + 1;
		  }
		  //if going down subtract 1 from the currentfloor
		  else if(x.nextFloor < x.currentFloor) {
			  x.nextFloor = x.nextFloor - 1;
		  }
		  //if we just got to the floor we want
		  //go into a loop opening and closing the door
		  //set flag back to 0 after
		  if(x.nextFloor == x.currentFloor) {
			// open doors, remove nextfloor from queue, close doors
			//this routine will manage all the open door and close
			//door flags and so on
			openDoorRoutine(&x);
			if(dequeueFloor(&x) == QUEUE_ERROR){
				//error message
			}
			//floorLightsManager(&x,
			//turn off lights for the floor
		  }
		  x.reachedFloorFlag == 0;

	  }

    //if the elevator needs to move up or down to get to where it is going
	  if(x.nextFloor < x.currentFloor) {
		  //elevator down
	  }
	  if(x.nextFloor > x.currentFloor) {
		  // elevator up
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
