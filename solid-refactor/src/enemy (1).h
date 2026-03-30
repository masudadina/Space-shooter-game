#pragma once

/* enemy.h
   SRP : Only enemy state and behaviour declared here.
   OCP : RespawnEnemy() isolates the respawn algorithm — adding new
         respawn patterns (formation, wave) extends this file without
         modifying main.c.
   LSP : DrawEnemies takes a const array — drawing never mutates enemy
         state; every caller can depend on that contract.
   DIP : Speed is always passed in as a parameter; this module never
         reads a global difficulty variable */
 

#include "raylib.h"
#include <stdbool.h>

#define MAX_ENEMIES 10

typedef struct {
    Vector2 pos;
    bool    active;
    float   speed;
} Enemy;

// Initialise all enemies at random off-screen positions 
void InitEnemies(Enemy enemies[], float speed, int screenWidth);

// Move enemies downward and drift toward the player 
void UpdateEnemies(Enemy enemies[], float speed,
                   int screenWidth, int screenHeight,
                   Vector2 playerPos);

// OCP : Single-enemy respawn extracted so the rule lives in one place 
void RespawnEnemy(Enemy* enemy, float speed, int screenWidth);

// LSP : const — drawing must not mutate enemy state 
void DrawEnemies(const Enemy enemies[], Texture2D texture, float scale);
