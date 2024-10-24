#include "serial.h"

uint32_t serialTimer = 0;
void handleSerial(void) {
  sendCommandToSerial();
   if (isControlLoopTimerExpired()){
    serialTimer = millis();
    sendStatus();
   }
}

void sendStatus(void) {
  float temperature = getCurrentTemperature();
  float illumination = getIlluminationPercent();
  uint32_t motionDetectCounter = getMotionDetectCounter();
  uint32_t securedStateTimer = getSecuredStateTimer();
  uint32_t autoStateTimer = getAutoStateTimer();

  Serial.println("D," + String(temperature) + ',' + String(illumination) + ',' + String(motionDetectCounter) +
    ',' + String(securedStateTimer) + ',' + String(autoStateTimer));
}

void sendMail(void) {
  Serial.println('M');
}

void sendCommandToSerial(void) {
  String msg;
  if (Serial.available() > 0) {
    msg = Serial.readString();
    if (msg == "LIGHT ON") {
      switchLightingState(LIGHTING_STATE_ON);
    }
    else if (msg == "LIGHT OFF") {
      switchLightingState(LIGHTING_STATE_OFF);
    }
    else if (msg == "LIGHT AUTO") {
      switchLightingState(LIGHTING_STATE_AUTO);
    }
    else if (msg == "LIGHT SECURE") {
      switchLightingState(LIGHTING_STATE_SECURE);
    }

    else if (msg == "HVAC ON") {
      switchHvacState(HVAC_STATE_ON);
    }
    else if (msg == "HVAC OFF") {
      switchHvacState(HVAC_STATE_OFF);
    }
    else if (msg == "COOLING") {
      switchHvacState(HVAC_STATE_ONLY_COOLING);
    }
    else if (msg == "HEATING") {
      switchHvacState(HVAC_STATE_ONLY_HEATING);
    }
  }
}