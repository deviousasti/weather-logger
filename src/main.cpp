#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>
#include <WebServer.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include "config_portal.h"
#include "web_server.h"
#include "ntp.h"
#include "debug.h"

Preferences preferences;
WebServer server(80);
AsyncWebServer asyncServer(80);  // Can use same port as config portal since they won't run simultaneously
ConfigPortal configPortal(server, preferences);
WebInterface webInterface(asyncServer);

void setup() {
    Serial.begin(9600);
    debugPrint("Starting up...");

    if (!LittleFS.begin(true)) {
        debugPrint("An error has occurred while mounting LittleFS");
        return;
    }

    if (!configPortal.tryConnect()) {
        configPortal.start();
        return;
    }
    
    if (!configPortal.setupMDNS()) {
        debugPrint("Failed to setup mDNS, continuing anyway");
    }
    
    webInterface.start();
}

void loop() {
    if (configPortal.isConfigMode()) {
        configPortal.process();
    }
}
