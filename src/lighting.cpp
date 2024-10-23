#include "lighting.h"
#include "util.h"
#include "security.h"

LightingState lightingCurrentState;
LightingState lightingPreviousState;

uint32_t controlLoopTimerLighting = 0; 
uint32_t motionSensorTimer = 0;
float illuminationPrecent;

void initLighting(void) {
  pinMode(PIN_LIGHT, OUTPUT);
  pinMode(PIN_PHOTORESISTOR, INPUT);
  disableLight();
  lightingCurrentState = DEFAULT_LIGHTING_STATE;
  lightingPreviousState = DEFAULT_LIGHTING_STATE;
}

void handleLighting(void) {
  
    // Send illumination to serial
  if ((millis() - controlLoopTimerLighting) >= SECONDS_TO_MILLIS(CONTROL_LOOP_PERIOD_SECONDS)){
    illuminationPrecent = readIllumination(); 
    // logWithTimestamp("Iluminaiton: " + String(illuminationPrecent) + "%");
    Serial.println(illuminationPrecent);
    controlLoopTimerLighting = millis();
  }

  if (lightingCurrentState == LIGHTING_STATE_AUTO) {

    if (illuminationPrecent < ILLUMINATION_TRESHOLD) {
      enableLight();
      // logWithTimestamp("Light ON: Illumination < 30%");
    } else {
      disableLight();
    }
  }
  

  else if (lightingCurrentState == LIGHTING_STATE_OFF) {
    disableLight();
  }

  else if (lightingCurrentState == LIGHTING_STATE_ON) {
    enableLight();
  }
  

  else if (lightingCurrentState == LIGHTING_STATE_SECURE) {

    if(motionDetected()) {
      enableLight();
      motionSensorTimer = millis();
    }
    
    // If there is no motion next 10s, light off
    if ((millis() - motionSensorTimer) >= 10000) {
      if (!motionDetected()) { // If no motion is detected after 10s 
        disableLight();
      } else {
        // Reset timer if motion is detected again
        motionSensorTimer = millis();
      }
    }
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

  return illuminationPrecent;
}

void enableLight(void) {
  digitalWrite(PIN_LIGHT, HIGH);
}

void disableLight(void) {
  digitalWrite(PIN_LIGHT, LOW);
}