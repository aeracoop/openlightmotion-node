#ifndef CONFIG_H
#define CONFIG_H

#include <Print.h>

#include "config_manager.hpp"

constexpr const char* config_file_path = "/config";

struct Config {
    IPAddress base_ip;
    String base_ssid;
    String base_password;
    int16_t node_number = -1;

    bool fromFile() {
        ConfigManager manager(config_file_path);
        return manager.read([this](String key, String value) {
            if (key == "base_ip") { this->base_ip.fromString(value); }
            else if (key == "base_ssid") { this->base_ssid = value; }
            else if (key == "base_password") { this->base_password = value; }
            else if (key == "node_number") { this->node_number = value.toInt(); }
        });
    }

    void toFile() {
        ConfigManager manager(config_file_path);
        manager.clear();
        manager.write("base_ip", base_ip.toString());
        manager.write("base_ssid", base_ssid);
        manager.write("base_password", base_password);
        manager.write("node_number", String(node_number));
    }

    void printTo(Print& to) {
        to.print("base_ip=");
        to.println(base_ip.toString());
        to.print("base_ssid=");
        to.println(base_ssid);
        to.print("base_password=");
        to.println(base_password);
        to.print("node_number=");
        to.println(String(node_number));
    }
};

#endif //CONFIG_H
