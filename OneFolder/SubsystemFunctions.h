
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

void setLeftForwardSpeed(int speed);
void setRightBackwardSpeed(int speed);
void setLeftBackwardSpeed(int speed);
void setRightForwardSpeed(int speed);

//@Servo
//everything can be done with myservo.write() :: takes in the amount of degrees you want


//@ColorSensor
void colorOpen();
uint16_t * colorSet();

//@InfraSensor
void infraOn();
int infraGrab(int receivePort);

//@UltraSensor
void ultraOn();
double ultraGrab(int sendPort, int receivePort);

#endif
