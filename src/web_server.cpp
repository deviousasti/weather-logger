#include "web_server.h"
#include "debug.h"

WebInterface::WebInterface(AsyncWebServer& webServer) 
    : server(webServer)
    , events("/events")
{
}

void WebInterface::sendEvent(const char* eventName, float value) {
    char str[32];
    snprintf(str, sizeof(str), "%.2f", value);
    events.send(str, eventName, millis());
}

void WebInterface::start() {
    // Set up SSE
    events.onConnect([](AsyncEventSourceClient* client) {
        if(client->lastId()) {
            debugPrint("Client reconnected! Last message ID that it received was: " + String(client->lastId()));
        }
        client->send("hello!", NULL, millis(), 1000);
    });
    server.addHandler(&events);

    // Serve the index page at root
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(LittleFS, "/index.html", "text/html");
    });

    // Serve static files
    server.serveStatic("/static", LittleFS, "/static/", "max-age=86400, private");

    // Handle 404
    server.onNotFound([](AsyncWebServerRequest *request) {
        debugPrint("404: " + String(request->url().c_str()));
        request->send(LittleFS, "/404.html", "text/html", 404);
    });

    server.begin();
    debugPrint("Web interface started");
}
