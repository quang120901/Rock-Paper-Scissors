#ifndef RPS_H
#define RPS_H

#include <Arduino.h>
#include <ESP32Servo.h>

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

    // Servos, ordered left-to-right to match the <select> order in
    // index.html: Scissors, Rock, Paper.
    Servo servoScissors;
    Servo servoRock;
    Servo servoPaper;

    static const int NEUTRAL_ANGLE = 0;
    static const int SHOW_ANGLE = 90;

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

    // Attach the 3 servos to their pins, matching the left-to-right physical
    // order on the breadboard (Rock, Paper, Scissors when wired per
    // diagram.json's pin 14/27/25 layout — see the attachServos() call site).
    void attachServos(uint8_t scissorsPin, uint8_t rockPin, uint8_t paperPin);
    // Rotate the servo matching "move" ("scissors"/"rock"/"paper") to show it,
    // and reset the other two back to neutral.
    void showMove(const String &move);
    // Reset all 3 servos back to their neutral (resting) position.
    void resetServos();
};

#endif