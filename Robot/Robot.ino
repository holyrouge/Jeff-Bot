#include <Arduino.h>
#include "Subsystems/RobotDrive.h"
#include "Subsystems/RobotDrive.ino"

/*
  Turns on an LED on for one second, then off for one second, repeatedly.
*/

void setup()
{
	Serial.begin(9600);


    pinMode(leftSpeed, OUTPUT);
    pinMode(rightDirection, OUTPUT);
    pinMode(rightSpeed, OUTPUT);
    pinMode(leftDirection, OUTPUT);


	pinMode(13, OUTPUT);
}

void loop()
{
	Serial.println("Hello world!");

    // moveBackward(255);
	delay(1000);              // wait for a second
	digitalWrite(13, HIGH);   // set the LED on
	delay(1000);              // wait for a second
	digitalWrite(13, LOW);    // set the LED off
}

