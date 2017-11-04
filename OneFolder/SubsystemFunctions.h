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


void ClawOn();
void clawSetOpen();
void clawSetClose();
void clawSetDegrees();

void ColorOpen();
uint16_t[] colorSet();

void InfraOn();
void infraGrab();

void UltraOn();
double ultraRead();



#endif
