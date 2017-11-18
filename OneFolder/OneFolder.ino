#include <Arduino.h>
#include "ArduinoPorts.h"
#include "SubsystemFunctions.h"
#include <Servo.h>

Servo myservo;
const byte leftStick = 0x4C; // Hex byte that represents leftStick, xBee-wise
const byte rightStick = 0x52; // Hex byte that represents rightStick, xBee-wise

//Button bytes
const byte LB = 1;
const byte RB = 2;
const byte A = 16;
const byte B = 32;
const byte X = 64;
const int Y = 128;

const byte DpadUp = 1;
const byte DpadDown = 2;
const byte DpadLeft = 4;
const byte DpadRight = 8;
const byte Start = 16;
const byte Back = 32;


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
	Serial.begin(9600);
	pinMode(leftSpeed, OUTPUT);
	pinMode(rightDirection, OUTPUT);
	pinMode(rightSpeed, OUTPUT);
	pinMode(leftDirection, OUTPUT);
	myservo.attach(clawPort);
	pinMode(13, OUTPUT);
	colorOpen();
	ultraOn();

    digitalWrite(leftSpeed, 0);
    digitalWrite(rightSpeed, 0);
	//testing
	int color1[3] = {255, 0, 0};
	int color2[3] = {254, 0, 0};
	//Serial.println(compareColors(color1, color2));
}

void loop() {
	ultra_left = ultraGrab(ultraTopSendPort, ultraTopReceivePort);
    Serial.println("ultra top: " + (String)ultra_top);
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
			if (raw_data[0] == 76 && raw_data[2] == 82)
				break;
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

		if (faceAndShoulderButtons = A) {

		}
		if (faceAndShoulderButtons = B) {

		}
		if (faceAndShoulderButtons = X) {

		}
		if (faceAndShoulderButtons = Y) {

		}
		if (faceAndShoulderButtons = LB) {

		}
		if (faceAndShoulderButtons = RB) {

		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LOOP_AUTO() {
	//INFRA CAN KILL ITSELF infra = infraGrab();
	ultra_top = ultraGrab(4, 5);
	ultra_bot = ultraGrab(2, 3);
	ultra_left = ultraGrab(12, 11);
	leakyArrayAdd(ultra_left, trend_left);
	ultra_right = ultraGrab(16, 17);
	leakyArrayAdd(ultra_right, trend_right);
	switch (STATE) {
	case -1:
		stop();
	case 0:
		//STATE_findLine();
        break;
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


void STATE_findBlock() { //state 1
    /*
    //check if wandered off line
	if (infra < INFRA_THRESH) {
		if (isFork()) {
			STATE = 5;
            return;
		}
		else {
			STATE = 0; //find line again
			return;
		}
	}
   */

    //first some centering calculations
    byte left_trend = findTrend(trend_left);
    byte right_trend = findTrend(trend_right);
    
    //first check if bottom ultra sensor sees something
	if (ultra_bot < DIST_THRESH) {
		if (ultra_top < DIST_THRESH + DIST_ERROR) { 
		    //WE HIT A WALL, now see if we're in hallway or corner
            byte wall_status = WAAAAAALLEEEE();
            if (wall_status < 0) { //right corner -> turn left
                turnLeft(MAX_SPEED, TIME_ONE_ROT / 4);
                return;
            }
            else if (wall_status > 0) { //left corner -> turn right
                turnRight(MAX_SPEED, TIME_ONE_ROT / 4);
                return;
            }
            else if (ROT_DIR == 0) { //hit a wall in a hallway when meant to go fwd, uh oh spaghettios
                //just double up on rotation based on trend
                if (left_trend > 0 && right_trend < 0) {
                    turnLeft( left_trend * TIME_ONE_ROT / 32);
                } else if (left_trend < 0 && right_trend > 0) {
                    turnRight( right_trend * TIME_ONE_ROT / 32);
                }
                moveForwardFullSpeed(POST_TREND_ROT_FWD_TIME); //get a new reading for ultra
                return; //immediately check next iter if it hit a wall again
            }
            //if reached here, prob means bot was rotating and saw a wall, just continue
            
		}
		else { //found a block!, this most likely isn't the goal zone so dw about sabotaging ourself
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
            return;
		}
	}

	/***** if block wasn't found, keep exploring *****/

    //first recenter the robot    
    if (left_trend > 0 && right_trend < 0) {
        turnLeft( left_trend * TIME_ONE_ROT / 32);
    } else if (left_trend < 0 && right_trend > 0) {
        turnRight( right_trend * TIME_ONE_ROT / 32);
    }
	switch (ROT_DIR) {
	case 0:
        if (ROT_CTR >= FWD_THRESH) {
            ROT_DIR = -1;
            ROT_CTR = 0;
            break;
        }
		moveForwardFullSpeed(EXPLORE_FWD_STEP_TIME);
		ROT_CTR++;
	case -1:
        if (ROT_CTR >= ROT_THRESH) {
            ROT_DIR = 1;
            ROT_CTR = 0;
            turnRight(ROT_THRESH * EXPLORE_ROT_STEP_TIME);
            break;
        }
		turnLeft(MAX_SPEED, EXPLORE_ROT_STEP_TIME);
		ROT_CTR++;
	case 1:
        if (ROT_CTR >= ROT_THRESH) {
            ROT_DIR = 0;
            ROT_CTR = 0;
            turnRight(ROT_THRESH * EXPLORE_ROT_STEP_TIME);
            break;
        }
		turnRight(MAX_SPEED, EXPLORE_ROT_STEP_TIME);
		ROT_CTR++;
	default:
		break;
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
    //first some centering calculations
    byte left_trend = findTrend(trend_left);
    byte right_trend = findTrend(trend_right);
    
    //first check if we hit a wall
    if (ultra_top < DIST_THRESH + DIST_ERROR) { 
        //now see if we're in hallway or corner
        byte wall_status = WAAAAAALLEEEE();
        if (wall_status < 0) { //right corner -> turn left
            turnRight(TIME_ONE_ROT / 8); //45deg
            moveForwardFullSpeed(GOAL_FWD_TIME);
            myservo.write(SERVO_OPEN);
            moveBackwardFullSpeed(GOAL_FWD_TIME);
            HAS_BLOCK = false;
            turnLeft(MAX_SPEED, 3 * TIME_ONE_ROT / 8);
            STATE = 0;
            return;
        }
        else if (wall_status > 0) { //left corner -> turn right
            turnLeft(TIME_ONE_ROT / 8); //45deg
            moveForwardFullSpeed(GOAL_FWD_TIME);
            myservo.write(SERVO_OPEN);
            moveBackwardFullSpeed(GOAL_FWD_TIME);
            HAS_BLOCK = false;
            turnRight(MAX_SPEED, 3 * TIME_ONE_ROT / 8);
            STATE = 0;
            return;
        }
        else if (ROT_DIR == 0) { //hit a wall in a hallway when meant to go fwd, uh oh spaghettios
            //just double up on rotation based on trend
            if (left_trend > 0 && right_trend < 0) {
                turnLeft( left_trend * TIME_ONE_ROT / 32);
            } else if (left_trend < 0 && right_trend > 0) {
                turnRight( right_trend * TIME_ONE_ROT / 32);
            }
            moveForwardFullSpeed(POST_TREND_ROT_FWD_TIME); //get a new reading for ultra
            return; //immediately check next iter if it hit a wall again
        }
        //if reached here, prob means bot was rotating and saw a wall, just continue
    }
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

void leakyArrayAdd(double n, double* arr) { //index 0 is most recent
    for (int i = ULTRA_TREND_SIZE-1; i > 0; i--)
        arr[i] = arr[i-1];
	arr[0] = n;
}

//-1 -- turn left, 0 -- no wall, 1 -- turn right
byte WAAAAAALLEEEE() {
    if (ultra_left - ultra_right > WALL_THRESH)
        return -1;
    else if (ultra_right - ultra_left > WALL_THRESH)
        return 1;
    return 0;
}

byte findTrend(double* arr) {
    byte center = 0;
    if (arr[ULTRA_TREND_SIZE-1] == 0)
        return 0; //not enough values to determine a trend
    for (int i = 1; i < ULTRA_TREND_SIZE; i++) {
        if (arr[i] - arr[i-1] > ULTRA_TREND_THRESH)
            center++;
        else if (arr[i-1] - arr[i] > ULTRA_TREND_THRESH)
            center--;
    }
	return center;
}

/* USELESS JUST LIKE ANDRE RODRIGO
//fills up lr_infra with IR measurements in 1/8 rot increments
void infraMeasureCircle() {
	//helpful readings to determine if we reached a fork
	turnLeft(MAX_SPEED, TIME_ONE_ROT / 8);
	lr_infra[0] = infraGrab();
	turnLeft(MAX_SPEED, TIME_ONE_ROT / 8);
	lr_infra[1] = infraGrab();
	turnRight(MAX_SPEED, 3 * TIME_ONE_ROT / 8);
	lr_infra[2] = infraGrab();
	turnRight(MAX_SPEED, TIME_ONE_ROT / 8);
	lr_infra[3] = infraGrab();
	turnLeft(MAX_SPEED, TIME_ONE_ROT / 4);
}
*/

/* WE AINT NEED NO INFRA WHEN THERE AINT NO LINE
void STATE_findLine() { //state 0
    //assume we start out directly facing short side
    moveForwardFullSpeed(1.0); //to get out of that black crossroads part

    //if we just found the line
    if (infra > INFRA_THRESH) {
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
        while (infra < INFRA_THRESH) {
            turnLeft(MAX_SPEED, SAFE_ROT_TIME * 5);
            infra = infraGrab();
        }
    }
    else if (right_trend > 0 && left_trend < 0) { //facing slightly left
        while (infra < INFRA_THRESH) {
            turnRight(MAX_SPEED, SAFE_ROT_TIME * 5);
            infra = infraGrab();
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
*/

/* THERE NO FORKS EITHER FOH
void STATE_fork() { //state 5
    moveForwardFullSpeed(FORK_TIME_FWD);
    turnLeft(FORK_TIME_ROT);
    STATE = 2; //change to grabbing block mode

    //now check that the block is ours...
    uint16_t * rgb = colorSet();
    if (similarColors(rgb, COLORS[OUR_COLOR])) {
        Serial.println("Our Block found!");
        //maybe sabotage the other teams block first   *************************************            
        NEXT_STATE = 3; //next state should be to return block
        return;
    }
    else { //opponent colored block
        Serial.println("Opp. Block found!");
        turnRight(2 * FORK_TIME_ROT);
        //should now be facing our block, so check again
        rgb = colorSet();
        if (similarColors(rgb, COLORS[OUR_COLOR])) {
            NEXT_STATE = 3;
            return;
        }
        else { //our block was probably moved, let's go on our merry way
            
        }
        //NEXT_STATE = 4; ************************************************
    }
}
*/

/* FORKS ARE NONEXISTENT MWAHAHA
bool isFork() {
    infraMeasureCircle();
    //if everyone is black, assume it's a fork
    return lr_infra[0] > INFRA_THRESH && lr_infra[1] > INFRA_THRESH && lr_infra[2] > INFRA_THRESH && lr_infra[0] > INFRA_THRESH;
}
*/

/* REORIENTATION IS FUTILE NOW FUCK INFRA
void reorient() { //use when robot on line, but want to be dead centered
    //move back a bit, esp. if near fork
    moveBackwardFullSpeed(REORIENT_TIME_BK);
    for (int i = 0; i < REORIENT_STEP; i++) {
        turnRight(TIME_ONE_ROT / REORIENT_STEP);
        infra = infraGrab();
        if (i >= 1 && infra > INFRA_THRESH) { //first black found 2nd time
            if (i < 8) {
                turnLeft((i+1) * TIME_ONE_ROT / 16);
            } else {
                turnRight((15-i) * TIME_ONE_ROT / 16);
            }
            moveBackwardFullSpeed(REORIENT_TIME_BK2);
            
        }
    }
}
*/










