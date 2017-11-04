#include <Arduino.h>
#include "SubsystemFunctions.h"
#include "Main\ArduinoPorts.h"

// defines variables
long duration;
double distance;

void infraOn(){
    pinMode(infraSendPort, OUTPUT);
    pinMode(infraReceivePort, INPUT);
    Serial.begin(9600);
}

 double infraGrab(){
    // Clears the trigPin
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
        duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
        distance= duration*0.017; // 0.017 - speed of sound in millisecs
        return distance;
}





