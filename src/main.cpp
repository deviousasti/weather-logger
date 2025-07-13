#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>
#include <WebServer.h>
#include <LittleFS.h>
#include "config_portal.h"
#include "ntp.h"
#include "debug.h"

// Global objects
Preferences preferences;
WebServer server(80);
ConfigPortal* configPortal = nullptr;

void setup() {
    Serial.begin(9600);
    debugPrint("Starting up...");

    if (!LittleFS.begin(true)) {
        debugPrint("An error has occurred while mounting LittleFS");
        return;
    }

    configPortal = new ConfigPortal(server, preferences);
    
    if (!configPortal->tryConnect()) {
        configPortal->start();
    }
}

void loop() {
    if (configPortal) {
        configPortal->process();
    }
}
