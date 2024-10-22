#include "serial.h"
#include "lighting.h"
#include "hvac.h"

void handleSerial(void) {
  sendCommandToSerial();
}


void sendCommandToSerial(void) {
  String msg;
  if (Serial.available() > 0) {
    msg = Serial.readString();
    if (msg == "LIGHT ON") {
      // IZMENI
      // Serial.println("Hello ON");
      switchLightingState(LIGHTING_STATE_ON);
    }
    else if (msg == "LIGHT OFF") {
      // IZMENI
      // Serial.println("Hello OFF");
      switchLightingState(LIGHTING_STATE_OFF);
    }
    else if (msg == "LIGHT AUTO") {
      // IZMENI
      // Serial.println("Hello AUTO");
      switchLightingState(LIGHTING_STATE_AUTO);
    }
    else if (msg == "LIGHT SECURE") {
      // IZMENI
      // Serial.println("Hello SECURE");
      switchLightingState(LIGHTING_STATE_SECURE);
    }

    else if (msg == "TEMPERATURE CONTROL ON") {
      switchTemperatureControlState(TEMPERATURE_CONTROL_ON);
    }
    else if (msg == "TEMPERATURE CONTROL OF") {
      switchTemperatureControlState(TEMPERATURE_CONTROL_OFF);
    }
  }
}