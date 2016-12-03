#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>

#include <PixySPI_SS.h>

#include "wifi_connector.hpp"
#include "block_reader.hpp"
#include "fake_reader.hpp"
#include "udp_writer.hpp"
#include "common.hpp"

WiFiConnector wifi;
BlockReader reader;
//FakeReader reader;
UDPWriter writer;

void setup() {
    Serial.begin(9600);
    Serial.println("Open Light Motion sensor module");

    wifi.init([writer](IPAddress target) {
        Serial.println("Connected to station. Station address: ");
        Serial.println(target);
        writer.init(target);
    SPIFFS.begin();
        writer.write(Point(0, 0));
        reader.begin();
    });

    reader.init();
    reader.setCallback([writer](Point point) {
        writer.write(point);
    });
}

void loop() {
    reader.loop();
}
