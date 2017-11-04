#ifndef SubsystemFunctions_h
#define SubsystemFunctions_h

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
void turnLeft(int speed, int degrees);
void turnRight(int speed, int degrees);

void clawSetOpen();
void clawSetClose();
void clawSetDegrees();

void colorOpen();
void colorGrab();

void colorReadRed();
void colorReadGreen();
void colorReadBlue();
void colorReadLux();
void colorReadColorTemp();

void infraOn();
void infraGrab();

void ultraOn();
void ultraRead();



#endif
