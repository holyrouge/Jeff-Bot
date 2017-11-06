#include <Arduino.h>
#include "ArduinoPorts.h"
#include <Servo.h>
Servo myservo;

void setup()
{
  Serial.begin(9600);
    
    pinMode(leftSpeed, OUTPUT);
    pinMode(rightDirection, OUTPUT);
    pinMode(rightSpeed, OUTPUT);
    pinMode(leftDirection, OUTPUT);
    myservo.attach(clawPort);
    pinMode(13, OUTPUT);
    ColorOpen();
    UltraOn();
    
}

void loop()
{
    moveBackward(50, 3000); //runs for three seconds
    delay(3000); // waits for three seconds before looping :D
}

void RC(){}

void Auto(){}


