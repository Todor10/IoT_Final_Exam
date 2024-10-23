#include "util.h"

uint32_t controlLoopTimer = 0;
bool timerExpired = false;

void logWithTimestamp(String s) {
  Serial.println(String(millis()) + " - " + s);
}

void handleControlLoopTimer(void) {
  if ((millis() - controlLoopTimer) >= MINUTES_TO_MILLIS(CONTROL_LOOP_PERIOD_MINUTES)){
    controlLoopTimer = millis();
    timerExpired = true;
  }
  else{
    timerExpired = false;
  }
}

bool isControlLoopTimerExpired(void) {
  return timerExpired;
}

