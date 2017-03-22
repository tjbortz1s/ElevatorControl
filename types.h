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

extern const char FLOOR_CHAR_EXTERNAL_ONE_UP;

extern const char FLOOR_CHAR_EXTERNAL_TWO_UP;
extern const char FLOOR_CHAR_EXTERNAL_TWO_DOWN;

extern const char FLOOR_CHAR_EXTERNAL_THREE_DOWN;

extern const char FLOOR_CHAR_INTERNAL_ONE;
extern const char FLOOR_CHAR_INTERNAL_TWO;
extern const char FLOOR_CHAR_INTERNAL_THREE;

extern const int ELEVATOR_DEFAULT_SPEED_DOWN;
extern const int ELEVATOR_DEFAULT_SPEED_UP;

extern const int GIPO_PIN_IR_SEND;
extern const int GIPO_PIN_IR_RECIEVE;
extern const int GIPO_PIN_REED;
extern const int GIPO_PIN_MOTOR_DOWN;
extern const int GIPO_PIN_MOTOR_UP;
extern const int GIPO_PIN_MOTOR_PWM;

//lots of output, shouldn't have alot happening
extern const int LOG_LEVEL_DEBUG;
//are there bugs happening?
extern const int LOG_LEVEL_ERROR;
//I WANT EVERYTHING EVEN IF IT LOOPS A THOUSAND TIMES A SECOND!
extern const int LOG_LEVEL_SUPERDEBUG;

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


//functions
//logging will have levels
//log function will take a string and an int
//logs above the level of the current number will display
//logs below the level set here will not display
void logString(char* string, int level);

void debugBlock(struct ElevatorData *ed);
