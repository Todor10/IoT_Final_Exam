#include <Arduino.h>
#include "hvac.h"
#include "serial.h"
#include "lighting.h"
#include "security.h"
#include "util.h"

void setup() {
  Serial.begin(9600);
  initLighting();
  initHvac();
  initSecurity();
}

void loop() {
  handleControlLoopTimer();
  handleLighting();
  handleHvac();
  // Da bi ucitalo sve
  handleSerial();
}

