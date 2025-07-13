#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>

// Debug print function
inline void debugPrint(const String& message, bool newline = true) {
  if (newline) {
    Serial.println(message);
  } else {
    Serial.print(message);
  }
}

#endif // DEBUG_H