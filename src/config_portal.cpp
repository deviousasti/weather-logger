#include "config_portal.h"
#include "debug.h"

ConfigPortal::ConfigPortal(WebServer& webServer, Preferences& prefs, const char* prefNamespace, const char* apSsid)
    : server(webServer)
    , preferences(prefs)
    , PREFERENCE_NAMESPACE(prefNamespace)
    , AP_SSID(apSsid)
    , isActive(false)
{
}

void ConfigPortal::setupWebRoutes() {
    server.on("/", HTTP_GET, [this]() {
        File file = LittleFS.open("/setup.html");
        server.streamFile(file, "text/html", 200);
    });

    server.on("/clear", HTTP_GET, [this]() {
        preferences.clear();
        server.send(200, "text/html", "Preferences reset! Turn off the device and turn it back on.");
    });

    server.on("/save", HTTP_POST, [this]() {
        String ssid = server.arg("ssid");
        String password = server.arg("password");
        String mdnsName = server.arg("mdnsname");
        
        debugPrint("Received new WiFi credentials");
        debugPrint("SSID: " + ssid);
        debugPrint("mDNS Name: " + mdnsName);

        preferences.begin(PREFERENCE_NAMESPACE, false);
        preferences.putString("ssid", ssid);
        preferences.putString("password", password);
        preferences.putString("mdnsname", mdnsName);
        preferences.end();
        
        File file = LittleFS.open("/saved.html");
        server.streamFile(file, "text/html", 200);
        String script = 
            "<script>"
            "let link = 'http://" + mdnsName + ".local';"
            "window.location.href = link;"
            "document.getElementById('mdns-link').href = link;"
            "document.getElementById('mdns-link').innerText = link;"
            "</script>";
        server.client().write(script.c_str(), script.length());
        debugPrint("Credentials saved to preferences");
        delay(6000);
        ESP.restart();
    });

    server.onNotFound([this]() {
        server.sendHeader("Location", "http://" + WiFi.softAPIP().toString());
        server.send(302, "text/plain", "");
    });
}

void ConfigPortal::start() {
    debugPrint("Starting configuration portal...");
    WiFi.softAP(AP_SSID);

    // Configure DNS server to capture all DNS requests
    dnsServer.start(53, "*", WiFi.softAPIP());
    
    setupWebRoutes();
    server.begin();
    isActive = true;
}

bool ConfigPortal::tryConnect() {
    preferences.begin(PREFERENCE_NAMESPACE, true);
    String ssid = preferences.getString("ssid", "");
    String password = preferences.getString("password", "");
    preferences.end();

    if (ssid.length() == 0) {
        debugPrint("No WiFi credentials stored");
        return false;
    }

    debugPrint("Attempting to connect to WiFi");
    debugPrint("SSID: " + ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        debugPrint(".");
        attempts++;
    }

    if (WiFi.status() != WL_CONNECTED) {
        debugPrint("Failed to connect to WiFi");
        return false;
    }

    debugPrint("Connected to WiFi");
    debugPrint("IP: " + WiFi.localIP().toString());
    return true;
}

void ConfigPortal::process() {
    if (isActive) {
        dnsServer.processNextRequest();
        server.handleClient();
    }
}

bool ConfigPortal::isConfigMode() const {
    return isActive;
}
