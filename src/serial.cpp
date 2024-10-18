#include "serial.h"
#include "lighting.h"

void handleSerial(void) {
  sendCommandToSerial();
}


void sendCommandToSerial(void) {
  String msg;
  if (Serial.available() > 0) {
    msg = Serial.readString();
    if (msg == "ON") {
      // IZMENI
      Serial.println("Hello ON");
      switchLightingState(LIGHTING_STATE_ON);
    }
    else if (msg == "OFF") {
      // IZMENI
      Serial.println("Hello OFF");
      switchLightingState(LIGHTING_STATE_OFF);
    }
    else if (msg == "AUTO") {
      // IZMENI
      Serial.println("Hello AUTO");
      switchLightingState(LIGHTING_STATE_AUTO);
    }
    else if (msg == "SECURE") {
      // IZMENI
      Serial.println("Hello SECURE");
      switchLightingState(LIGHTING_STATE_SECURE);
    }
  }
}