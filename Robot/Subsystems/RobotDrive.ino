
#include "Main/ArduinoPorts.h"
#include "SubsystemFunctions.h"

void moveForward(int speed){
  digitalWrite(rightDirection, 1);
  analogWrite(rightSpeed, speed);

  digitalWrite(leftDirection, 1);
  analogWrite(leftSpeed, speed);
  }

void moveBackward(int speed){
  digitalWrite(rightDirection, 0);
  analogWrite(rightSpeed, speed);

  digitalWrite(leftDirection, 0);
  analogWrite(leftSpeed, speed);
  }

void turnLeft(int speed){
  digitalWrite(rightDirection, 0);
  analogWrite(rightSpeed, speed);

  digitalWrite(leftDirection, 1);
  analogWrite(leftSpeed, speed);
  }
void turnRight(int speed){
  digitalWrite(rightDirection, 1);
  analogWrite(rightSpeed, speed);

  digitalWrite(leftDirection, 0);
  analogWrite(leftSpeed, speed);
  }

