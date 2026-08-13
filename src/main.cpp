#include <Arduino.h>
#include "RPS.h"

RPS Game = RPS();

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  String userInput = Game.chosenInput[random(0,4)];
  Serial.println(userInput);
  Game.checkUserWin(userInput, "rock");
  Serial.printf("Won: %d | Lost: %d | Tied: %d \n",
               Game.GameScore[0].score,
               Game.GameScore[1].score,
               Game.GameScore[2].score);
  delay(2000);
}