#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include <LittleFS.h>

class ConfigPortal {
private:
    WebServer& server;
    DNSServer dnsServer;
    Preferences& preferences;
    const char* PREFERENCE_NAMESPACE;
    const char* AP_SSID;
    bool isActive;

    void setupWebRoutes();

public:
    ConfigPortal(WebServer& webServer, Preferences& prefs, const char* prefNamespace = "wifi_config", const char* apSsid = "Set me up");
    void start();
    bool tryConnect();
    bool setupMDNS();
    void process();
    bool isConfigMode() const;
    String getMDNSName();
};
