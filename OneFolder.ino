#include <Arduino.h>
#include "ArduinoPorts.h"
#include "SubsystemFunctions.h"
#include <Servo.h>

Servo myservo;
const byte leftStick = 0x4C; // Hex byte that represents leftStick, xBee-wise
const byte rightStick = 0x52; // Hex byte that represents rightStick, xBee-wise

//Button bytes
const int  LB = 1;
const int  RB = 2;
const int  A = 16;
const int  B = 32;
const int  X = 64;
const int  Y = 128;

const int DpadUp = 1;
const int DpadDown = 2;
const int DpadLeft = 4;
const int DpadRight = 8;
const int Start = 16;
const int Back = 32;


/*****************************************************************************************
 * EXPERIMENTAL CONSTS
 *****************************************************************************************/
const int MAX_SPEED = 127;
 
const int BLOCK_COLOR = 0; //color of block to return; 0 for red, 1 for blue
const int COLORS[2][3] = {{255, 0, 0}, {0, 0, 255}}; //color sensor readings for blocks
const double COLOR_THRESH = 25; //max euclidean dist btwn colors for there to be a match

const int SERVO_OPEN = 0; //max degrees servo can open (or close?)
const int SERVO_CLOSE = 180; //min degrees servo can close (or open?)

const double DIST_THRESH = 3; //distance to stop before it detects a wall
const double DIST_ERROR = 0.1; //horiz dist btwn top and bottom sensor
//dist error for when it finds wall and bot sensor in front of top so it thinks it's block

const double TIME_ONE_ROT = 1.0; //time in ms for robot to rotate a full circle
const int FWD_THRESH = 5; //how long (in 0.1s intervals) to move forward in expl before rot
const int ROT_THRESH = 7; //how long to rotate left and then right before fwd again
/*****************************************************************************************/


/***************************************************
 *  ROBOT STATES AND VARS
     * -1 -- STOP 
     *  0 -- exploratory (looking for blocks)
     *  1 -- grabbing block
     *  2 -- returning block
     *  3 -- removing block from view
     *  4 -- navigating maze ???
***************************************************/
int STATE = 0;
int NEXT_STATE = 0; //helper var for when grabbing block
double infra_avg;
double ultra_top, ultra_bot, ultra_left, ultra_right;
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
        byte raw_data[6];
        for(int i = 0; i < 4; i++)
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

        //                                                                    sticks                                                            //
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

        //                                                                buttons                                                                           //
        /*The data for the buttons is a word, where the high byte has A,B,X,Y,LB,RB
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
void STATE_grab() {
    //assume block is in straight line front of robot
    double distance_slow = 2; //go full speed until here
    double distance_stop = 0.5; //ease in and stop here to pick up block
    if (ultra_bot > distance_slow) {
        moveForwardFullSpeed(0.5);
    }
    else if (ultra_bot > distance_stop) {
        moveForwardFullSpeed(0.1);
    }
    else { //in range to pick up block
        myservo.write(SERVO_CLOSE);
        HAS_BLOCK = true;
        if (NEXT_STATE == 2 || NEXT_STATE == 3) {
            STATE = NEXT_STATE;
        }
    }
}

void STATE_return() {
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

void STATE_remove() {
    turnLeft(MAX_SPEED, TIME_ONE_ROT/2);
    myservo.write(SERVO_OPEN);
    HAS_BLOCK = false;
    turnRight(MAX_SPEED, TIME_ONE_ROT/2);
}

void STATE_explore() {
    //check if there's something in the way of bottom ultra sensor
    if (ultra_bot < DIST_THRESH) {
        if (ultra_top < DIST_THRESH + DIST_ERROR) { //just hit a wall!
            //do something when u hit el wallo
        }
        else { //found a block!
            STATE = 1; //change to grabbing block mode
            
            //now check that the block is ours...
            uint16_t * rgb = colorSet();
            if (sqrt(pow(COLORS[BLOCK_COLOR][0]-rgb[0],2)+pow(COLORS[BLOCK_COLOR][1]-rgb[1],2)+pow(COLORS[BLOCK_COLOR][2]-rgb[2],2))<COLOR_THRESH) {
                Serial.println("Our Block found!");
                NEXT_STATE = 2; //next state should be to return block
            }
            else { //opponent colored block
                //pick it up, turn 180, put it down, turn 180, and keep going
                Serial.println("Opp. Block found!");
                NEXT_STATE = 3;
            }
        }
    }

    //if block wasn't found, start exploring
    if (STATE == 0)
        switch(ROT_DIR) {
            case 0:
                moveForwardFullSpeed(0.1);
                ROT_CTR++;
                if (ROT_CTR >= FWD_THRESH) {
                    ROT_DIR = -1;
                    ROT_CTR = 0;
                }
            case -1:
                turnLeft(127, 0.1);
                ROT_CTR++;
                if (ROT_CTR >= ROT_THRESH) {
                    ROT_DIR = 1;
                    ROT_CTR = 0;
                }
            case 1:
                turnRight(127, 0.1);
                ROT_CTR++;
                if (ROT_CTR >= ROT_THRESH) {
                    ROT_DIR = 0;
                    ROT_CTR = 0;
                }
            default:
                break;
        }
    }










void LOOP_AUTO() {     
    infra_avg = infraGrab(A0) + infraGrab(A1) / 2.0;
    ultra_top = ultraGrab(4, 5);
    ultra_bot = ultraGrab(2, 3);
    ultra_left = ultraGrab(12, 11);
    ultra_right = ultraGrab(16, 17);

    switch(STATE) {
        case -1:
            stop();
        case 0:
            STATE_explore();
        case 1:
            STATE_grab();
        case 2:
            STATE_return();
        case 3:
            STATE_remove();
        default:
            break;
    }
}



