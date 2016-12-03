#ifndef WIFI_CONNECTOR_H
#define WIFI_CONNECTOR_H

#include <ESP8266WiFi.h>

#include "config.hpp"
#include "web.hpp"
class WiFiConnector {
public:
    WiFiConnector(Config& config, WebManager& web) : config(config),
                                                     web(web),
                                                     connectionAttemptStartedAt(0)

    void init(std::function<void(IPAddress)> onStationConnected) {
        onIP = WiFi.onStationModeGotIP([onStationConnected](const WiFiEventStationModeGotIP e){
            if (onStationConnected) onStationConnected(WiFi.gatewayIP());
        });
        WiFi.begin("OpenLightMotionStation", "12345678");
        WiFi.softAPConfig(IPAddress(192, 168, 0, 1), IPAddress(0, 0, 0, 0), IPAddress(255, 255, 255, 0));
        WiFi.softAP("OpenLightMotionNode", "");
    }

private:
    WiFiEventHandler onIP;

    uint32_t connectionAttemptStartedAt;
    Config& config;
    WebManager& web;
};

#endif // WIFI_CONNECTOR_H
