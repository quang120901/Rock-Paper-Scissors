#include <Arduino.h>
#include "RPS.h"

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