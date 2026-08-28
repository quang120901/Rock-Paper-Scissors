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

String userInput = "";
int randomNumber = 0;
//prototype
void checkParams(AsyncWebServerRequest *request);

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
              checkParams(request);
            });

  // CSS
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/style.css", "text/css"); });
  // jQuery
  server.on("/jquery.min.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/jquery.min.js", "text/javascript"); });

  server.begin();

  Game.state = Game.waitForUserInput;

}

void loop()
{

  String bot = "";

  switch(Game.state) {
    case Game.playing:

      randomNumber = random(0,3);    
      bot = Game.chosenInput[randomNumber];

      Game.checkUserWin(userInput, bot);
      Game.checkGameplay();
    break;

    case Game.waitForUserInput:
    break;

    case Game.end:
    break;

  }

  // String userInput = Game.chosenInput[random(0, 4)];
  // Serial.println(userInput);
  // Game.checkUserWin(userInput, "rock");
  // Serial.printf("Won: %d | Lost: %d | Tied: %d \n",
  //               Game.GameScore[0].score,
  //               Game.GameScore[1].score,
  //               Game.GameScore[2].score);
  // delay(2000);
}

void checkParams(AsyncWebServerRequest *request) {

  if(request->hasParam("sspb")) {
    userInput=request->getParam("sspb")->value();

    Game.state = Game.playing;
  }

}