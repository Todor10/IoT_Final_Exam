#include "lighting.h"
#include "util.h"
#include "security.h"
#include "serial.h"

LightingState lightingCurrentState;
LightingState lightingPreviousState;

uint32_t controlLoopTimerLighting = 0; 
uint32_t motionSensorTimer = 0;
float illuminationPrecent;
uint32_t motionDetectCounter = 0;

uint32_t securedStateTimer = 0;
uint32_t autoStateTimer = 0;

bool isNotificationSent = false;


void initLighting(void) {
  pinMode(PIN_LIGHT, OUTPUT);
  pinMode(PIN_PHOTORESISTOR, INPUT);
  disableLight();
  lightingCurrentState = DEFAULT_LIGHTING_STATE;
  lightingPreviousState = DEFAULT_LIGHTING_STATE;
  illuminationPrecent = DEFAULT_ILLUMINATION;
}

void handleLighting(void) {
  
    // Send illumination to serial
  if (isControlLoopTimerExpired()){
    illuminationPrecent = readIllumination(); 
    // logWithTimestamp("Iluminaiton: " + String(illuminationPrecent) + "%");
    // Serial.println(illuminationPrecent); // float getIlluminationPrecent
    controlLoopTimerLighting = millis();

    if (lightingCurrentState == LIGHTING_STATE_AUTO) {
      autoStateTimer = autoStateTimer + CONTROL_LOOP_PERIOD_MINUTES; 
    }
    else if (lightingCurrentState == LIGHTING_STATE_SECURE) {
      securedStateTimer = securedStateTimer + CONTROL_LOOP_PERIOD_MINUTES;
    }
  }

  if (lightingCurrentState == LIGHTING_STATE_AUTO) {

    illuminationPrecent = readIllumination();
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
      if (!isNotificationSent) {
        sendMail();
        motionDetectCounter++;
        isNotificationSent = true;
      }
    }
    
    // If there is no motion next 10s, light off
    if ((millis() - motionSensorTimer) >= 10000) {
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
  digitalWrite(PIN_LIGHT, HIGH);
}

void disableLight(void) {
  digitalWrite(PIN_LIGHT, LOW);
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