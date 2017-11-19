#include <Servo.h>
#include "SubsystemFunctions.h"
#include "ArduinoPorts.h"

const int led = 7;
// Motor RIGHT
int enA = 11;  //pwm
int in1 = 9; //forward
int in2 = 8; //backward
// Motor LEFT
int enB = 6;  //pwm
int in3 = 5; //forward
int in4 = 3; //backward
int servo = 10;
byte raw_data[7];
Servo myservo;

/*****************************************************************************************
   EXPERIMENTAL CONSTS
 *****************************************************************************************/
const byte MAX_SPEED = 127;

const byte OUR_COLOR = 0; //color of block to return; 0 for red, 1 for blue
//Goal zone color????????
const int COLORS[2][3] = {{255, 0, 0}, {0, 0, 255}}; //color sensor readings for blocks
const double COLOR_THRESH = 25; //max euclidean dist btwn colors for there to be a match

const int SERVO_OPEN = 0; //max degrees servo can open (or close?)
const int SERVO_CLOSE = 180; //min degrees servo can close (or open?)

const byte DIST_THRESH = 3; //distance to stop before it detects a wall w/ front ultra
const byte WALL_THRESH = 10; //dist disparity btwn left/right ultra so we know what type of corner
const byte DIST_SLOW = 2; //block found --> go full speed until here
const double DIST_STOP = 0.5; //block found --> ease in and stop here to pick up block
const double DIST_ERROR = 0.1; //horiz dist btwn top and bottom sensor
//dist error for when it finds wall and bot sensor in front of top so it thinks it's block

const double TIME_ONE_ROT = 1.0; //time in ms for robot to rotate a full circle
const byte FWD_THRESH = 5; //how long (in 0.1s intervals) to move forward in expl before rot
const byte ROT_THRESH = 7; //how long to rotate left and then right before fwd again
const double EXPLORE_FWD_STEP_TIME = 0.1; //how much to move forward each step
const double EXPLORE_ROT_STEP_TIME = 0.1; //hpw long to rotate each step

#define ULTRA_TREND_SIZE 5
const double ULTRA_TREND_THRESH = 0.1; //ultra trendsetter
const byte ULTRA_TREND_STEP = 32;
const double POST_TREND_ROT_FWD_TIME = 0.3; //after trend-based rot, move fwd for new ultra reading

const double GOAL_FWD_TIME = 0.5; //in goal, move toward corner so won't move it by accident

/* USELESS CONSTS
//const byte INFRA_THRESH = 17; // < 17 = white, > 17 = snicker's bar; assuming infra does in fact go from 4-30
//const double SAFE_ROT_TIME = 0.1; //when line is found, rot this much to go away from edge
//const double FORK_TIME_FWD = 1.0; //when reach fork, move this amt of time fwd
//const double FORK_TIME_ROT = 0.5; //how long to rot after mv fwd (examine blocks here)
//const byte REORIENT_STEP = 32;
//const double REORIENT_TIME_BK = 0.3;
//const double REORIENT_TIME_BK2 = 0.2;
*/
/*****************************************************************************************/


/***************************************************
    ROBOT STATES AND VARS
       -1 -- STOP
        //0 -- find_line
        1 -- find_block
        2 -- grab_block
        3 -- return_block
        4 -- remove_block (from line)
        5 -- fork
        6 -- maze ????
***************************************************/
byte STATE = 0;
byte NEXT_STATE = 0; //helper var for when grabbing block
//FUCK INFRA double infra;
//INFRA IS USELESS double lr_infra[4]; //for determining if reached fork, 2 for left and right rot
double ultra_top, ultra_bot, ultra_left, ultra_right;
double trend_left[ULTRA_TREND_SIZE] = {0}, trend_right[ULTRA_TREND_SIZE] = {0};
bool HAS_BLOCK = false;
byte ROT_DIR = 0; //-1 for left, 0 for none, 1 for right
byte ROT_CTR = 0; //when exploring, keep ctr to know when to switch dir





void setup() {
Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
pinMode(led, OUTPUT);
  // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  myservo.attach(10);
  ultraOn();
  // Start with motors disabled and direction forward
  
  // Motor A
  
  digitalWrite(enA, LOW);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  
  // Motor B
  
  digitalWrite(enB, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
//index 1 is left joystick Y, index 2 is left trigger, 
//and index 4 is right joystick Y, index 5 is right trigger 
//index 6 is 255 if there is new data
//Index 0 is 'L' index 3 is 'R'
//byte raw_data[7];
void checkData() {
   
//Wait until at least 6 bytes are available. We expect 6 bytes at a time from msg
if (Serial.available() >= 6) {
  for(int i = 0; i < 6; i++) {
    raw_data[i] = Serial.read();
    Serial.println("raw_data = ");
    Serial.println(raw_data[i]);
      Serial.print(" Data: ");
      Serial.print(raw_data[i]);
  }
//   If we get out of sync realign data. Could also try Serial.flush() instead if not aligned
  for(int i = 0; i < 6; i++) {
    if(raw_data[0] == 76 && raw_data[3] == 82) {
      break;
    }
    byte temp = raw_data[0];
    raw_data[0] = raw_data[1];  
    raw_data[1] = raw_data[2];  //1 is left joystick
    raw_data[2] = raw_data[3];  //2 is left trigger
    raw_data[3] = raw_data[4]; 
    raw_data[4] = raw_data[5];  //4 is right joystick
    raw_data[5] = temp;         //5 is right trigger
  }
  raw_data[6] = 255;
}
}
void loop() {
  
   checkData(); // Checks data and updates raw_data global variable
   
  //Raw data[6] is a flag that checkdata sets. Make sure to set this to other than 255 if we have tended to it        

//   int distance = ultraGrab(ultraFrontSendPort, ultraFrontReceivePort);
//   
//   if (distance < 10) {
//    turnLeft(2000);
//   }
//    moveForwardFullSpeed(900.0);

   
//if(raw_data[2] > 0) {
//  myservo.write(90);
//} else {
//  myservo.write(0);
//}
if (raw_data[4] <= 127) {                               //left stick is pressed FORWARD 
  analogWrite(leftSpeed, raw_data[4]*2);                       //Multiply by 2? ask joe/zotto
  digitalWrite(leftDirection, HIGH);
}
  else if (raw_data[4] > 127) {
    analogWrite(leftSpeed, raw_data[4]);
    digitalWrite(leftDirection, LOW);
  }
  
if (raw_data[1] <= 127) {                               //right stick is pressed BACKWARD 
  analogWrite(rightSpeed, raw_data[1]*2);                       //Multiply by 2? ask joe/zotto
  digitalWrite(rightDirection, HIGH);
}
  else if (raw_data[1] > 127) {
    analogWrite(rightSpeed, raw_data[1]);
    digitalWrite(rightDirection, LOW);
  }
  
}
 
