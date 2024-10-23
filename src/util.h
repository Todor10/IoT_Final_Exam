#ifndef UTIL_H
#define UTIL_H

#include <Arduino.h>

#define CONTROL_LOOP_PERIOD_MINUTES 1
#define MINUTES_TO_MILLIS(minutes) minutes * 1000UL * 60UL


void logWithTimestamp(String s);

void handleControlLoopTimer(void);
bool isControlLoopTimerExpired(void);

#endif // UTIL_H
