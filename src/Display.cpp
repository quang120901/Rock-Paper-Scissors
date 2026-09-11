#include "Display.h"
#include <Wire.h>

Display::Display(uint8_t i2cAddress, uint8_t cols, uint8_t rows)
    : lcd(i2cAddress, cols, rows), m_cols(cols), m_rows(rows)
{
}

void Display::begin(uint8_t sdaPin, uint8_t sclPin)
{
    Wire.begin(sdaPin, sclPin);
    lcd.init();
    lcd.backlight();
}

String Display::capitalize(const String &s)
{
    if (s.length() == 0)
    {
        return s;
    }
    String result = s;
    result[0] = toupper(result[0]);
    return result;
}

String Display::centered(const String &text) const
{
    if ((int)text.length() >= m_cols)
    {
        return text.substring(0, m_cols);
    }

    int totalPad = m_cols - text.length();
    int padLeft = totalPad / 2;

    String result = "";
    for (int i = 0; i < padLeft; i++)
    {
        result += ' ';
    }
    result += text;

    return result;
}

void Display::showRound(int round, const String &botChoice, const String &result,
                         int won, int lost, int tied)
{
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print(centered("-- ROUND " + String(round) + " --"));

    lcd.setCursor(0, 1);
    lcd.print(centered("Bot: " + capitalize(botChoice)));

    lcd.setCursor(0, 2);
    String resultLine;
    if (result == "won")
    {
        resultLine = "You WIN!";
    }
    else if (result == "lost")
    {
        resultLine = "You LOSE!";
    }
    else
    {
        resultLine = "It's a TIE!";
    }
    lcd.print(centered(resultLine));

    lcd.setCursor(0, 3);
    lcd.print(centered("W:" + String(won) + " L:" + String(lost) + " T:" + String(tied)));
}

void Display::showMessage(const String &line1, const String &line2)
{
    lcd.clear();
    // Place the 2-line message on the middle rows so it looks vertically
    // centered on a 4-row display.
    uint8_t row1 = (m_rows >= 4) ? 1 : 0;
    uint8_t row2 = (m_rows >= 4) ? 2 : 1;

    lcd.setCursor(0, row1);
    lcd.print(centered(line1));
    lcd.setCursor(0, row2);
    lcd.print(centered(line2));
}