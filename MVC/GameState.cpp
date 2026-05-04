#include "GameState.hpp"

/* GameState.cpp
   SRP : ONE reason to change — scoring and level-progression rules */
 

/* Constructor replaces GameState_Init() */
GameState::GameState(double startTime)
    : score(0), highScore(0), level(1), lastLevelUpTime(startTime)
{}

void GameState::addScore(int points)
{
    score += points;
    if (score > highScore)
        highScore = score;
}

/* OCP : level-up condition lives here only — change without touching main.cpp */
bool GameState::tryLevelUp(double currentTime, double intervalSeconds)
{
    if (currentTime - lastLevelUpTime >= intervalSeconds) {
        level++;
        lastLevelUpTime = currentTime;
        return true;
    }
    return false;
}

void GameState::reset(double currentTime)
{
    score           = 0;
    level           = 1;
    lastLevelUpTime = currentTime;
    /* highScore intentionally preserved across deaths */
}
