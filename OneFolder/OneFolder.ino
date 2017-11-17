#include <Arduino.h>
#include "ArduinoPorts.h"
#include <Servo.h>

Servo myservo;

const byte leftStick = 0x4C; // Hex byte that represents leftStick, xBee-wise
const byte rightStick = 0x52; // Hex byte that represents rightStick, xBee-wise


void setup()
{
  Serial.begin(9600);
    
    pinMode(leftSpeed, OUTPUT);
    pinMode(rightDirection, OUTPUT);
    pinMode(rightSpeed, OUTPUT);
    pinMode(leftDirection, OUTPUT);
    myservo.attach(clawPort);
    pinMode(13, OUTPUT);
    colorOpen();
    ultraOn();
    
}

void loop()
{
    LOOP_RC();
    moveBackward(50, 3000); //runs for three seconds
    delay(3000); // waits for three seconds before looping :D
}

void LOOP_RC(){
    if (Serial.available() >= 4){
    byte leftMag;
    byte rightMag;
   //                                     for grabing data                                          //   
    byte raw_data[4];
  for(int i = 0; i < 4; i++) {
    raw_data[i] = Serial.read();
  }
      // If we get out of sync realign data. Could also try Serial.flush()
  for(int i = 0; i < 4; i++) {
    if(raw_data[0] == 76 && raw_data[2] == 82) {
      break;
    }
  byte temp = raw_data[0];
    raw_data[0] = raw_data[1];
    raw_data[1] = raw_data[2];
    raw_data[2] = raw_data[3];
    raw_data[3] = temp;
  
  }
      
 //                                      for magnitudes                                             //
leftMag = raw_data[1];
rightMag = raw_data[3];
  
  //designed as a two stick drive system
  if (leftMag >= 0 && leftMag <= 127){
      setLeftSpeedForward(leftMag);
  }
  else if (leftMag > 127 && leftMag <= 255){
    leftMag -= 126;  
    setLeftSpeedBackward(leftMag);
  }

  if (rightMag >= 0 && rightMag <= 127){
      setRightSpeedForward(rightMag);
  }
  else if (rightMag >= 127 && rightMag <= 255){
      rightMag -= 126;
      setRightSpeedBackward(rightMag);
  }

  
    }
  }

void LOOP_AUTO(){
  uint16_t * rgb = colorSet();
  Serial.println(rgb[0]);
  }


