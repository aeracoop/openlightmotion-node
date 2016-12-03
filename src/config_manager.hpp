#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <functional>

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FS.h>

class ConfigManager {
public:
    ConfigManager(const char* path) : path(path) {}

    bool read(std::function<void(String key, String value)> cb) {
        File f = SPIFFS.open(path, "r");
        if (f) {
            while (f.available()) {
                String line = f.readStringUntil('\n');
                int sep = line.indexOf("=");
                if (sep) {
                    String key = line.substring(0, sep);
                    String value = line.substring(sep + 1);
                    if (key != "" && cb) {
                        cb(key, value);
                    }
                }
            }
            f.close();
            return true;
        } else return false;
    }

    bool clear() {
        File f = SPIFFS.open(path, "w");
        if (f) {
            f.close();
            return true;
        } else return false;
    }

    bool write(String key, String value) {
        File f = SPIFFS.open(path, "a");
        if (f) {
            f.write(reinterpret_cast<const uint8_t*>(key.c_str()), key.length());
            f.write('=');
            f.write(reinterpret_cast<const uint8_t*>(value.c_str()), value.length());
            f.write('\n');
            f.close();
            return true;
        } else return false;
    }


protected:
    const char* path;
};


#endif //CONFIG_MANAGER_H
