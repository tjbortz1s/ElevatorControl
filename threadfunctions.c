#include "threadfunctions.h"

//this runs constantly in the background, checking for the IR to time out
void* irTimeoutFunction(void* args){
  struct ArgumentData *ad = (struct ArgumentData *)args;
  struct ElevatorData *ed = ad->ed;
  pthread_mutex_t *mutex = ad->mutex;

  while(1){
    //printf("RunningTimeout\n");
    long theTime = time(NULL);
    pthread_mutex_lock(mutex);
    //if the door isn't open do nothing
    if(ed->doorOpenFlag){
      //wait the initial time to pass
      if(theTime - ed->lastIRTime > INITIAL_DOOR_WAIT_TIME && ed->initialDoorWaitOverFlag == 1){
        ed->initialDoorWaitOverFlag = 0;
      }
      //from them on, pause for a bit if anyone walks through the door again
      if(theTime - ed->lastIRTime > IR_DOOR_WAIT_TIME  && ed->initialDoorWaitOverFlag == 0){
        ed->doorFlag = 0;
      }
    }
    pthread_mutex_unlock(mutex);
  }
}

//this will be run when the IR interrputs
void* irInterruptFunction(void* args){
  struct ArgumentData *ad = (struct ArgumentData *)args;
  struct ElevatorData *ed = ad->ed;
  pthread_mutex_t *mutex = ad->mutex;
  //if the door is not open, then the IR beam cannot
  //detect anything at all.
  while(1){
    //this will actually
    //be a blocking function
    //instead of an if statement
    //int err = waitforInterrupt(PI_GIPO_IR_PINNUM, -1(time in ms))
    //if(err == -1) there was an error
    //if(err == 0) timeout happened
    //if(err == 1) pin was triggered
    if(0){
      pthread_mutex_lock(mutex);
      if(ed->doorOpenFlag){
        long theTime = time(NULL);

        ed->lastIRTime = theTime;
        ed->doorFlag = 1;
      }
      pthread_mutex_unlock(mutex);
  }
  }
}

//this thread will run on reaching a floor
void* reachFloorFunction(void* args){
  struct ArgumentData *ad = (struct ArgumentData *)args;
  struct ElevatorData *ed = ad->ed;
  pthread_mutex_t *mutex = ad->mutex;
  //check if the door has been opened
  //this should use some sort of blocking function
  //that will be provided by the wiringpi library
  //which is not in use yet.
  int checkForElevatorFloorReach = 0;
  while(1){
    //this will actually
    //be a blocking function
    //instead of an if statement
    //int err = waitforInterrupt(PI_GIPO_REED_PINNUM, -1)
    //if(err == -1) there was an error
    //if(err == 0) timeout happened
    //if(err == 1) pin was triggered
    if(checkForElevatorFloorReach){
      pthread_mutex_lock(mutex);
      ed->reachedFloorFlag = 1;
      pthread_mutex_unlock(mutex);
    }
  }

}

void* keyListener(void* args){
  struct ArgumentData *ad = (struct ArgumentData *)args;
  struct ElevatorData *ed = ad->ed;
  pthread_mutex_t *mutex = ad->mutex;

  char str[50];
  str[0] = 'n';

  while(1){
    fgets(str, 50, stdin);
    printf("%s\n", str);
    //case statement doesn't like const variables
    if(str[0] == FLOOR_CHAR_INTERNAL_ONE){
      //void floorQueueManager(struct ElevatorData *ed, pthread_mutex_t *mutex, int requestNumber);
      floorQueueManager(ed, mutex, FLOOR_ONE_REQUEST);
    }
    else if(str[0] == FLOOR_CHAR_INTERNAL_TWO){
      floorQueueManager(ed, mutex, FLOOR_TWO_REQUEST);
    }
    else if(str[0] == FLOOR_CHAR_INTERNAL_THREE){
      floorQueueManager(ed, mutex, FLOOR_THREE_REQUEST);
    }

    else if(str[0] == FLOOR_CHAR_EXTERNAL_ONE_UP){
      floorQueueManager(ed, mutex, FLOOR_ONE_UP_REQUEST);
    }

    else if(str[0] == FLOOR_CHAR_EXTERNAL_TWO_UP){
      floorQueueManager(ed, mutex, FLOOR_TWO_UP_REQUEST);
    }
    else if(str[0] == FLOOR_CHAR_EXTERNAL_TWO_DOWN){
      floorQueueManager(ed, mutex, FLOOR_TWO_DOWN_REQUEST);
    }

    else if(str[0] == FLOOR_CHAR_EXTERNAL_THREE_DOWN){
      floorQueueManager(ed, mutex, FLOOR_THREE_DOWN_REQUEST);
    }
  }
}
