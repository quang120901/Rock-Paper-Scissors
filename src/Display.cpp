#include "Display.h"
#include <Wire.h>

Display::Display(uint8_t i2cAddress, uint8_t cols, uint8_t rows)
    : lcd(i2cAddress, cols, rows)
{
}

void Display::begin(uint8_t sdaPin, uint8_t sclPin)
{
    Wire.begin(sdaPin, sclPin);
    lcd.init();
    lcd.backlight();
}

void Display::showRound(int round, const String &botChoice, const String &result,
                         int won, int lost, int tied)
{
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Round " + String(round));

    lcd.setCursor(0, 1);
    lcd.print("Bot: " + botChoice);

    lcd.setCursor(0, 2);
    lcd.print("Result: " + result);

    lcd.setCursor(0, 3);
    lcd.print("W:" + String(won) + " L:" + String(lost) + " T:" + String(tied));
}

void Display::showMessage(const String &line1, const String &line2)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
}