#ifndef LIGHT_H
#define LIGHT_H

#include <Arduino.h>

// Pins
#define PIN_LIGHT 6
#define PIN_PHOTORESISTOR A1

#define ILLUMINATION_TRESHOLD 30.0
#define ANALOG_VALUE_RANGE 1023.0

#define DEFAULT_LIGHTING_STATE LIGHTING_STATE_SECURE

typedef enum {
  LIGHTING_STATE_AUTO,
  LIGHTING_STATE_OFF,
  LIGHTING_STATE_ON,
  LIGHTING_STATE_SECURE
} LightingState;

void setLightMode(LightingState state);


void enableLight(void);
void disableLight(void);
float readIllumination(void);
void switchLightingState(LightingState state);

void initLighting(void);
void handleLighting(void);

#endif // LIGHT_H

