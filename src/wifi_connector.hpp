#ifndef WIFI_CONNECTOR_H
#define WIFI_CONNECTOR_H

#include <ESP8266WiFi.h>

class WiFiConnector {
public:
    WiFiConnector() {}

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
};

#endif // WIFI_CONNECTOR_H
