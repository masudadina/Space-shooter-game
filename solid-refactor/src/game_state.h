#pragma once

/* game_state.h
   SRP : Score, high-score and level progression are the ONLY concern
         of this module.  Nothing else in the project touches these
         variables directly.
   OCP : To change the level-up rule (e.g. score-based instead of
         time-based) you only edit GameState_TryLevelUp() here — the
         game loop in main.c is untouched */
 

#include <stdbool.h>

typedef struct {
    int    score;
    int    highScore;
    int    level;
    double lastLevelUpTime;
} GameState;

// Initialise all fields to zero / starting values 
void GameState_Init(GameState* gs, double startTime);

// Add points; automatically updates highScore if beaten 
void GameState_AddScore(GameState* gs, int points);


/* OCP — level-up logic lives here only.
   Returns true (and increments level) when intervalSeconds has elapsed */
 
bool GameState_TryLevelUp(GameState* gs, double currentTime,
                          double intervalSeconds);

// Reset score and level after a player death; highScore is preserved 
void GameState_Reset(GameState* gs, double currentTime);
