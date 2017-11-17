#include <Arduino.h>
#include "ArduinoPorts.h"
#include "SubsystemFunctions.h"
#include <Servo.h>

Servo myservo;

const byte leftStick = 0x4C; // Hex byte that represents leftStick, xBee-wise
const byte rightStick = 0x52; // Hex byte that represents rightStick, xBee-wise

const int BLOCK_COLOR = 0; //color of block to get// 0 for red, 1 for blue
//find experimentally, these are the "actual" colors
const int COLORS[2][3] = {{255, 0, 0}, {0, 0, 255}};

/* ROBOT STATES
     *  0 -- exploratory (looking for blocks)
     *  1 -- grabbing block
     *  2 -- returning block
     *  4 -- navigating maze ???
     *  -1 -- STOP
     */
int STATE = 0;

int infra1 = 0, infra2 = 0;
double infra_avg = 0.0;
double ultra_top=0.0, ultra_bot=0.0, ultra_left=0.0, ultra_right=0.0;

    
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
    moveBackward(50, 3000); //runs for three seconds
    delay(3000); // waits for three seconds before looping :D
}


void LOOP_RC() {
    byte leftMag;
    byte rightMag;

    //grabs byte from Serial/Xbee
    byte currentStick = Serial.read();

    //if the byte is of value 4C, the next byte will be a magnitude for the stick
    if (currentStick == leftStick) {
        leftMag = Serial.read();
    }
    //same for the right
    else if (currentStick == rightStick) {
        rightMag = Serial.read();
    }

    //designed as a two stick drive system
    if (leftMag >= 0 && leftMag <= 127) {
        setLeftForwardSpeed(leftMag);
    }
    else if (leftMag > 127 && leftMag <= 255) {
        setLeftBackwardSpeed(leftMag);
    }

    if (rightMag >= 0 && rightMag <= 127){
        setRightForwardSpeed(rightMag);
    }
    else if (rightMag >= 127 && rightMag <= 255) {
        setRightBackwardSpeed(rightMag);
    }  
}


void LOOP_AUTO() {     
    infra1 = infraGrab(A0);
    infra2 = infraGrab(A1);
    infra_avg = infra1 + infra2 / 2.0;

    ultra_top = ultraGrab(4, 5);
    ultra_bot = ultraGrab(2, 3);
    ultra_left = ultraGrab(12, 11);
    ultra_right = ultraGrab(17, 16);


    switch(STATE) {
        case -1:
            break;
        case 0:
            STATE_explore();
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        default:
            break;
    }
    //timers  
}

//given 2 RGB arrays, see if they're close enough to match
boolean similarColor(int * c1, int * c2) {
    int color_threshold = 25;
    return sqrt( pow(c1[0] - c2[0], 2) + pow(c1[1] - c2[1], 2) + pow(c1[2] - c2[2], 2) ) < color_threshold;
}


void STATE_explore() {
    double distance_threshold = 3;
    double distance_error = 0.1; //horiz dist btwn top and bottom sensor
    //distance error accounts for when bot hits wall and bot sensor is slightly in front of top

    //check if there's something in the way of bottom ultra sensor
    if (ultra_bot < distance_threshold) {
        if (ultra_top < distance_threshold + distance_error) { //just hit a wall!
            //do something when u hit el wallo
        }
        else { //found a block!
            //now check that the block is ours...
            uint16_t * rgb = colorSet();
            if ( similarColor(rgb, COLORS[BLOCK_COLOR]) ) { //our colored block
                Serial.println("Block found!");
                STATE = 1; //change to grabbing block mode
            }
            else { //opponent colored block
                //either pick it up, turn 180, put it down, turn 180, and keep going
                //or just keep going
            }
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
        myservo.write(180); //is it 180 tho????
    }
}

void STATE_return() {

}




