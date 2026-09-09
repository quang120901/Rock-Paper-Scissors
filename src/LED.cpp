#include "LED.h"

LED::LED(uint8_t pin) noexcept {
    begin(pin);
}

void LED::begin(uint8_t pin) noexcept {
    m_pin = pin;
    pinMode(m_pin, OUTPUT);
    off();
}

void LED::on() const noexcept {
    digitalWrite(m_pin, HIGH);
}

void LED::off() const noexcept {
    digitalWrite(m_pin, LOW);
}

bool LED::isInitialized() const noexcept {
    return m_pin != 255;
}

uint8_t LED::getPin() const noexcept {
    return m_pin;
}