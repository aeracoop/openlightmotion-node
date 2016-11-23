#ifndef BLOCK_READER_H
#define BLOCK_READER_H

#include <Arduino.h>
#include <functional>

#include <SPI.h>
#include <Wire.h>
#include <PixySPI_SS.h>

#include "common.hpp"

class BlockReader {
public:
    BlockReader() : pixy(PixySPI_SS(10)), sending(false) {}

    void init() {
        pixy.init();
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
        if (!sending) return; // no point in reading data if we don't send it

        old_blocks = blocks;
        blocks = pixy.getBlocks();

        if (blocks && (old_blocks != blocks)) {
            for (uint16_t i = 0; i < blocks; i++) {
                pixy.blocks[i].print();
                if (block_callback) {
                    block_callback(Point(pixy.blocks[i].x, pixy.blocks[i].y));
                }
            }
        }
    }

protected:
    bool sending;
    uint16_t blocks;
    uint16_t old_blocks;
    std::function<void(Point)> block_callback;
    PixySPI_SS pixy;
};

#endif //BLOCK_READER_H
