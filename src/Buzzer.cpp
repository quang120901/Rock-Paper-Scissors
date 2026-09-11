#include "Buzzer.h"

Buzzer::Buzzer(uint8_t pin) : m_pin(pin)
{
}

void Buzzer::begin()
{
    pinMode(m_pin, OUTPUT);
    digitalWrite(m_pin, LOW);
}

// Bit-banged square wave, deliberately NOT using tone()/LEDC.
// This keeps the buzzer fully independent of the hardware PWM channels
// used by ESP32Servo, avoiding channel conflicts that could otherwise
// freeze a servo when the buzzer plays.
void Buzzer::beep(unsigned int frequencyHz, unsigned int durationMs)
{
    unsigned long periodUs = 1000000UL / frequencyHz;
    unsigned long halfPeriodUs = periodUs / 2;
    unsigned long cycles = (unsigned long)durationMs * 1000UL / periodUs;

    for (unsigned long i = 0; i < cycles; i++)
    {
        digitalWrite(m_pin, HIGH);
        delayMicroseconds(halfPeriodUs);
        digitalWrite(m_pin, LOW);
        delayMicroseconds(halfPeriodUs);
    }
}

void Buzzer::playMatchResult(int won, int lost)
{
    if (won > lost)
    {
        // Happy ascending jingle
        beep(1000, 150);
        delay(30);
        beep(1300, 150);
        delay(30);
        beep(1600, 250);
    }
    else if (lost > won)
    {
        // Sad descending tone
        beep(700, 200);
        delay(30);
        beep(500, 200);
        delay(30);
        beep(300, 300);
    }
    else
    {
        // Flat "tie" beep
        beep(600, 200);
        delay(30);
        beep(600, 200);
    }

    digitalWrite(m_pin, LOW);
}