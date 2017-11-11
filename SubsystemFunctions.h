
#ifndef SubsystemFunctions_h
#define SubsystemFunctions_h

//@RobotDrive
void stop();

void moveForward(int speed);
void moveForward(int speed, double time);
void moveForwardFullSpeed(double time);

void moveBackward(int speed);
void moveBackward(int speed, double time);
void moveBackwardFullSpeed(double time);

void turnLeft(int speed);
void turnRight(int speed);
void turnLeft(int speed, double time);
void turnRight(int speed, double time);

//@Servo
//everything can be done with myservo.write() :: takes in the amount of degrees you want


//@ColorSensor
void ColorOpen();
uint16_t * colorSet();

//@InfraSensor
void InfraOn();
void infraGrab();

//@UltraSensor
void UltraOn();
double ultraGrab(int sendPort, int receivePort);

#endif
