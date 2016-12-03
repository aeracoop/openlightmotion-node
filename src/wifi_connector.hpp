#ifndef WIFI_CONNECTOR_H
#define WIFI_CONNECTOR_H

#include <ESP8266WiFi.h>

#include "config.hpp"
#include "web.hpp"
#include "logging.hpp"

constexpr uint32_t connection_timeout = 10000;

class WiFiConnector {
public:
    WiFiConnector(Config& config, WebManager& web) : config(config),
                                                     web(web),
                                                     connectionAttemptStartedAt(0)
    {
        // Don't use the internal ESP8266 method for storing SSID+password
        // in flash, since we are using our own.
        WiFi.persistent(false);

        // If previous configuration was stored in flash, don't use it to try
        // to reconnect automatically.
        WiFi.setAutoConnect(false);

        onDisconnected = WiFi.onStationModeDisconnected([this](const WiFiEventStationModeDisconnected e) {
            // Log any disconnect event (which are usually connection errors)
            // except for the case when we are explicitly leaving the AP (ASSOC_LEAVE event)
            // Codes for the various types are here:
            // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFiType.h#L66
            if (e.reason != WIFI_DISCONNECT_REASON_ASSOC_LEAVE) {
                log("Disconnected from base station for reason: ", e.reason);
                if (e.reason == WIFI_DISCONNECT_REASON_NO_AP_FOUND) {
                    // When we get disconnected because the base AP has disappeared,
                    // we call WiFi.disconnect to prevent the system to keep trying to
                    // reconnect. The reason for this is because in this case
                    // the system will try to find the base AP on another channel.
                    // Since we only have one radio, this will cause the node's own
                    // AP to go down for the duration of the scan.
                    // If the base AP does not appear again, we will never be able
                    // to connect to the node AP, because the system will keep
                    // changing channels.
                    WiFi.disconnect(false);
                }
            }

            this->lastDisconnectReason = e.reason;
        });

        onConnected = WiFi.onStationModeConnected([](const WiFiEventStationModeConnected e) {
            log("Authenticated with base station ", e.ssid, ". Requesting IP.");
        });

        onIP = WiFi.onStationModeGotIP([this](const WiFiEventStationModeGotIP e) {
            log("Connected to base station ", WiFi.SSID() , ". Local IP: ", WiFi.localIP(), " Gateway: ", WiFi.gatewayIP());
            this->connectionAttemptStartedAt = 0;
            this->setupSoftAP();
            if (this->stationConnectedCallback) this->stationConnectedCallback(WiFi.gatewayIP());
        });
    }

    void init(std::function<void(IPAddress)> callback) {
        stationConnectedCallback = callback;

        WiFi.mode(WIFI_AP_STA);

        if (config.base_ssid.length() > 0) {
            connectionAttemptStartedAt = millis();
            log("Attempting to connect to base station ", config.base_ssid, " (pwd: ", config.base_password, ")");
            WiFi.begin(config.base_ssid.c_str(), config.base_password.c_str());
        } else {
            // if there is no base station configured, just start the access point
            // immediately
            setupSoftAP();
        }
    }

    void loop() {
        // do nothing if we never tried to start a connection
        if (connectionAttemptStartedAt == 0) return;

        // if we tried to start a connection but it is taking too long,
        // stop trying and start the access point
        if (millis() - connectionAttemptStartedAt >= connection_timeout) {
            log("Timeout connecting to base station.");
            connectionAttemptStartedAt = 0;
            WiFi.disconnect(false);
            setupSoftAP();
        }
    }

    void setupSoftAP() {
        String node_name = "OpenLightMotionNode";
        if (config.node_number > 0) {
            node_name += "_";
            node_name += config.node_number;
        }

        // Note that we provide a password only to keep away devices that try to
        // connect to any open network automatically. We do this because the
        // ESP8266 accepts a maximum of 4 clients at the same time, so if there
        // are enough people with mobile phones in the room it can be a problem.
        // Also note that the password needs to be at least 8 characters long
        // or it will be ignored and we will have an open network.
        WiFi.softAP(node_name.c_str(), "12345678");
        WiFi.softAPConfig(IPAddress(192, 168, 0, 1), IPAddress(0, 0, 0, 0), IPAddress(255, 255, 255, 0));
        log("Started node access point with name: ", node_name);

        web.begin();
    }

public:
    WiFiDisconnectReason lastDisconnectReason;

private:
    std::function<void(IPAddress)> stationConnectedCallback;

    WiFiEventHandler onConnected;
    WiFiEventHandler onIP;
    WiFiEventHandler onDisconnected;

    uint32_t connectionAttemptStartedAt;
    Config& config;
    WebManager& web;
};

#endif // WIFI_CONNECTOR_H
