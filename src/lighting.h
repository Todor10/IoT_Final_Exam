#ifndef LIGHT_H
#define LIGHT_H

#include <Arduino.h>

#define DEFAULT_LIGHTING_STATE LIGHTING_STATE_OFF

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

