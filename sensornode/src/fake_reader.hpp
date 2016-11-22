#ifndef FAKE_READER_H
#define FAKE_READER_H

#include <Arduino.h>
#include <functional>

#include "common.hpp"

class FakeReader {
public:
    FakeReader() : sending(false) {}

    void init() {
    }

    void setCallback(std::function<void(Point)> f) {
        block_callback = f;
    }

    void begin() {
        sending = true;
    }

    void end() {
        sending = false;
    }

    void loop() {
        if (sending && block_callback) {
            block_callback(Point(rand(), rand()));
        }
    }

protected:
    bool sending;
    std::function<void(Point)> block_callback;
};

#endif //BLOCK_READER_H
