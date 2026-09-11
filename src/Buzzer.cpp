#include "Buzzer.h"

Buzzer::Buzzer(uint8_t pin) : m_pin(pin)
{
}

void Buzzer::begin()
{
    pinMode(m_pin, OUTPUT);
}

void Buzzer::playMatchResult(int won, int lost)
{
    if (won > lost)
    {
        // Happy ascending jingle
        tone(m_pin, 1000, 150);
        delay(180);
        tone(m_pin, 1300, 150);
        delay(180);
        tone(m_pin, 1600, 250);
        delay(280);
    }
    else if (lost > won)
    {
        // Sad descending tone
        tone(m_pin, 700, 200);
        delay(220);
        tone(m_pin, 500, 200);
        delay(220);
        tone(m_pin, 300, 300);
        delay(320);
    }
    else
    {
        // Flat "tie" beep
        tone(m_pin, 600, 200);
        delay(220);
        tone(m_pin, 600, 200);
        delay(220);
    }

    noTone(m_pin);
}