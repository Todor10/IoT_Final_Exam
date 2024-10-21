#include <Arduino.h>
#include "hvac.h"
#include "serial.h"
#include "lighting.h"

void setup() {
  Serial.begin(9600);
  initHvac();
  initLighting();
}

void loop() {
  handleSerial();
  handleHvac();
  handleLighting();
}

