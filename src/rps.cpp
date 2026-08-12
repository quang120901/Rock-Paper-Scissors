#include <Arduino.h>
#include "rps.h"

void rps::checkUserWin(String user, String bot)
{
    if (user == bot)
    {
        // tied
        Serial.println("TIED");
    }
    else
    {
        for (int i = 0; i < 3; i++)
        {
            if (solutions[i][0] == user)
            {
                if(solutions[i][1]==bot){
                    //we have won
                    Serial.println("WON");

                }
                else {
                    //we have lost
                    Serial.println("LOST");

                }
            }
        }
    }
}