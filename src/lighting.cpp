#include "lighting.h"
#include "util.h"

LightingState lightingCurrentState;
LightingState lightingPreviousState;

uint32_t controlLoopTimerLighting = 0; 

void initLighting(void) {
  disableLight();
  lightingCurrentState = DEFAULT_LIGHTING_STATE;
  lightingPreviousState = DEFAULT_LIGHTING_STATE;
}

void handleLighting(void) {
  
  if (lightingCurrentState == LIGHTING_STATE_AUTO) {
    if ((millis() - controlLoopTimerLighting) >= SECONDS_TO_MILLIS(CONTROL_LOOP_PERIOD_SECONDS)){
    controlLoopTimerLighting = millis();

    if (readIllumination() < ILLUMINATION_TRESHOLD) {
      enableLight();
      // logWithTimestamp("Light ON: Illumination < 30%");
    } else {
      disableLight();
      }
    }
  }

  else if (lightingCurrentState == LIGHTING_STATE_OFF) {
    disableLight();
  }

  else if (lightingCurrentState == LIGHTING_STATE_ON) {
    enableLight();

  }

  else if (lightingCurrentState == LIGHTING_STATE_SECURE) {

  }
}

void switchLightingState(LightingState state) {
  lightingPreviousState = lightingCurrentState;
  lightingCurrentState = state;
}

float readIllumination(void) {
  float illuminationPrecent = 0;
  int sensorValue = 0;

  sensorValue = analogRead(PIN_PHOTORESISTOR);

  illuminationPrecent = (sensorValue / ANALOG_VALUE_RANGE) * 100.0;

  // Debugging
  // logWithTimestamp("Iluminaiton: " + String(illuminationPrecent) + "%");
  Serial.println(illuminationPrecent);

  return illuminationPrecent;
}

void enableLight(void) {
  digitalWrite(PIN_LIGHT, HIGH);
}

void disableLight(void) {
  digitalWrite(PIN_LIGHT, LOW);
}