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

void setup()
{
  Serial.begin(115200);

  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  connectAP();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              request->send(SPIFFS, "/index.html", String(), false, processor);
              // checkParams(request);
            });

  // CSS
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/style.css", "text/css"); });
  // jQuery
  server.on("/jquery.min.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/jquery.min.js", "text/javascript"); });

  server.begin();





}

void loop()
{
  // String userInput = Game.chosenInput[random(0, 4)];
  // Serial.println(userInput);
  // Game.checkUserWin(userInput, "rock");
  // Serial.printf("Won: %d | Lost: %d | Tied: %d \n",
  //               Game.GameScore[0].score,
  //               Game.GameScore[1].score,
  //               Game.GameScore[2].score);
  // delay(2000);
}