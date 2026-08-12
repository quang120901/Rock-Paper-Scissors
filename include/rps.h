#ifndef rps_h
#define rps_h

#include <Arduino.h>

class rps
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
        endE
    };

    String chosenInput[3] = {"rock", "paper", "scissors"};

    scores GameScore[3]
    {
        {"won", 0},
            {"lost", 0},
            {"tied", 0}
    };

    void allLEDsOff();
    void turnAllLEDsOn();
    void checkGameplay();
    void checkUserWin(String user, String bot);
    void resetGame();
};

#endif