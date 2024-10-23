#include <Arduino.h>
#include "hvac.h"
#include "serial.h"
#include "lighting.h"
#include "security.h"

void setup() {
  Serial.begin(9600);
  initHvac();
  initSecurity();
  initLighting();
}

void loop() {
  handleSerial();
  handleHvac();
  handleLighting();
}

