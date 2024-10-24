#ifndef UTIL_H
#define UTIL_H

#include <Arduino.h>

#define CONTROL_LOOP_PERIOD_MINUTES 10
#define MINUTES_TO_MILLIS(minutes) minutes * 1000UL * 60UL

/**
 * @brief logging to Serial with timestamp, for debuging code
 * 
 * @param s: string to be logged 
 */
void logWithTimestamp(String s);

/**
 * @brief mesaurements are sent every x minutes to serial. If x minutes are expired,
 * 'controlLoopTimer' is reset and 'timerExpired' is sent to true.
 * Else, 'timerExpired' is set to false.
 * 
 */
void handleControlLoopTimer(void);

/**
 * @brief checks if timer for sending mesaurements is expired.
 * 
 * @return true: timer is expired 
 * @return false: timer is not expired
 */
bool isControlLoopTimerExpired(void);

#endif // UTIL_H
