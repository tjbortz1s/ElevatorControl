#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define QUEUE_ERROR -274
#define QUEUE_SUCCESS -273

#define FLOOR_ONE_REQUEST 0
#define FLOOR_TWO_REQUEST 1
#define FLOOR_THREE_REQUEST 2

#define FLOOR_ONE_UP_REQUEST 3
#define FLOOR_TWO_DOWN_REQUEST 4
#define FLOOR_TWO_UP_REQUEST 5
#define FLOOR_THREE_DOWN_REQUEST 6

#define FIRE_CODE 1

//#define FLOOR_QUEUE_SIZE 3
#define INITIAL_QUEUE_SIZE 3
#define QUEUE_EMPTY_FLAG -1

#define INITIAL_DOOR_WAIT_TIME 10
#define IR_DOOR_WAIT_TIME 3

#define NUMBER_OF_FLOORS 3

//structures
struct ElevatorData {
  int floorRequests[2 + ((NUMBER_OF_FLOORS - 2) * 2)];
  int internalRequests[NUMBER_OF_FLOORS];
  //array, treated as a queue, that will
  //hold the next floor to go to.
  //the floor queue is going to start
  //as an array of INITIAL_QUEUE_SIZE
  //then will dynamically resize
  //as it is needed to while elements are removed

  //int floorQueue[FLOOR_QUEUE_SIZE];
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

//prototypes
void initializeData(struct ElevatorData *ed);
void irTimeoutFunction(struct ElevatorData *ed, pthread_mutex_t *mutex);
void irInterruptFunction(struct ElevatorData *ed, pthread_mutex_t *mutex);
void reachFloorFunction(struct ElevatorData *ed, pthread_mutex_t *mutex);
void floorQueueManager(struct ElevatorData *ed, pthread_mutex_t *mutex, int requestNumber);


//these functions are expected to be called by functions that
//lock the mutex before they call them
//they will not lock or wait for a mutex to run
//as that would cause deadlock.
int enqueueFloor(struct ElevatorData *ed, int floor);
int enqueueFloorToFront(struct ElevatorData *ed, int floor);
int dequeueFloor(struct ElevatorData *ed);
int getQueueSize(struct ElevatorData *ed);
int increaseQueueSize(struct ElevatorData *ed);

void openDoor(struct ElevatorData *ed);
void closeDoor(struct ElevatorData *ed);


int main(){
  struct ElevatorData x;
  initializeData(&x);
  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex, NULL);
  //program code where the mutex is passed as a reference/pointer
  //bla bla bla
  //program ends here
  int i;
  i = enqueueFloor(&x, 1);
  if(i == QUEUE_ERROR){
    printf("ErrorFound1\n");
  }
  i = enqueueFloor(&x, 2);
  if(i == QUEUE_ERROR){
    printf("ErrorFound2\n");
  }
  i = enqueueFloorToFront(&x, 3);
  if(i == QUEUE_ERROR){
    printf("ErrorFound3\n");
  }
  i = enqueueFloor(&x, 4);
  if(i == QUEUE_ERROR){
    printf("ErrorFound4\n");
  }

  printf("%d\n", getQueueSize(&x));
  i = 0;
  i = dequeueFloor(&x);
  if(i == QUEUE_ERROR){
    printf("ErrorFoundIndque\n");
  }
  else{
    printf("%s%d\n", "NoErrorFoundNumberis ", i);
  }

  i = 0;
  i = dequeueFloor(&x);
  if(i == QUEUE_ERROR){
    printf("ErrorFoundIn dque\n");
  }
  else{
    printf("%s%d\n", "NoErrorFoundNumberis ", i);
  }

  i = 0;
  i = dequeueFloor(&x);
  if(i == QUEUE_ERROR){
    printf("ErrorFoundIn dque\n");
  }
  else{
    printf("%s%d\n", "NoErrorFoundNumberis ", i);
  }

  i = 0;
  i = dequeueFloor(&x);
  if(i == QUEUE_ERROR){
    printf("ErrorFoundIn dque\n");
  }
  else{
    printf("%s%d\n", "NoErrorFoundNumberis ", i);
  }

  //not sure if this will ever happen safely
  //elevators never stop being controlled
  //ensure that
  pthread_mutex_destroy(&mutex);
  return 0;
}


void initializeData(struct ElevatorData *ed){
  int i = 0;
  //initialize floorRequests array
  for(i = 0; i < (2 + ((NUMBER_OF_FLOORS - 2) * 2)); i++){
    ed->floorRequests[i] = 0;
  }
  //initialize internalRequests array
  for(i = 0; i < NUMBER_OF_FLOORS; i++){
    ed->internalRequests[i] = 0;
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


//the function that initially opens the door
//should set the door open flag to 1
//should set the initialDoorWaitOverFlag to 1
//this portion of the code may actually be done in the part that
//manages opening and closing the door
//will have to be discussed in-person or in group later on
void openDoor(struct ElevatorData *ed){
  int theTime = time(NULL);
  ed->lastIRTime = theTime;
  ed->doorOpenFlag = 1;
  ed->initialDoorWaitOverFlag = 1;
  //do the function that "opens the door" here
}

void closeDoor(struct ElevatorData *ed){
  ed->doorOpenFlag = 0;
}

void irTimeoutFunction(struct ElevatorData *ed, pthread_mutex_t *mutex){
  long theTime = time(NULL);
  pthread_mutex_lock(mutex);
  //if the door isn'nt open do nothing
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

void irInterruptFunction(struct ElevatorData *ed, pthread_mutex_t *mutex){
  //if the door is not open, then the IR beam cannot
  //detect anything at all.
  pthread_mutex_lock(mutex);
  if(ed->doorOpenFlag){
    long theTime = time(NULL);

    ed->lastIRTime = theTime;
    ed->doorFlag = 1;
  }
  pthread_mutex_unlock(mutex);
}

//this function will run on reaching a floor
//it will need to open the doors
//then close if the ir value is false
//well, that means I don't need group discussion
//don't redo work

void reachFloorFunction(struct ElevatorData *ed, pthread_mutex_t *mutex){
  //we cannot pause the main thread
  //however, we will set a flag in the shared data class
  //and that flag should cause the main thread to stop trying to move

  //the main function should check this value
  //and if it is not 0, it should immidiately set it back
  //and then it will know a floor was reached

  pthread_mutex_lock(mutex);
  ed->reachedFloorFlag = 1;
  pthread_mutex_unlock(mutex);

  //this function should NOT open the doors
  //that should be done in the main function
  //as should checking if the IR flags are off
  //due to that the elevator will reach floors
  //that do not need to be stopped at.

}

void floorQueueManager(struct ElevatorData *ed, pthread_mutex_t *mutex, int requestNumber){
  //if this is the case
  //then the number is a multi-digit emergency code
  //which will contain an amount of data

  //if the reqeust number is more than two digits
  if(requestNumber > 99){
    //the last two of three digits
    int emergencyFloor = requestNumber % 100;
    //the first digit
    int emergencyCode = requestNumber / 100;

    //in event of fire
    if(emergencyCode == FIRE_CODE){
      //do fire things
    }
  }
  switch(requestNumber){
    case FLOOR_ONE_REQUEST: //internal floor 1
      break;
    case FLOOR_TWO_REQUEST: //internal floor 2
      break;
    case FLOOR_THREE_REQUEST: //internal floor 3
      break;
    case FLOOR_ONE_UP_REQUEST: //external floor 1 up request
      break;
    case FLOOR_TWO_DOWN_REQUEST: //external floor 2 down request
      break;
    case FLOOR_TWO_UP_REQUEST: //external floor 2 up request
      break;
    case FLOOR_THREE_DOWN_REQUEST: //external floor 3 down request
      break;
  }
}


//this is wrong
//it should put the number on the BACK of the queue
//it should NOT put it on the front
int enqueueFloor(struct ElevatorData *ed, int floor){
  int success = QUEUE_SUCCESS;
  //if the queue is full, expand it
  if(!(ed->floorQueue[ed->queueSize - 1] == QUEUE_EMPTY_FLAG)){
    success = increaseQueueSize(ed);
  }
  if(success == QUEUE_SUCCESS){ //if expansion was successful
    //add the value to the back
    int firstEmpty = 0;
    int i;
    for(i = 0; i < ed->queueSize; i++){
      if(ed->floorQueue[i] == QUEUE_EMPTY_FLAG){
        firstEmpty = i;
        i = ed->queueSize;
        break;
      }
    }
    ed->floorQueue[firstEmpty] = floor;
    return QUEUE_SUCCESS;
  }
  else{ //if expansion failed
    return QUEUE_ERROR;
  }
}

int getQueueSize(struct ElevatorData *ed){
  //runs through the queue until it hits a -1 value
  //then returns the count
  int i;
  int size = -1;
  for(i = 0; i < ed->queueSize; i++){
    if(ed->floorQueue[i] == QUEUE_EMPTY_FLAG){
      size = i - 1;
    }
  }
  if(size == -1){
    return i;
  }
  return size;
}

int enqueueFloorToFront(struct ElevatorData *ed, int floor){
  //move all data in ed back a section
  //empty spaces of ed will be cointed as a -1
  //if no -1 spaces exist, return error code
  int i = 0;
  int hasEmpty = 0;
  for(i = 0; i < ed->queueSize; i++){
    if(ed->floorQueue[i] == QUEUE_EMPTY_FLAG){
      hasEmpty = 1;
    }
  }
  //if there is no empty position
  //increase the size of the queue
  if(hasEmpty == 0){
    int hasEmpty = increaseQueueSize(ed);
  }
  else{
    hasEmpty = QUEUE_SUCCESS;
  }
  //if it was successfully expanded
  if(hasEmpty == QUEUE_SUCCESS){
    //move all values forward by 1
    int i;
    for(i = ed->queueSize - 1; i > 0; i--){
      ed->floorQueue[i] = ed->floorQueue[i - 1];
    }
    //set the 0th space to be the given value
    ed->floorQueue[0] = floor;
    return QUEUE_SUCCESS;
  }
  else{ //if expansion of queue failed
    return QUEUE_ERROR;
  }
}

int dequeueFloor(struct ElevatorData *ed){
  //if the 0th value is empty
  //return error
  //otherwise get the value and move values towards 0, set backmost value to empty
  if(ed->floorQueue[0] == QUEUE_EMPTY_FLAG){
    return QUEUE_ERROR;
  }
  else{
    //move all values forward by one
    int returnval = ed->floorQueue[0];
    int i;

    for(i = 0; i < ed->queueSize - 1; i++){
      ed->floorQueue[i] = ed->floorQueue[i + 1];
    }
    //set the last value to -1
    ed->floorQueue[ed->queueSize - 1] = QUEUE_EMPTY_FLAG;
    //print the full QUEUE
    return returnval;
  }
}


int increaseQueueSize(struct ElevatorData *ed){
  //create and initialize the storage array
  int oldSize = ed->queueSize;
  int storeArray[oldSize];
  int i;
  for(i = 0; i < oldSize; i++){
    storeArray[i] = ed->floorQueue[i];
  }
  //delete the old array
  free(ed->floorQueue);
  //create the new array
  //make it have twice the size of the old one
  ed->queueSize = (ed->queueSize * 2);
  ed->floorQueue = malloc(sizeof(int[ed->queueSize]));
  //if the new array wasn't initialized return the error value
  if(ed->floorQueue == NULL){
    return QUEUE_ERROR;
  }
  //initialize the new array to have the same values as the old one
  for(i = 0; i < oldSize; i++){
    ed->floorQueue[i] = storeArray[i];
  }
  //set the rest of the values to the empty flag
  for(;i < ed->queueSize; i++){
    ed->floorQueue[i] = QUEUE_EMPTY_FLAG;
  }
  return QUEUE_SUCCESS;
}
