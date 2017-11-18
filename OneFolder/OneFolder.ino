#include <Arduino.h>
#include "ArduinoPorts.h"
#include "SubsystemFunctions.h"
#include <Servo.h>
:)
Servo myservo;
const byte leftStick = 0x4C; // Hex byte that represents leftStick, xBee-wise
const byte rightStick = 0x52; // Hex byte that represents rightStick, xBee-wise

//Button bytes
const int LB = 1;
const int RB = 2;
const int A = 16;
const int B = 32;
const int X = 64;
const int Y = 128;

const int DpadUp = 1;
const int DpadDown = 2;
const int DpadLeft = 4;
const int DpadRight = 8;
const int Start = 16;
const int Back = 32;


/*****************************************************************************************
   EXPERIMENTAL CONSTS
 *****************************************************************************************/
const int MAX_SPEED = 127;

const int OUR_COLOR = 0; //color of block to return; 0 for red, 1 for blue
const int COLORS[2][3] = {{255, 0, 0}, {0, 0, 255}}; //color sensor readings for blocks
const double COLOR_THRESH = 25; //max euclidean dist btwn colors for there to be a match
const int INFRA_THRESH = 17; // < 17 = white, > 17 = snicker's bar; assuming infra does in fact go from 4-30
const int SAFE_ROT_TIME = 0.1; //when line is found, rot this much to go away from edge

const int SERVO_OPEN = 0; //max degrees servo can open (or close?)
const int SERVO_CLOSE = 180; //min degrees servo can close (or open?)

const double DIST_THRESH = 3; //distance to stop before it detects a wall
const double DIST_SLOW = 2; //block found --> go full speed until here
const double DIST_STOP = 0.5; //block found --> ease in and stop here to pick up block
const double DIST_ERROR = 0.1; //horiz dist btwn top and bottom sensor
//dist error for when it finds wall and bot sensor in front of top so it thinks it's block

const double TIME_ONE_ROT = 1.0; //time in ms for robot to rotate a full circle
const int FWD_THRESH = 5; //how long (in 0.1s intervals) to move forward in expl before rot
const int ROT_THRESH = 7; //how long to rotate left and then right before fwd again
const double L3R_THRESH = 0.1;
/*****************************************************************************************/


/***************************************************
 *  ROBOT STATES AND VARS
       -1 -- STOP
        0 -- find_line
        1 -- find_block
        2 -- grab_block
        3 -- return_block
        4 -- remove_block (from line)
        5 -- navigating maze ???
***************************************************/
int STATE = 0;
int NEXT_STATE = 0; //helper var for when grabbing block
double infra_avg;
double lr_infra[4]; //for determining if reached fork, 2 for left and right rot
double ultra_top, ultra_bot, ultra_left, ultra_right;
double l3r_left[3], l3r_right[3];
bool HAS_BLOCK = false;
int ROT_DIR = 0; //-1 for left, 0 for none, 1 for right
int ROT_CTR = 0; //when exploring, keep ctr to know when to switch dir


void setup() {
    Serial.begin(9600);
    pinMode(leftSpeed, OUTPUT);
    pinMode(rightDirection, OUTPUT);
    pinMode(rightSpeed, OUTPUT);
    pinMode(leftDirection, OUTPUT);
    myservo.attach(clawPort);
    pinMode(13, OUTPUT);
    colorOpen();
    ultraOn();
    
    //testing
    int color1[3] = {255, 0, 0};
    int color2[3] = {254, 0, 0};
    //Serial.println(compareColors(color1, color2));
}

void loop() {
    LOOP_RC();
}

void LOOP_RC() {
    if (Serial.available() >= 4) {
        byte leftMag;
        byte rightMag;
        byte nonFaceButtons;	
        byte faceAndShoulderButtons;
        
        // for grabbing data
        byte raw_data[4];
        for (int i = 0; i < 4; i++)
            raw_data[i] = Serial.read();
    
        // If we get out of sync realign data. Could also try Serial.flush()
        for (int i = 0; i < 4; i++) {
            if (raw_data[0] == 76 && raw_data[2] == 82) {
                break;
            }
            byte temp = raw_data[0];
            raw_data[0] = raw_data[1];
            raw_data[1] = raw_data[2];
            raw_data[2] = raw_data[3];
            raw_data[3] = raw_data[4];
            raw_data[4] = raw_data[5];
            raw_data[5] = temp;   
        }
    
        // for magnitudes
        leftMag = raw_data[1];
        rightMag = raw_data[3];
    
        //designed as a two stick drive system
        if (leftMag >= 0 && leftMag <= 127) {
          setLeftForwardSpeed(leftMag);
        }
        else if (leftMag > 127 && leftMag <= 255) {
          leftMag -= 126;
          setLeftBackwardSpeed(leftMag);
        }
        if (rightMag >= 0 && rightMag <= 127) {
          setRightForwardSpeed(rightMag);
        }
        else if (rightMag >= 127 && rightMag <= 255) {
          rightMag -= 126;
          setRightBackwardSpeed(rightMag);
    }

  /* The data for the buttons is a word, where the high byte has A,B,X,Y,LB,RB
     and the low byte has the DPAD, Back, Start, LeftStick and RightStick
  LB = 1
  RB = 2
  A = 16
  B = 32
  X = 64
  Y = 128

  DpadUp = 1
  DpadDown = 2
  DpadLeft = 4
  DpadRight = 8
  Start = 16
  Back = 32
  
  */   
     //face buttons and shoulder buttons
     faceAndShoulderButtons = raw_data[4];

     if (faceAndShoulderButtons = A){
      
      }
     if (faceAndShoulderButtons = B){
      
      }
     if (faceAndShoulderButtons = X){
      
      }
      if (faceAndShoulderButtons = Y){
      
      }
      
      if (faceAndShoulderButtons = LB){
      
      }

      if (faceAndShoulderButtons = RB){
      
      }
    }
}


void LOOP_AUTO() {
  infra_avg = infraGrab(A0) + infraGrab(A1) / 2.0;
  ultra_top = ultraGrab(4, 5);
  ultra_bot = ultraGrab(2, 3);
  ultra_left = ultraGrab(12, 11);
  leakyArrayAdd(ultra_left, l3r_left);
  ultra_right = ultraGrab(16, 17);
  leakyArrayAdd(ultra_right, l3r_right);
  switch (STATE) {
    case -1:
      stop();
    case 0:
      STATE_findLine();
    case 1:
      STATE_findBlock();
    case 2:
      STATE_grab();
    case 3:
      STATE_return();
    case 4:
      STATE_remove();
    case 5:
      STATE_maze();
    default:
      break;
  }
}


void STATE_findLine() { //state 0
  //assume we start out directly facing short side
  moveForwardFullSpeed(1.0); //to get out of that black crossroads part

  //if we just found the line
  if (infra_avg > INFRA_THRESH) {
    if (ROT_CTR != 0) { //if line was found by rotating, rotate more for good measure
      if (ROT_DIR == -1)
        turnLeft(MAX_SPEED, SAFE_ROT_TIME);
      else if (ROT_DIR == 1)
        turnRight(MAX_SPEED, SAFE_ROT_TIME);
    }
    ROT_DIR = 0;
    STATE = 1;
    return;
  }

  //line not found yet, so start looking
  int left_trend = findTrend(l3r_left);
  int right_trend = findTrend(l3r_right);
  if (left_trend > 0 && right_trend < 0) { //means facing slightly right
    while (infra_avg < INFRA_THRESH) {
      turnLeft(MAX_SPEED, SAFE_ROT_TIME * 5);
      infra_avg = infraGrab(A0) + infraGrab(A1) / 2.0;
    }
  }
  else if (right_trend > 0 && left_trend < 0) { //facing slightly left
    while (infra_avg < INFRA_THRESH) {
      turnRight(MAX_SPEED, SAFE_ROT_TIME * 5);
      infra_avg = infraGrab(A0) + infraGrab(A1) / 2.0;
    }
  }
  else {
    switch (ROT_DIR) {
      case -1:
        turnLeft(MAX_SPEED, 0.1);
        ROT_CTR++;
        if (ROT_CTR >= ROT_THRESH) {
          ROT_DIR = -1;
          ROT_CTR = 0;
        }
      case 1:
        turnLeft(MAX_SPEED, 0.1);
        ROT_CTR++;
        if (ROT_CTR >= ROT_THRESH) {
          ROT_DIR = 1;
          ROT_CTR = 0;
        }
      default:
        break;
	}
  }
}


void STATE_findBlock() { //state 1
  //check if wandered off line
  if (infra_avg < INFRA_THRESH) {
    if (isFork()) {
        //go forward a set amount
    }
    else {
      STATE = 0;
      return;
    }
  }

  //check if there's something in the way of bottom ultra sensor
  //should at least be on the line by now
  if (ultra_bot < DIST_THRESH) {
    if (ultra_top < DIST_THRESH + DIST_ERROR) { //just hit a wall!
      if (ultra_left < DIST_THRESH) {
        turnRight(MAX_SPEED, TIME_ONE_ROT / 4);
      }
      else if (ultra_right < DIST_THRESH) {
        turnLeft(MAX_SPEED, TIME_ONE_ROT / 4);
      }
      return; //immediately check next iter if it hit a wall again
    }
    else { //found a block!
      STATE = 2; //change to grabbing block mode

      //now check that the block is ours...
      uint16_t * rgb = colorSet();
      if (similarColors(rgb, COLORS[OUR_COLOR])) {
        Serial.println("Our Block found!");
        NEXT_STATE = 3; //next state should be to return block
      }
      else { //opponent colored block
        Serial.println("Opp. Block found!");
        NEXT_STATE = 4;
      }
    }
  }

  //if block wasn't found, start exploring
  if (STATE == 1) {
    switch (ROT_DIR) {
      case 0:
        moveForwardFullSpeed(0.1);
        ROT_CTR++;
        if (ROT_CTR >= FWD_THRESH) {
          ROT_DIR = -1;
          ROT_CTR = 0;
        }
      case -1:
        turnLeft(MAX_SPEED, 0.1);
        ROT_CTR++;
        if (ROT_CTR >= ROT_THRESH) {
          ROT_DIR = 1;
          ROT_CTR = 0;
        }
      case 1:
        turnRight(MAX_SPEED, 0.1);
        ROT_CTR++;
        if (ROT_CTR >= ROT_THRESH) {
          ROT_DIR = 0;
          ROT_CTR = 0;
        }
      default:
        break;
    }
  }
}


void STATE_grab() { //state 2
  //assume block is in straight line front of robot
  if (ultra_bot > DIST_SLOW) {
    moveForwardFullSpeed(0.5);
  }
  else if (ultra_bot > DIST_STOP) {
    moveForwardFullSpeed(0.1);
  }
  else { //in range to pick up block
    myservo.write(SERVO_CLOSE);
    HAS_BLOCK = true;
    if (NEXT_STATE == 3 || NEXT_STATE == 4) {
      STATE = NEXT_STATE;
    }
  }
}

void STATE_return() { //state 3
  //algo resembles looking for a toilet when u needa shit

  /*
    NEXT_STATE = 1;
    if (ultra_bot < DIST_THRESH) {
      if (ultra_top < DIST_THRESH + DIST_ERROR) { //just hit a wall
          //turn6
      }
    }
  */
}

void STATE_remove() { //state 4
  turnLeft(MAX_SPEED, TIME_ONE_ROT / 4);
  myservo.write(SERVO_OPEN);
  HAS_BLOCK = false;
  turnRight(MAX_SPEED, TIME_ONE_ROT / 4);
}


void STATE_maze() { //state 5
  Serial.println("xddddddd");
}

//given 2 RGB arrays, see if they're close enough to match
bool similarColors(int* c1, int* c2) {
  return sqrt(pow(c1[0] - c2[0], 2) + pow(c1[1] - c2[1], 2) + pow(c1[2] - c2[2], 2)) < COLOR_THRESH;
}

void leakyArrayAdd(double n, double* arr) {
  arr[2] = arr[1];
  arr[1] = arr[0];
  arr[0] = n;
}

int findTrend(double* arr) {
  if (arr[0] - arr[1] > L3R_THRESH && arr[1] - arr[2] > L3R_THRESH)
    return 1;
  if (arr[2] - arr[1] > L3R_THRESH && arr[1] - arr[0] > L3R_THRESH)
    return -1;
  return 0;
}

//fills up lr_infra with IR measurements in 1/8 rot increments
void infraMeasureCircle() {
  //helpful readings to determine if we reached a fork
  turnLeft(MAX_SPEED, TIME_ONE_ROT / 8);
  lr_infra[0] = infraGrab(A0) + infraGrab(A1) / 2.0;
  turnLeft(MAX_SPEED, TIME_ONE_ROT / 8);
  lr_infra[1] = infraGrab(A0) + infraGrab(A1) / 2.0;
  turnRight(MAX_SPEED, 3 * TIME_ONE_ROT / 8);
  lr_infra[2] = infraGrab(A0) + infraGrab(A1) / 2.0;
  turnRight(MAX_SPEED, TIME_ONE_ROT / 8);
  lr_infra[3] = infraGrab(A0) + infraGrab(A1) / 2.0;
  turnLeft(MAX_SPEED, TIME_ONE_ROT / 4);
}

bool isFork() {
  infraMeasureCircle();
  //if everyone is black, assume it's a fork
  return lr_infra[0] > INFRA_THRESH && lr_infra[1] > INFRA_THRESH && lr_infra[2] > INFRA_THRESH && lr_infra[0] > INFRA_THRESH;
}
