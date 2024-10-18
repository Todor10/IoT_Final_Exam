#ifndef UTIL_H
#define UTIL_H

#include <Arduino.h>

#define CONTROL_LOOP_PERIOD_SECONDS 5
#define SECONDS_TO_MILLIS(seconds) seconds * 1000


void logWithTimestamp(String s);

#endif // UTIL_H
