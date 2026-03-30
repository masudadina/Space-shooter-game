#include "game_state.h"

/* game_state.c
   SRP : ONE reason to change — scoring / level rules */
 

void GameState_Init(GameState* gs, double startTime) {
    gs->score           = 0;
    gs->highScore       = 0;
    gs->level           = 1;
    gs->lastLevelUpTime = startTime;
}

void GameState_AddScore(GameState* gs, int points) {
    gs->score += points;
    if (gs->score > gs->highScore)
        gs->highScore = gs->score;
}

// OCP: change the interval or promotion condition only here 
bool GameState_TryLevelUp(GameState* gs, double currentTime,
                          double intervalSeconds) {
    if (currentTime - gs->lastLevelUpTime >= intervalSeconds) {
        gs->level++;
        gs->lastLevelUpTime = currentTime;
        return true;
    }
    return false;
}

void GameState_Reset(GameState* gs, double currentTime) {
    gs->score           = 0;
    gs->level           = 1;
    gs->lastLevelUpTime = currentTime;
    // highScore intentionally preserved across deaths 
}
