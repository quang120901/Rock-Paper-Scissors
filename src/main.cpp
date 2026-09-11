#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <ESP32Servo.h>

#include "RPS.h"
#include "Credentials.h"
#include "Wifi.h"
#include "Display.h"
#include "Buzzer.h"
#include "WebRoutes.h"

#define BUZZER_PIN 22
#define LCD_SDA_PIN 32
#define LCD_SCL_PIN 33

RPS Game = RPS();
Display display(0x27, 20, 4);
Buzzer buzzer(BUZZER_PIN);

int roundCounter = 0;

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

  display.begin(LCD_SDA_PIN, LCD_SCL_PIN);
  display.showMessage("Rock Paper", "Scissors!");

  buzzer.begin();

  setupRoutes(Game, display, buzzer, roundCounter);

  server.begin();

  Game.state = Game.waitForUserInput;
}

void loop()
{
  // All gameplay logic now runs synchronously inside the "/play" route
  // (see WebRoutes.cpp), so there is nothing left to poll for here.
}