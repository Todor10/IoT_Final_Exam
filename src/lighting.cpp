#include "lighting.h"
#include "util.h"

LightingState currentState;
LightingState previousState;

uint32_t controlLoopTimer2 = 0; 

void initLighting(void) {
  disableLight();
  currentState = DEFAULT_LIGHTING_STATE;
  previousState = DEFAULT_LIGHTING_STATE;
}

void handleLighting(void) {
  
  if (currentState == LIGHTING_STATE_AUTO) {
    if ((millis() - controlLoopTimer2) >= SECONDS_TO_MILLIS(CONTROL_LOOP_PERIOD_SECONDS)){
    controlLoopTimer2 = millis();

    if (readIllumination() < ILLUMINATION_TRESHOLD) {
      enableLight();
      logWithTimestamp("Light ON: Illumination < 30%");
    } else {
      disableLight();
      }
    }
  }

  else if (currentState == LIGHTING_STATE_OFF) {
    disableLight();
    // logWithTimestamp("Light OFF");
  }

  else if (currentState == LIGHTING_STATE_ON) {
    enableLight();
    // logWithTimestamp("Light ON");

  }

  else if (currentState == LIGHTING_STATE_SECURE) {

  }
}

void switchLightingState(LightingState state) {
  previousState = currentState;
  currentState = state;
}

float readIllumination(void) {
  float illuminationPrecent = 0;
  int sensorValue = 0;

  sensorValue = analogRead(PIN_PHOTORESISTOR);

  illuminationPrecent = (sensorValue / ANALOG_VALUE_RANGE) * 100.0;

  // Debugging
  logWithTimestamp("Iluminaiton: " + String(illuminationPrecent) + "%");

  return illuminationPrecent;
}

void enableLight(void) {
  digitalWrite(PIN_LIGHT, HIGH);
}

void disableLight(void) {
  digitalWrite(PIN_LIGHT, LOW);
}