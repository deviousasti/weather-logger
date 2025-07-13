#include "web_server.h"
#include "debug.h"

WebInterface::WebInterface(AsyncWebServer& webServer) 
    : server(webServer) 
{
}

void WebInterface::begin() {
    // Serve the index page at root
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        debugPrint("Serving index.html");
        request->send(LittleFS, "/index.html", "text/html");
    });

    // Serve static files
    server.serveStatic("/", LittleFS, "/");

    // Handle 404
    server.onNotFound([](AsyncWebServerRequest *request) {
        debugPrint("404: " + String(request->url().c_str()));
        request->send(404, "text/plain", "Not found");
    });

    server.begin();
    debugPrint("Web interface started");
}
