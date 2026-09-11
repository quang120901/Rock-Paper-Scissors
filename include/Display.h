#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class Display
{
private:
    LiquidCrystal_I2C lcd;
    uint8_t m_cols;
    uint8_t m_rows;

    String centered(const String &text) const;
    static String capitalize(const String &s);

public:
    Display(uint8_t i2cAddress, uint8_t cols, uint8_t rows);

    void begin(uint8_t sdaPin, uint8_t sclPin);

    // Shows the outcome of one round, using all 4 rows.
    void showRound(int round, const String &botChoice, const String &result,
                   int won, int lost, int tied);

    // Generic 2-line message (used for the idle screen and game-over screen).
    void showMessage(const String &line1, const String &line2);
};

#endif