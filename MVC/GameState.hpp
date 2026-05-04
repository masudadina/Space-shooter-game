#pragma once

/* GameState.hpp
 *
 * C → C++ conversion:
 *   C  : typedef struct GameState { ... };  + free functions GameState_*()
 *   C++ : class GameState { private data + public methods }
 *
 * SOLID (unchanged from C version):
 *   SRP : Score, high-score, and level progression are the ONLY concern
 *         of this class. No other file touches these values directly.
 *   OCP : To change the level-up rule, only tryLevelUp() needs editing —
 *         the game loop in main.cpp is completely untouched.
 */

class GameState
{
public:
    int    score;
    int    highScore;
    int    level;
    double lastLevelUpTime;

    /* Constructor replaces GameState_Init() */
    GameState(double startTime);

    /* SRP : each method has one job */
    void addScore  (int points);
    bool tryLevelUp(double currentTime, double intervalSeconds); /* OCP */
    void reset     (double currentTime);
};
