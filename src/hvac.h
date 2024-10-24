// heating ventilation and cooling
#ifndef HVAC_H // Ako nije definsano, definisi https://www.geeksforgeeks.org/include-guards-in-c/
#define HVAC_H

#include "util.h"

#include <Arduino.h>

// Pins
#define PIN_HEATER 4
#define PIN_COOLER 5
#define PIN_TEMPERATURE_SENSOR A0

// (5.0 / 1023) * 100
#define ADC_TO_TEMP 0.488f

#define HEATING_THRESHOLD 17
#define COOLING_THRESHOLD 23

#define DEFAULT_HVAC_STATE HVAC_STATE_ON
#define DEFAULT_TEMPERATURE 0

/**
 * @brief states used for hvac state machines
 * 
 */
typedef enum {
  HVAC_STATE_ON,
  HVAC_STATE_OFF,
  HVAC_STATE_ONLY_HEATING,
  HVAC_STATE_ONLY_COOLING
} HvacState;

/**
 * @brief initialization of hvac. Code that will be inside setup()
 * 
 */
void initHvac(void);
/**
 * @brief code that will be inside loop()
 * 
 */
void handleHvac(void);

/**
 * @brief switching the state of the hvac state machine
 * 
 * @param state desired state of the state machine
 */
void switchHvacState(HvacState state);

/**
 * @brief turns on DC motor
 * 
 */
void enableCooling(void);
/**
 * @brief turns of DC motor
 * 
 */
void disableCooling(void);

/**
 * @brief heats up the resistor (heater)
 * 
 */
void enableHeating(void);
/**
 * @brief turns off current in the resistor (heater)
 * 
 */
void disableHeating(void);

/**
 * @brief reads temperature from the LM35 temperature sensor
 * 
 * @return float: temperature in celsius
 */
float readTemperature(void);
/**
 * @brief Get the Current Temperature object
 * 
 * @return float: currentTemperature
 */
float getCurrentTemperature(void);

#endif // HVAC_H