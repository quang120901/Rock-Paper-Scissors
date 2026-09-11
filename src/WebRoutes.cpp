#include "WebRoutes.h"
#include <SPIFFS.h>
#include "Wifi.h"

void setupRoutes(RPS &game, Display &display, Buzzer &buzzer, int &roundCounter)
{
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
    server.on("/play", HTTP_GET, [&game, &display, &buzzer, &roundCounter](AsyncWebServerRequest *request)
              {
                  if (!request->hasParam("sspb"))
                  {
                      request->send(400, "application/json", "{\"error\":\"missing sspb param\"}");
                      return;
                  }

                  String userInput = request->getParam("sspb")->value();
                  userInput.toLowerCase();

                  int botIndex = random(0, 3);
                  String botChoice = game.chosenInput[botIndex];

                  game.showMove(botChoice);

                  String result = game.checkUserWin(userInput, botChoice);
                  game.checkGameplay();
                  roundCounter++;

                  bool gameOver = (game.state == game.end);

                  display.showRound(roundCounter, botChoice, result,
                                     game.GameScore[0].score, game.GameScore[1].score, game.GameScore[2].score);

                  if (gameOver)
                  {
                      String finalLine;
                      if (game.GameScore[0].score > game.GameScore[1].score)
                      {
                          finalLine = "You win!";
                      }
                      else if (game.GameScore[1].score > game.GameScore[0].score)
                      {
                          finalLine = "Bot wins!";
                      }
                      else
                      {
                          finalLine = "Tie match!";
                      }

                      display.showMessage("GAME OVER!", finalLine);
                      buzzer.playMatchResult(game.GameScore[0].score, game.GameScore[1].score);
                  }

                  String json = "{";
                  json += "\"round\":" + String(roundCounter) + ",";
                  json += "\"user\":\"" + userInput + "\",";
                  json += "\"bot\":\"" + botChoice + "\",";
                  json += "\"result\":\"" + result + "\",";
                  json += "\"won\":" + String(game.GameScore[0].score) + ",";
                  json += "\"lost\":" + String(game.GameScore[1].score) + ",";
                  json += "\"tied\":" + String(game.GameScore[2].score) + ",";
                  json += "\"gameOver\":" + String(gameOver ? "true" : "false");
                  json += "}";

                  request->send(200, "application/json", json);
              });

    // Reset the match back to zero
    server.on("/reset", HTTP_GET, [&game, &display, &roundCounter](AsyncWebServerRequest *request)
              {
                  game.resetGame();
                  roundCounter = 0;
                  display.showMessage("Rock Paper", "Scissors!");
                  request->send(200, "application/json", "{\"status\":\"ok\"}");
              });
}