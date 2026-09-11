#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "RPS.h"
#include "Credentials.h"
#include "Wifi.h"

RPS Game = RPS();

int roundCounter = 0;

#define BUZZER_PIN 22
#define LCD_SDA_PIN 33
#define LCD_SCL_PIN 32

LiquidCrystal_I2C lcd(0x27, 20, 4);

void updateLCD(int round, const String &botChoice, const String &result, int won, int lost, int tied)
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

void showLCDMessage(const String &line1, const String &line2)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}

// Only plays at the end of the match. Blocking, but short (~1s).
void playGameOverSound(int won, int lost)
{
  if (won > lost)
  {
    // Happy ascending jingle
    tone(BUZZER_PIN, 1000, 150);
    delay(180);
    tone(BUZZER_PIN, 1300, 150);
    delay(180);
    tone(BUZZER_PIN, 1600, 250);
    delay(280);
  }
  else if (lost > won)
  {
    // Sad descending tone
    tone(BUZZER_PIN, 700, 200);
    delay(220);
    tone(BUZZER_PIN, 500, 200);
    delay(220);
    tone(BUZZER_PIN, 300, 300);
    delay(320);
  }
  else
  {
    // Flat "tie" beep
    tone(BUZZER_PIN, 600, 200);
    delay(220);
    tone(BUZZER_PIN, 600, 200);
    delay(220);
  }

  noTone(BUZZER_PIN);
}

void setup()
{
  Serial.begin(115200);

  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  connectAP();

  // Left-to-right servo order matches the dropdown in index.html:
  // Scissors, Rock, Paper -> pins 14, 27, 25 (per diagram.json wiring)
  Game.attachServos(14, 27, 25);

  Wire.begin(LCD_SDA_PIN, LCD_SCL_PIN);
  lcd.init();
  lcd.backlight();
  showLCDMessage("Rock Paper", "Scissors!");

  pinMode(BUZZER_PIN, OUTPUT);

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", String(), false, processor); });

  // CSS
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/style.css", "text/css"); });
  // jQuery
  server.on("/jquery.min.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/jquery.min.js", "text/javascript"); });

  // Play one round. Expects ?sspb=rock|paper|scissors
  // Responds with JSON describing what happened in this round.
  server.on("/play", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              if (!request->hasParam("sspb"))
              {
                request->send(400, "application/json", "{\"error\":\"missing sspb param\"}");
                return;
              }

              String userInput = request->getParam("sspb")->value();
              userInput.toLowerCase();

              int botIndex = random(0, 3);
              String botChoice = Game.chosenInput[botIndex];

              Game.showMove(botChoice);

              String result = Game.checkUserWin(userInput, botChoice);
              Game.checkGameplay();
              roundCounter++;

              bool gameOver = (Game.state == Game.end);

              updateLCD(roundCounter, botChoice, result, Game.GameScore[0].score, Game.GameScore[1].score, Game.GameScore[2].score);

              if (gameOver)
              {
                String finalLine;
                if (Game.GameScore[0].score > Game.GameScore[1].score)
                {
                  finalLine = "You win!";
                }
                else if (Game.GameScore[1].score > Game.GameScore[0].score)
                {
                  finalLine = "Bot wins!";
                }
                else
                {
                  finalLine = "Tie match!";
                }

                showLCDMessage("GAME OVER!", finalLine);
                playGameOverSound(Game.GameScore[0].score, Game.GameScore[1].score);
              }

              String json = "{";
              json += "\"round\":" + String(roundCounter) + ",";
              json += "\"user\":\"" + userInput + "\",";
              json += "\"bot\":\"" + botChoice + "\",";
              json += "\"result\":\"" + result + "\",";
              json += "\"won\":" + String(Game.GameScore[0].score) + ",";
              json += "\"lost\":" + String(Game.GameScore[1].score) + ",";
              json += "\"tied\":" + String(Game.GameScore[2].score) + ",";
              json += "\"gameOver\":" + String(gameOver ? "true" : "false");
              json += "}";

              request->send(200, "application/json", json);
            });

  // Reset the match back to zero
  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              Game.resetGame();
              roundCounter = 0;
              showLCDMessage("Rock Paper", "Scissors!");
              request->send(200, "application/json", "{\"status\":\"ok\"}");
            });

  server.begin();

  Game.state = Game.waitForUserInput;
}

void loop()
{
  // All gameplay logic now runs synchronously inside the "/play" route,
  // so there is nothing left to poll for here.
}