#pragma once
#include <pthread.h>

extern const int QUEUE_ERROR;
extern const int QUEUE_SUCCESS;

//inside buttons
extern const int FLOOR_ONE_REQUEST;
extern const int FLOOR_TWO_REQUEST;
extern const int FLOOR_THREE_REQUEST;

//outside buttons
extern const int FLOOR_ONE_UP_REQUEST;
extern const int FLOOR_TWO_DOWN_REQUEST;
extern const int FLOOR_TWO_UP_REQUEST;
extern const int FLOOR_THREE_DOWN_REQUEST;

extern const int FIRE_CODE;

extern const int INITIAL_QUEUE_SIZE;
extern const int QUEUE_EMPTY_FLAG ;

extern const int INITIAL_DOOR_WAIT_TIME ;
extern const int IR_DOOR_WAIT_TIME;

extern const int NUMBER_OF_FLOORS;


//structures
struct ElevatorData {
  //used by lights data

  //this cannot be of variable size in this .c file
  //instead I will initialize it to the proper size
  //with that, it will work fine
  //int externalFloorRequests[2 + ((NUMBER_OF_FLOORS - 2) * 2)];
  int *externalFloorRequests;
  //same here.  Turning to a pointer and malloc to make it an array of
  //propper size
  //int internalFloorRequests[NUMBER_OF_FLOORS];
  int *internalFloorRequests;

  //array, treated as a queue, that will
  //hold the next floor to go to.
  //the floor queue is going to start
  //as an array of INITIAL_QUEUE_SIZE
  //then will dynamically resize
  //as it is needed to while elements are removed

  int queueSize;
  int *floorQueue;

  //no longer queue-related stuff
  int reachedFloorFlag;
  //later on
  //make so that all threads
  //loop until this is equal to 1
  int stopProgramFlag;
  int doorFlag;
  long lastIRTime;
  int doorOpenFlag;
  int initialDoorWaitOverFlag;
  int currentFloor;
  int nextFloor;
};

struct ArgumentData{
  struct ElevatorData *ed;
  pthread_mutex_t *mutex;
};
