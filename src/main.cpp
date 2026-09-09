#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <ESP32Servo.h>

#include "RPS.h"
#include "Credentials.h"
#include "Wifi.h"

RPS Game = RPS();

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