#ifndef WEB_H
#define WEB_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <FS.h>

#include "config.hpp"
#include "logging.hpp"

#ifndef WEB_PORT
  #define WEB_PORT 80
#endif

class WebManager {
public:
    WebManager(Config& config) : server(ESP8266WebServer(WEB_PORT)),
                                 config(config) {}

    void init() {
        server.on("/config", [this]() { this->saveConfiguration(); });
        server.on("/", [this]() { this->sendConfigurationPage(); });
        server.onNotFound([this]() {
            server.send(404, "text/plain", String("Not found: ") + server.uri());
        });
    }

    void begin() {
        log("Starting web server.");
        server.begin();
    }

    void loop() {
        server.handleClient();
    }

protected:
    void sendConfigurationPage() {
        server.send(200, "text/html",
                    String("OK<br>base_ip:") + config.base_ip.toString() +
                           "<br>base_ssid:" + config.base_ssid +
                           "<br>base_password:" + config.base_password +
                           "<br>node_number:" + config.node_number);
    }

    void saveConfiguration() {
        if (server.hasArg("base_ip")) config.base_ip.fromString(server.arg("base_ip"));
        if (server.hasArg("base_ssid")) config.base_ssid = server.arg("base_ssid");
        if (server.hasArg("base_password")) config.base_password = server.arg("base_password");
        if (server.hasArg("node_number")) config.node_number = server.arg("node_number").toInt();
        config.toFile();
        config.printTo(Serial);
        sendConfigurationPage();
    }

    ESP8266WebServer server;
    Config& config;
};

#endif // WEB_H
