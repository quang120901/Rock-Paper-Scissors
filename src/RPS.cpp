#include <Arduino.h>
#include "RPS.h"

void RPS::checkGameplay() {
    if(GameScore[0].score >= 2 || GameScore[1].score >= 2) {
        state = end;
    } else {
        state = waitForUserInput;
    }

    Serial.print("result");
    Serial.print("Won:");
    Serial.print(GameScore[0].score);
    Serial.print("Lost:");
    Serial.print(GameScore[1].score);
    Serial.print("Tied:");
    Serial.print(GameScore[2].score);
    Serial.println("---------------");



}

void RPS::checkUserWin(String user, String bot)
{
    if (user == bot)
    {
        // tied
        GameScore[2].score += 1;
    }
    else
    {
        for (int i = 0; i < 3; i++)
        {
            if (solutions[i][0] == user)
            {
                if (solutions[i][1] == bot)
                {
                    GameScore[0].score += 1;
                }
                else
                {
                    GameScore[1].score += 1;
                }
            }
        }
    }
}