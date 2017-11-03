#include <Arduino.h>
#include "Main/ArduinoPorts.h"
#include "Subsystems/RobotDrive.ino"


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
    moveBackward(50, 3000); //runs for three seconds
    delay(3000); // waits for three seconds before looping :D
}

























































































// issa robot
