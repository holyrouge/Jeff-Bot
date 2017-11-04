#include <Arduino.h>
#include "Main/ArduinoPorts.h"
#include "SubsystemFunctions.h"

void stop(){
    analogWrite(rightSpeed, 0);
    analogWrite(leftSpeed, 0);
}

void moveForward(int speed){
  digitalWrite(rightDirection, 1);
  analogWrite(rightSpeed, speed);

  digitalWrite(leftDirection, 1);
  analogWrite(leftSpeed, speed);
}

void moveForward(int speed, double time){
  moveForward(speed);
  delay(time);
  stop();
  }
void moveForwardFullSpeed(double time){
    moveForward(255, time);
}
void moveBackward(int speed){
  digitalWrite(rightDirection, 0);
  analogWrite(rightSpeed, speed);
  digitalWrite(leftDirection, 0);
  analogWrite(leftSpeed, speed);
}
void moveBackward(int speed, double time){
  moveBackward(speed);
  delay(time);
  stop();
  }
void moveBackwardFullSpeed(double time){
    moveBackward(255, time);
}

void turnLeft(int speed){
  digitalWrite(rightDirection, 0);
  analogWrite(rightSpeed, speed);
  digitalWrite(leftDirection, 1);
  analogWrite(leftSpeed, speed);
}
void turnLeft(int speedLeft, double time){
  turnLeft(speedLeft);
  delay(time);
  stop();
  }
void turnRight(int speed){
  digitalWrite(rightDirection, 1);
  analogWrite(rightSpeed, speed);
  digitalWrite(leftDirection, 0);
  analogWrite(leftSpeed, speed);
}
void turnRight(int speed, double time){
  turnRight(speed);
  delay(time);
  stop();
  }
