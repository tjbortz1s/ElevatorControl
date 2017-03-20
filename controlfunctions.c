#include "controlfunctions.h"

//the function that initially opens the door
//should set the door open flag to 1
//should set the initialDoorWaitOverFlag to 1
//this portion of the code may actually be done in the part that
//manages opening and closing the door
//will have to be discussed in-person or in group later on
void openDoorRoutine(struct ElevatorData *ed){
  int theTime = time(NULL);
  ed->lastIRTime = theTime;
  ed->doorFlag = 1;
  ed->doorOpenFlag = 1;
  ed->initialDoorWaitOverFlag = 1;
  //do the function that "opens the door" here
}

//run by openDoorRoutine to close the door after it is complete
void closeDoor(struct ElevatorData *ed){
  ed->doorOpenFlag = 0;
}

void floorLightsManager(struct ElevatorData *ed, pthread_mutex_t *mutex, int requestNumber, int toggle){
	//if toggle is 0
	//lights are probably being turne don

	//if toggle is 1
	//the door is opening
	//and lights are probably being turned off
}

int turnRequestNumberIntofloor(int requestNumber){
	if((requestNumber == 0) || (requestNumber == 3)) {
    return 1;
  }
  if ((requestNumber == 2) || (requestNumber == 6)){
    return 3;
  }
  if(requestNumber == 1){
    return 2;
  }
  //two down
  if (requestNumber == 4){
    return 4;
  }
  //two up
  if (requestNumber == 5){
    return 5;
  }
  return -1;
}

void floorQueueManager(struct ElevatorData *ed, pthread_mutex_t *mutex, int requestNumber){

  int realRequest = turnRequestNumberIntofloor(requestNumber);

  pthread_mutex_lock(mutex);
  //turn lights on for the floor
  //add the pressed buttons into the array of pressed buttons
  floorLightsManager(ed, mutex, requestNumber, 0);

  int temp;
  //if the request is not the floor the elevator is on or moving away from
  if(realRequest != ed->currentFloor){
    //if the current floor is 2
  	if(ed->currentFloor == 2 ) {
	  // push request to front of queue, regardless of what it is
      if(realRequest <= 3)
        {
          temp = ed->nextFloor;
          ed->nextFloor = realRequest;
          enqueueFloorToFront(ed,temp);
        }
      else if (realRequest == 4)
        {
          temp = ed->nextFloor;
          ed->nextFloor = 1;
          enqueueFloorToFront(ed,temp);
        }
      else if (realRequest == 5)
        {
          temp = ed->nextFloor;
          ed->nextFloor = 3;
          enqueueFloorToFront(ed,temp);
        }
      return;
  	}

  	if(ed->currentFloor == 3) {
  		if((realRequest == 2) || (realRequest == 4) || (realRequest == 5)){
  			// if request is for second floor, add it to front of queue
          temp = ed->nextFloor;
          ed->nextFloor = 2;
          enqueueFloorToFront(ed,temp);
  		}
  		if((realRequest == 1) && (ed->nextFloor != 2))  {
			//if request is for first floor and we don't need to go to 2, push 1 to front of queue
        temp = ed->nextFloor;
        ed->nextFloor = 1;
        enqueueFloorToFront(ed,temp);
  		}
  		if((realRequest == 1) && (ed->nextFloor == 2))  {
			// if request is for floor 1, and we need to stop at to 2, push 1 to back of queue
  			enqueueFloor(ed, realRequest);
  		}
      return;
  	}

  	if(ed->currentFloor == 1) {
  		if((realRequest == 2) || (realRequest == 4) || (realRequest == 5)){
  			// if request is for the second floor, add it to front of queue
        temp = ed->nextFloor;
        ed->nextFloor = 2;
        enqueueFloorToFront(ed,temp);
  		}
  		if((realRequest == 3) && (ed->nextFloor != 2))  {
			//if request is for floor 3, and we don't have to stop at 2, add 3 to front of queue
        temp = ed->nextFloor;
        ed->nextFloor = 3;
        enqueueFloorToFront(ed,temp);
  		}
  		if((realRequest == 3) && (ed->nextFloor == 2))  {
			//if request is for floor 3, and we have to stop at 2, add 3 to back of queue
  			enqueueFloor(ed, requestNumber);
  		}

  		return;
  	}
  }
	if((ed->currentFloor != 1) && (getQueueSize(ed) == 0)){
		//if the queue is empty, the elevator should return to floor 1
			enqueueFloor(ed, 1);
	}
  pthread_mutex_unlock(mutex);
}
