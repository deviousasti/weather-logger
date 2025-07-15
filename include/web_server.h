#pragma once

#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

class WebInterface {
private:
    AsyncWebServer& server;
    AsyncEventSource events;

public:
    WebInterface(AsyncWebServer& webServer);
    void start();
    void sendEvent(const char* eventName, float value);
};
