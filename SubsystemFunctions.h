#ifndef SubsystemFunctions_h
#define SubsystemFunctions_h

void moveForward(int speed);
void moveForward(int speed, double time);
void moveForwardFullSpeed(double time);

void moveBackward(int speed);
void moveBackward(int speed, double time);
void moveBackwardFullSpeed(double time);

void turnLeft(int speed);
void turnRight(int speed);

void turnLeft(int speedLeft, double time);
void turnRight(int speedRight, double time);
void turnTime(int speed, double time);

void turnLeft(int speedLeft, int degrees);
void turnRight(int speedRight, int degrees);
void turnDegrees(int speed, int degrees);

void setLeftMotorSpeed(int speedLeft, double time);
void setLeftMotorSpeed(int speedRight, double time);
void setSpeed(int speed, double time);

void stop();

void clawSetOpen();
void clawSetClose();
void clawSetDegrees();

void colorOpen();
void colorClose();
void colorGrab();

void infraOn();
void infraOff();
void infraGrab();

void ultraOn();
void ultraOff();
void ultraRead();

#endif
