#include "hvac.h"
#include "util.h"

uint32_t controlLoopTimerHvac = 0; 
float currentTemperature;
TemperatureControlState hvacCurrentState;
TemperatureControlState hvacPreviousState;

void initHvac() {
  currentTemperature = DEFAULT_TEMPERATURE;
  hvacCurrentState = DEFAULT_TEMPERATURE_CONTROL_STATE;
  hvacPreviousState = DEFAULT_TEMPERATURE_CONTROL_STATE;

  pinMode(PIN_COOLER, OUTPUT);
  pinMode(PIN_HEATER, OUTPUT);
  pinMode(PIN_TEMPERATURE_SENSOR, INPUT);
}

void handleHvac() {


  if ((millis() - controlLoopTimerHvac) >= SECONDS_TO_MILLIS(CONTROL_LOOP_PERIOD_SECONDS)) {

    if (hvacCurrentState == TEMPERATURE_CONTROL_ON) {
      // Da mozemo vise puta da proveravamo
      controlLoopTimerHvac = millis();

      currentTemperature = readTemperature();
      // logWithTimestamp("Temp: " + String(currentTemperature) + "C");
      Serial.println(currentTemperature);
      if (currentTemperature > COOLING_THRESHOLD) {
        // logWithTimestamp("Cooling enabled");

        disableHeating();
        enableCooling();
      } else if (currentTemperature <= HEATING_THRESHOLD) {
        // logWithTimestamp("Heating enabled");

        disableCooling();
        enableHeating();
      }  
    }

    else if (hvacCurrentState == TEMPERATURE_CONTROL_OFF) {
      disableCooling();
      disableHeating();
      // Da mozemo vise puta da proveravamo
      controlLoopTimerHvac = millis();

      currentTemperature = readTemperature();
      // logWithTimestamp("Temp: " + String(currentTemperature) + "C");
      Serial.println(currentTemperature);
    }
  }
}

void switchTemperatureControlState(TemperatureControlState state) {
  hvacPreviousState = hvacCurrentState;
  hvacCurrentState = state;
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