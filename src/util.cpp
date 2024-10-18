#include "util.h"

void logWithTimestamp(String s) {
  Serial.println(String(millis()) + " - " + s);
}