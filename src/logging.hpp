#ifndef UTILS_LOGGING_H
#define UTILS_LOGGING_H

/*
Type safe logging functions that just pass the arguments on to
Serial.print[ln] if we have serial logging defined. Otherwise
they should just get optimized away to nothing.

For more information on the C++11 template syntax used see
https://monoinfinito.wordpress.com/2011/04/26/cool-c0x-features-iii-variadic-templates-a-fix-for-varargs/
*/
#ifdef LOG_ACTIVE

void log_init() {
    #ifdef LOG_USE_SERIAL
        Serial.begin(LOG_USE_SERIAL_SPEED);
        Serial.println("Serial initialized.");
    #endif
    #ifdef LOG_USE_LED
        pinMode(LOG_USE_LED_PIN, OUTPUT);
    #endif
}

void log()
{
    #ifdef LOG_USE_SERIAL
        Serial.println();
    #endif
}
template <typename H, typename... T>
void log(H h, T... t)
{
    #ifdef LOG_USE_SERIAL
        Serial.print(h);
        log(t...);
    #endif
}

void log_led(bool on) {
    #ifdef LOG_USE_LED
        digitalWrite(LOG_USE_LED_PIN, on ? HIGH : LOW);
    #endif
}

#else // LOG_ACTIVE

#define log_init()
#define log(...)
#define log_led()

#endif

#endif // UTILS_LOGGING_H
