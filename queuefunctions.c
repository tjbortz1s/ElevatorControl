#include "queuefunctions.h"


//this is fixed, and puts things on the back
//not the front
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


//returns the number of values in the queue
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
