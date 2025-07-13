#include "web_server.h"
#include "debug.h"

WebInterface::WebInterface(AsyncWebServer& webServer) 
    : server(webServer) 
{
}

void WebInterface::start() {
    // Serve the index page at root
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(LittleFS, "/index.html", "text/html");
    });

    // Serve static files
    server.serveStatic("/", LittleFS, "/");

    // Handle 404
    server.onNotFound([](AsyncWebServerRequest *request) {
        debugPrint("404: " + String(request->url().c_str()));
        request->send(LittleFS, "/404.html", "text/html", 404);
    });

    server.begin();
    debugPrint("Web interface started");
}
