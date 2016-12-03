#ifndef STATS_MANAGER_H
#define STATS_MANAGER_H

#include <ESP8266WiFiType.h>

class StatsManager {
public:
    static StatsManager& instance() {
        static StatsManager manager;
        return manager;
    }

    uint32_t packets_sent;
    uint32_t tx_start_time;
    uint32_t last_disconnect_reason;

private:
    StatsManager() : packets_sent(0),
                     tx_start_time(0),
                     last_disconnect_reason(0) {}
};

#endif //STATS_MANAGER_H
