#ifndef LIGHT_H
#define LIGHT_H

#include "util.h"
#include "security.h"
#include "serial.h"

#include <Arduino.h>

// Pins
#define PIN_LIGHT 6
#define PIN_PHOTORESISTOR A1

#define ANALOG_VALUE_RANGE 1023.0

#define ILLUMINATION_TRESHOLD 30.0

#define DEFAULT_LIGHTING_STATE LIGHTING_STATE_AUTO
#define DEFAULT_ILLUMINATION 100.0

#define STATE_TIMER_PERIOD 1000

/**
 * @brief states used for lighting state machine
 * 
 */
typedef enum {
  LIGHTING_STATE_AUTO,
  LIGHTING_STATE_OFF,
  LIGHTING_STATE_ON,
  LIGHTING_STATE_SECURE
} LightingState;

/**
 * @brief initialization of lighting. Code that will be inside setup()
 * 
 */
void initLighting(void);
/**
 * @brief code that will be inside loop()
 * 
 */
void handleLighting(void);

/**
 * @brief switching the state of the lighting state machine
 * 
 * @param state: desired state of the state machine. 
 */
void switchLightingState(LightingState state);

/**
 * @brief turns on the LED
 * 
 */
void enableLight(void);
/**
 * @brief turns off the LED
 * 
 */
void disableLight(void);

/**
 * @brief reads illumination from the phototransistor
 * 
 * @return float: illumination percent (0 - 100)
 */
float readIllumination(void);

/**
 * @brief Get the Illumination Percent object
 * 
 * @return float: illuminationPrecent
 */
float getIlluminationPercent(void);

/**
 * @brief Get the Motion Detect Counter object
 * 
 * @return uint32_t: motionDetectCounter
 */
uint32_t getMotionDetectCounter(void);

/**
 * @brief Get the Secured State Timer object
 * 
 * @return uint32_t: securedStateTimer
 */
uint32_t getSecuredStateTimer(void);

/**
 * @brief Get the Auto State Timer object
 * 
 * @return uint32_t: autoStateTimer
 */
uint32_t getAutoStateTimer(void);

#endif // LIGHT_H

