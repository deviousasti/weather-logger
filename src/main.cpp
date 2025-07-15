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
#include "sensor.h"

Preferences preferences;
WebServer server(80);
AsyncWebServer asyncServer(80);  // Can use same port as config portal since they won't run simultaneously
ConfigPortal configPortal(server, preferences);
WebInterface webInterface(asyncServer);
Sensor sensor;

unsigned long lastSensorRead = 0;
const unsigned long SENSOR_READ_INTERVAL = 2000; // Read every 2 seconds

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
    sensor.begin();
}

void loop() {
    if (configPortal.isConfigMode()) {
        configPortal.process();
        return;
    }

    // Read sensors periodically
    unsigned long currentMillis = millis();
    if (currentMillis - lastSensorRead >= SENSOR_READ_INTERVAL) {
        lastSensorRead = currentMillis;

        // Read all sensors and send events
        for (int i = 0; i < sensor.sensorCount(); i++) {
            float value = sensor.readValue(i);
            const char* name = sensor.sensorName(i);
            webInterface.sendEvent(name, value);
            debugPrint(String(name) + ": " + String(value));
        }
    }
}
