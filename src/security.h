#ifndef SECURITY_H
#define SECURITY_H

#include <Arduino.h>

// Pins
#define PIN_MOTION_SENSOR 7

// void enableMotionSensor(void);
// void disableMotionSensor(void);
int readMotionSensor(void);

bool motionDetected(void);

void initSecurity(void);
void handleSecurity(void);


#endif // SECURITY_H
