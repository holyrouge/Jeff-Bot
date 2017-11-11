#include <Arduino.h>
#include "ArduinoPorts.h"
#include <Servo.h>
Servo myservo;

void setup() {
  Serial.begin(9600); 
  pinMode(leftSpeed, OUTPUT);
  pinMode(rightDirection, OUTPUT);
  pinMode(rightSpeed, OUTPUT);
  pinMode(leftDirection, OUTPUT);
  pinMode(13, OUTPUT);
  myservo.attach(clawPort);
  ColorOpen();
  UltraOn();  
}

void loop() {
  moveBackward(50, 3000); //runs for three seconds
  delay(3000); // waits for three seconds before looping :D
}

void LOOP_RC() {
  
}

void LOOP_AUTO() {
  uint16_t * rgb = ColorSet();
  Serial.println(rgb[0]);
  
}


