#pragma once

#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

class WebInterface {
private:
    AsyncWebServer& server;

public:
    WebInterface(AsyncWebServer& webServer);
    void start();
};
