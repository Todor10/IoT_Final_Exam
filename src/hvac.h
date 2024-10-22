// heating ventilation and cooling
#ifndef HVAC_H // Ako nije definsano, definisi https://www.geeksforgeeks.org/include-guards-in-c/
#define HVAC_H

#include <Arduino.h>

// Pins
#define PIN_HEATER 4
#define PIN_COOLER 5
#define PIN_TEMPERATURE_SENSOR A0


#define ADC_TO_TEMP 0.488f
#define DEFAULT_TEMPERATURE 0
#define HEATING_THRESHOLD 17 
#define COOLING_THRESHOLD 23

#define DEFAULT_HVAC_STATE HVAC_STATE_ON

typedef enum {
  HVAC_STATE_ON,
  HVAC_STATE_OFF,
  HVAC_STATE_ONLY_HEATING,
  HVAC_STATE_ONLY_COOLING
} HvacState;


void enableCooling(void);
void disableCooling(void);

void enableHeating(void);
void disableHeating(void);

float readTemperature(void);

void initHvac(void);
void handleHvac(void);

void switchHvacState(HvacState state);

#endif // HVAC_H