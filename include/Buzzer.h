#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

class Buzzer
{
private:
    uint8_t m_pin;

public:
    explicit Buzzer(uint8_t pin);

    void begin();

    // Plays a short jingle depending on the final match result.
    // Blocking, but short (~1 second total).
    void playMatchResult(int won, int lost);

private:
    // Bit-banged beep (no tone()/LEDC — avoids conflicting with the
    // hardware PWM channels used by ESP32Servo).
    void beep(unsigned int frequencyHz, unsigned int durationMs);
};

#endif