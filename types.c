#include "types.h"

const int QUEUE_ERROR = -274;
const int QUEUE_SUCCESS = -273;

//inside buttons
const int FLOOR_ONE_REQUEST = 0;
const int FLOOR_TWO_REQUEST = 1;
const int FLOOR_THREE_REQUEST = 2;

//outside buttons
const int FLOOR_ONE_UP_REQUEST = 3;
const int FLOOR_TWO_DOWN_REQUEST = 4;
const int FLOOR_TWO_UP_REQUEST = 5;
const int FLOOR_THREE_DOWN_REQUEST = 6;

const int FIRE_CODE = 1;

const int INITIAL_QUEUE_SIZE = 3;
const int QUEUE_EMPTY_FLAG = -1;

const int INITIAL_DOOR_WAIT_TIME = 10;
const int IR_DOOR_WAIT_TIME = 3;

const int NUMBER_OF_FLOORS = 3;

//THE BUTTON LAYOUT FOR THE Elevator
//MAPPED ONTO KEYBOARD KEYS TO BE PRESSED
// the three floors
//       1  2  3
// up:   q  w  ~
// down: ~  s  d
// the inside buttons
// 1:          r
// 2:          f
// 3:          v
// (extras?) z x c

const char FLOOR_CHAR_EXTERNAL_ONE_UP = 'q';

const char FLOOR_CHAR_EXTERNAL_TWO_UP = 'w';
const char FLOOR_CHAR_EXTERNAL_TWO_DOWN = 's';

const char FLOOR_CHAR_EXTERNAL_THREE_DOWN = 'd';

const char FLOOR_CHAR_INTERNAL_ONE = 'r';
const char FLOOR_CHAR_INTERNAL_TWO = 'f';
const char FLOOR_CHAR_INTERNAL_THREE = 'v';
