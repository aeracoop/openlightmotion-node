#ifndef COMMON_H
#define COMMON_H

#include <Arduino.h>

struct Point {
    uint16_t x;
    uint16_t y;
    Point(uint16_t x, uint16_t y) : x(x), y(y) {}
};

#endif // COMMON_H
