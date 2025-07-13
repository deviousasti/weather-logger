
#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include "debug.h"

namespace ntp {

  static const char* ntpServer1 = "0.in.pool.ntp.org";
  static const char* ntpServer2 = "1.in.pool.ntp.org";
  static const char* ntpServer3 = "2.in.pool.ntp.org";
  static const long  gmtOffset_sec = 5.5 * 3600;
  static const int   daylightOffset_sec = 0;

  static void printLocalTime()
  {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
      debugPrint("Failed to obtain time");
      return;
    }
    char buf[64];
    strftime(buf, sizeof(buf), "%A, %B %d %Y %H:%M:%S", &timeinfo);
    debugPrint(buf);
  }

  static void setupNTP() {
    debugPrint("Configuring time with NTP servers:");
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2, ntpServer3);
    printLocalTime();
  }
}
