#include "lighting.h"

LightingState lightingCurrentState;
LightingState lightingPreviousState;

uint32_t motionSensorTimer = 0;
float illuminationPrecent;
uint32_t motionDetectCounter = 0;

uint32_t securedStateTimer = 0;
uint32_t autoStateTimer = 0;

bool isNotificationSent = false;

uint32_t stateTimer = 0;


void initLighting(void) {
  pinMode(PIN_LIGHT, OUTPUT);
  pinMode(PIN_PHOTORESISTOR, INPUT);
  disableLight();
  lightingCurrentState = DEFAULT_LIGHTING_STATE;
  lightingPreviousState = DEFAULT_LIGHTING_STATE;
  illuminationPrecent = DEFAULT_ILLUMINATION;
}

void handleLighting(void) {
  
  // Counts securedState and autoState timers with resolution of 1s
  if((millis() - stateTimer) >= SECONDS_TO_MILLIS(STATE_TIMER_PERIOD_SECONDS)){
    if (lightingCurrentState == LIGHTING_STATE_AUTO) {
      autoStateTimer = autoStateTimer + STATE_TIMER_PERIOD_SECONDS; 
    }
    else if (lightingCurrentState == LIGHTING_STATE_SECURE) {
      securedStateTimer = securedStateTimer + STATE_TIMER_PERIOD_SECONDS;
    }
    stateTimer = millis();
  }

  if (isControlLoopTimerExpired()){
    illuminationPrecent = readIllumination(); 
  }

  if (lightingCurrentState == LIGHTING_STATE_AUTO) {
    illuminationPrecent = readIllumination();
    if (illuminationPrecent < ILLUMINATION_TRESHOLD) {
      enableLight();
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
      if (!isNotificationSent) {
        sendMail();
        motionDetectCounter++;
        isNotificationSent = true;
      }
    }
    
    // If there is no motion next 10s, light off
    if ((millis() - motionSensorTimer) >= MOTION_DETECTED_PERIOD) {
      if (!motionDetected()) { // If no motion is detected after 10s 
        disableLight();
        isNotificationSent = false;
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
  digitalWrite(PIN_LIGHT, LOW);
}

void disableLight(void) {
  digitalWrite(PIN_LIGHT, HIGH);
}

float getIlluminationPercent(void) {
  return illuminationPrecent;
}

uint32_t getMotionDetectCounter(void) {
  return motionDetectCounter;
}

uint32_t getSecuredStateTimer(void) {
  return securedStateTimer;
}

uint32_t getAutoStateTimer(void) {
  return autoStateTimer;
}