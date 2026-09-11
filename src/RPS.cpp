#include <Arduino.h>
#include "RPS.h"
#include "LED.h"

LED ledRed1 = LED(4);
LED ledRed2 = LED(17);
LED ledBlue = LED(5);
LED ledYellow1 = LED(18);
LED ledYellow2 = LED(19);


void RPS::allLEDsOff() {
    ledRed1.off();
    ledRed2.off();
    ledBlue.off();
    ledYellow1.off();
    ledYellow2.off();
}

void RPS::displayLEDs() {

    if(GameScore[0].score == 1) {
        ledRed1.on();
    } 
    else if(GameScore[0].score == 2) {
        ledRed1.on();
        ledRed2.on();
    }

    if(GameScore[1].score == 1) {
        ledYellow1.on();
    } 
    else if(GameScore[1].score == 2) {
        ledYellow1.on();
        ledYellow2.on();

    }

    if(GameScore[2].score == 1) {
        ledBlue.on();
    }

}

void RPS::resetGame() {
    for (int i = 0; i < 3; i++)
    {
        GameScore[i].score=0;
    }

    state=waitForUserInput;
    allLEDsOff();
    resetServos();
    
}

void RPS::checkGameplay() {
    if(GameScore[0].score >= 2 || GameScore[1].score >= 2) {
        state = end;
    } else {
        state = waitForUserInput;
    }

    Serial.println("RESULT");
    Serial.print("Won: ");
    Serial.println(GameScore[0].score);
    Serial.print("Lost: ");
    Serial.println(GameScore[1].score);
    Serial.print("Tied: ");
    Serial.println(GameScore[2].score);
    Serial.println("--------------------------");

    displayLEDs();
}

String RPS::checkUserWin(String user, String bot)
{
    String outcome = "tied";

    if (user == bot)
    {
        // tied
        GameScore[2].score += 1;
        outcome = "tied";
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
                    outcome = "won";
                }
                else
                {
                    GameScore[1].score += 1;
                    outcome = "lost";
                }
            }
        }
    }

    return outcome;
}

void RPS::attachServos(uint8_t scissorsPin, uint8_t rockPin, uint8_t paperPin)
{
    servoScissors.attach(scissorsPin);
    servoRock.attach(rockPin);
    servoPaper.attach(paperPin);

    resetServos();
}

void RPS::showMove(const String &move)
{
    // Little "shake" build-up before revealing the final move, for suspense
    // (mimics the classic "rock... paper... scissors... shoot!" rhythm)
    for (int i = 0; i < 2; i++)
    {
        servoScissors.write(SHOW_ANGLE);
        servoRock.write(SHOW_ANGLE);
        servoPaper.write(SHOW_ANGLE);
        delay(90);

        servoScissors.write(NEUTRAL_ANGLE);
        servoRock.write(NEUTRAL_ANGLE);
        servoPaper.write(NEUTRAL_ANGLE);
        delay(90);
    }

    // Now reveal the bot's actual move
    if (move == "scissors")
    {
        servoScissors.write(SHOW_ANGLE);
    }
    else if (move == "rock")
    {
        servoRock.write(SHOW_ANGLE);
    }
    else if (move == "paper")
    {
        servoPaper.write(SHOW_ANGLE);
    }
}

void RPS::resetServos()
{
    servoScissors.write(NEUTRAL_ANGLE);
    servoRock.write(NEUTRAL_ANGLE);
    servoPaper.write(NEUTRAL_ANGLE);
}