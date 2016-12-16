#ifndef WEB_H
#define WEB_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <FS.h>

#include "config.hpp"
#include "logging.hpp"
#include "stats_manager.hpp"

#ifndef WEB_PORT
  #define WEB_PORT 80
#endif

class WebManager {
public:
    WebManager(Config& config) : server(ESP8266WebServer(WEB_PORT)),
                                 config(config) {}

    void init() {
        server.on("/config-reset", [this]() { this->resetConfiguration(); });
        server.on("/config-save", [this]() { this->saveConfiguration(); });
        server.on("/config", [this]() { this->sendConfigurationPage(); });
        server.on("/stats", [this]() { this->sendStatsPage(); });
        server.on("/", [this]() { this->sendStatsPage(); });
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
                    String("<h1>Change Node Configuration</h1>") +
                           "<p>Please note that the changes you make here will only be effective after you reboot the node.</p>" + 
                           "<form method='POST' action='/config-save'>" +
                           "<table>" +
                           "<tr><td>base_ssid:</td><td><input type='text' name='base_ssid' value='" + config.base_ssid + "'></td></tr>" +
                           "<tr><td>base_password:</td><td><input type='text' name='base_password' value='" + config.base_password + "'></td></tr>" +
                           "<tr><td>base_ip:</td><td><input type='text' name='base_ip' value='" + config.base_ip.toString() + "'></td></tr>" +
                           "<tr><td>node_number:</td><td><input type='text' name='node_number' value='" + String(config.node_number) + "'></td></tr>" +
                           "<tr><td colspan='2'><input type='submit' name='save' value='Save'></input>" +
                           "</table></form>" +
                           "<form method='POST' action='/config-reset'>" +
                           "<input type='submit' name='reset' value='Reset to defaults'></input>" +
                           "</form>" +
                           "<a href='/stats'>Node statistics</a>");
    }

    void resetConfiguration() {
        SPIFFS.remove(config_file_path);
        config.resetDefaults();
        config.printTo(Serial);
        server.sendHeader("Location", "/stats");
        server.send(303);
    }

    void saveConfiguration() {
        if (server.hasArg("base_ip")) config.base_ip.fromString(server.arg("base_ip"));
        if (server.hasArg("base_ssid")) config.base_ssid = server.arg("base_ssid");
        if (server.hasArg("base_password")) config.base_password = server.arg("base_password");
        if (server.hasArg("node_number")) config.node_number = server.arg("node_number").toInt();
        config.toFile();
        config.printTo(Serial);
        server.sendHeader("Location", "/stats");
        server.send(303);
    }

    void sendStatsPage() {
        StatsManager& stats = StatsManager::instance();
        uint32_t tx_time = millis() - stats.tx_start_time;
        uint32_t sent = stats.packets_sent;

        server.send(200, "text/html",
            String("<h1>Statistics</h1><table>") +
            "<tr><td>Node IP address:</td><td>" + WiFi.localIP().toString() + " s</td></tr>" +
            "<tr><td>Time transmitting:</td><td>" + (tx_time / 1000) + " s</td></tr>" +
            "<tr><td>Total packets sent:</td><td>" + sent + "</td></tr>" +
            "<tr><td>Data rate:</td><td>" + (sent / (tx_time / 1000))  + " packets/s</td></tr>" +
            "<tr><td>Last connection failure code:</td><td>" + StatsManager::instance().last_disconnect_reason + " (<a href='https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFiType.h#L66' target='blank'>codes table</a>)</td></tr>" +
            "</table>" +
            "<h1>Current node configuration</h1><table>" +
            "<tr><td>base_ssid:</td><td>" + config.base_ssid + "</td></tr>" +
            "<tr><td>base_password:</td><td>" + config.base_password + "</td></tr>" +
            "<tr><td>base_ip:</td><td>" + config.base_ip.toString() + "</td></tr>" +
            "<tr><td>node_number:</td><td>" + String(config.node_number) + "</td></tr>" +
            "</table>" +
            "<a href='/config'>Edit node configuration</a>"
        );
    }

    ESP8266WebServer server;
    Config& config;
};

#endif // WEB_H
