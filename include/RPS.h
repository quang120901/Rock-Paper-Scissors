#ifndef RPS_H
#define RPS_H

#include <Arduino.h>

class RPS
{
private:
    String solutions[3][2] = {
        {"rock", "scissors"},
        {"paper", "rock"},
        {"scissors", "paper"}};

    struct scores
    {
        String match;
        int score;
    };

    int previousScore;

public:
    enum states
    {
        playing,
        waitForUserInput,
        end
    } state;

    String chosenInput[3] = {"rock", "paper", "scissors"};

    scores GameScore[3]{
        {"won", 0},
        {"lost", 0},
        {"tied", 0}};

    void allLEDsOff();
    void displayLEDs();
    void checkGameplay();
    // Now returns the outcome of this single round: "won", "lost" or "tied"
    String checkUserWin(String user, String bot);
    void resetGame();
};

#endif