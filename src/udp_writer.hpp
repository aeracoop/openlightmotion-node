#ifndef UDP_WRITER_H
#define UDP_WRITER_H

#include <WiFiUdp.h>

#include "common.hpp"

constexpr uint16_t target_port = 7777;

class UDPWriter {
public:
    void init(IPAddress newTarget) {
        target = newTarget;
    }

    bool write(Point point) {
        uint8_t buf[4];
        buf[0] = (point.x >> 8) & 0xFF;
        buf[1] = point.x & 0xFF;
        buf[2] = (point.y >> 8) & 0xFF;
        buf[3] = point.y & 0xFF;
        write(buf, 4);
    }

protected:
    bool write(uint8_t* buffer, size_t size) {
        Udp.beginPacket(target, target_port);
        Udp.write(buffer, size);
        Udp.endPacket();
    }

    WiFiUDP Udp;
    IPAddress target;
};

#endif // UDP_WRITER_H
