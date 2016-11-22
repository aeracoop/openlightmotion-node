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
        write(reinterpret_cast<uint8_t*>(&point), sizeof(point));
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
