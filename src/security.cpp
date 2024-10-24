#include "security.h"

int readMotionSensor(void) {
  return digitalRead(PIN_MOTION_SENSOR);
}

bool motionDetected(void) {
  if (readMotionSensor() == HIGH) {
    return true;
  } else {
    return false;
  }
}

void initSecurity(void) {
  pinMode(PIN_MOTION_SENSOR, INPUT);
}

void handleSecurity(void) {
  motionDetected();
}
