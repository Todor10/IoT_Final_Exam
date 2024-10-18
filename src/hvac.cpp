#include "hvac.h"
#include "util.h"

uint32_t controlLoopTimer = 0; 
float currentTemperature;

void initHvac() {
  currentTemperature = DEFAULT_TEMPERATURE;

  pinMode(PIN_COOLER, OUTPUT);
  pinMode(PIN_HEATER, OUTPUT);
  pinMode(PIN_TEMPERATURE_SENSOR, INPUT);
}

void handleHvac() {
  if ((millis() - controlLoopTimer) >= SECONDS_TO_MILLIS(CONTROL_LOOP_PERIOD_SECONDS)) {
    // Da mozemo vise puta da proveravamo
    controlLoopTimer = millis();

    currentTemperature = readTemperature();
    logWithTimestamp("Temp: " + String(currentTemperature) + "C");
    if (currentTemperature > COOLING_THRESHOLD) {
      logWithTimestamp("Cooling enabled");

      disableHeating();
      enableCooling();
    } else if (currentTemperature <= HEATING_THRESHOLD) {
      logWithTimestamp("Heating enabled");

      disableCooling();
      enableHeating();
    } 
  }
}

void enableCooling(void) {
  digitalWrite(PIN_COOLER, HIGH);
}

void disableCooling(void) {
  digitalWrite(PIN_COOLER, LOW);
}

void enableHeating(void) {
  digitalWrite(PIN_HEATER, HIGH);
}

void disableHeating(void){
  digitalWrite(PIN_HEATER, LOW);
}

float readTemperature(void) {
  float temp = analogRead(PIN_TEMPERATURE_SENSOR) * ADC_TO_TEMP;
  return temp;
}