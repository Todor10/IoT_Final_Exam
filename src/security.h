#ifndef SECURITY_H
#define SECURITY_H

#include <Arduino.h>

// Pins
#define PIN_MOTION_SENSOR 7

#define MOTION_DETECTED_PERIOD 10000 // 10s

/**
 * @brief initialization of security. Code that will be inside setup()
 * 
 */
void initSecurity(void);

/**
 * @brief code that will be inside loop()
 * 
 */
void handleSecurity(void);

/**
 * @brief reads input from the HC-SR501 – PIR motion detector sensor
 * 
 * @return int: 5V or 0V (HIGH or LOW) 
 */
int readMotionSensor(void);

/**
 * @brief Checks if motion is detected
 * 
 * @return true: motion is detected 
 * @return false: motion is not detected
 */
bool motionDetected(void);

#endif // SECURITY_H
