#include <Wire.h>
#include <Arduino.h>
#include <Servo.h>
#include "Adafruit_TCS34725.h"
#include "ArduinoPorts.h"
#include "SubsystemFunctions.h"

//@Infrared
void infraOn() {
    /*
    pinMode(infraLeftSendPort, OUTPUT);
    pinMode(infraLeftReceivePort, INPUT);
    pinMode(infraRightSendPort, OUTPUT);
    pinMode(infraRightReceivePort, INPUT);
    */
}

int infraGrab(int receivePort) {
     // 5v
  float volts = analogRead(receivePort)*0.0048828125;  // value from sensor * (5/1024)
  int distance = 13/volts; // worked out from datasheet graph
  delay(1000); // slow down serial port 
  
  if (distance <= 30){
   return distance;   // print the distance
  }
}

int infraGrab() { //change this
    return 0;
}


//@Ultrasonic:

// defines variables
long duration;
double distance;

void ultraOn() {
    pinMode(ultraTopSendPort, OUTPUT);
    pinMode(ultraTopReceivePort, INPUT);
    pinMode(ultraRightSendPort, OUTPUT);
    pinMode(ultraRightReceivePort,INPUT); 
    pinMode(ultraLeftSendPort, OUTPUT);
    pinMode(ultraLeftReceivePort,INPUT);
    pinMode(ultraBotSendPort, OUTPUT);
    pinMode(ultraBotReceivePort,INPUT);
}

double ultraGrab(int sendPort, int receivePort) {
    // Clears the trigPin
        digitalWrite(sendPort, LOW);
        delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
        digitalWrite(sendPort, HIGH);
        delayMicroseconds(10);
        digitalWrite(sendPort, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
        duration = pulseIn(receivePort, HIGH);
    // Calculating the distance
        distance = duration*0.017; // 0.017 - speed of sound in millisecs
        return distance;
}



//@Servo:
void setServo(int degrees){
  // (0)- 0 degrees, (1)- 180 degrees
  double microsecs = ((degrees/180) + 1) * 1000;
  
  digitalWrite(clawPort, HIGH);
  delayMicroseconds(microsecs);
  digitalWrite(clawPort, HIGH);
  delayMicroseconds(20000-microsecs);
  }
//everything can be done with myservo.write() :: takes in the amount of degrees you want



//@RobotDrive:

void stop() {
    analogWrite(rightSpeed, 0);
    analogWrite(leftSpeed, 0);
}

void moveForward(int speed) {
  digitalWrite(rightDirection, 1);
  analogWrite(rightSpeed, speed);

  digitalWrite(leftDirection, 1);
  analogWrite(leftSpeed, speed);
}

void moveForward(int speed, double time) { 
  moveForward(speed);
  delay(time);
  stop();
}

void moveForwardFullSpeed(double time) {
    moveForward(255, time);
}

void moveBackward(int speed) {
  digitalWrite(rightDirection, 0);
  analogWrite(rightSpeed, speed);
  digitalWrite(leftDirection, 0);
  analogWrite(leftSpeed, speed);
}

void moveBackward(int speed, double time) {
  moveBackward(speed);
  delay(time);
  stop();
}

void moveBackwardFullSpeed(double time) {
    moveBackward(255, time);
}

void turnLeft(int speed) {
    digitalWrite(rightDirection, 0);
    analogWrite(rightSpeed, speed);
    digitalWrite(leftDirection, 1);
    analogWrite(leftSpeed, speed);
}

void turnLeft(int speedLeft, double time) {
    turnLeft(speedLeft);
    delay(time);
    stop();
}

void turnRight(int speed) {
    digitalWrite(rightDirection, 1);
    analogWrite(rightSpeed, speed);
    digitalWrite(leftDirection, 0);
    analogWrite(leftSpeed, speed);
}

void turnRight(int speed, double time) {
    turnRight(speed);
    delay(time);
    stop();
}

void setLeftForwardSpeed(int speed) {
    digitalWrite(leftDirection, 1);
    analogWrite(leftSpeed, speed);
}

void setLeftBackwardSpeed(int speed) {
    digitalWrite(leftDirection, 0);
    analogWrite(leftSpeed, speed);
}

void setRightForwardSpeed(int speed) {
    digitalWrite(rightDirection, 1);
    analogWrite(rightSpeed, speed);
}

void setRightBackwardSpeed(int speed) {
    digitalWrite(rightDirection, 0);
    analogWrite(rightSpeed, speed);
}


//@ColorSensor:

// Initialise with default values (int time = 2.4ms, gain = 1x)
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();

// Initialise with specific int time and gain values
Adafruit_TCS34725 ColorSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);
uint16_t Color[6] = {0,0,0,0,0,0};

void colorOpen() {
    /*
    while(ColorSensor.begin() == false) {
        Serial.print("Color Sensor Not Found");
        if (ColorSensor.begin()) {
            break;
        }
    }
    */
    // Now we're ready to get readings!
}

uint16_t * colorSet() {
    ColorSensor.getRawData(&Color[0], &Color[1], &Color[2], &Color[3]);

    Color[4] = ColorSensor.calculateColorTemperature(Color[0], Color[1], Color[2]);
    Color[5] = ColorSensor.calculateLux(Color[0], Color[1], Color[2]);
    /*
    Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
    Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
    Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
    Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
    Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");

    Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
    Serial.println(" ");
    */
    return Color;
}


