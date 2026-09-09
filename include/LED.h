#ifndef LED_H
#define LED_H

#include <Arduino.h>

class LED {
    uint8_t m_pin = 255;

public:
    LED() noexcept = default;
    explicit LED(uint8_t pin) noexcept;

    void begin(uint8_t pin) noexcept;

    void on() const noexcept;
    void off() const noexcept;

    [[nodiscard]] bool isInitialized() const noexcept;
    [[nodiscard]] uint8_t getPin() const noexcept;
};

#endif