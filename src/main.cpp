#include <Arduino.h>
#include "RPS.h"

RPS Game = RPS();

void setup() {
    Serial.begin(115200);
    Game.checkUserWin("paper","rock");
}

void loop() {
  // put your main code here, to run repeatedly:
}