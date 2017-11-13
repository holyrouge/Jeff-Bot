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
    byte leftMag;
    byte rightMag;

    //grabs byte from Serial/Xbee
    byte currentStick = Serial.read();

   //if the byte is of value 4C, the next byte will be a magnitude for the stick
   if (currentStick == leftStick){
      leftMag = Serial.read();
   }
   //same for the right
   else if(currentStick == rightStick){
      rightMag = Serial.read();
   }

  //designed as a two stick drive system
  if (leftMag >= 0 && leftMag <= 127){
      setLeftSpeedForward(leftMag);
  }
  else if (leftMag > 127 && leftMag <= 255){
      setLeftSpeedBackward(leftMag);
  }

  if (rightMag >= 0 && rightMag <= 127){
      setRightSpeedForward(rightMag);
  }
  else if (rightMag >= 127 && rightMag <= 255){
      setRightSpeedBackward(rightMag);
  }

  
  
  }

void LOOP_AUTO(){
  uint16_t * rgb = colorSet();
  Serial.println(rgb[0]);
  }


