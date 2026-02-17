#pragma once        // Ensures this header file avoiding redefinition errors

#include "raylib.h"     //provides core graphics/game functions and utilities
#include <stdbool.h>    //for using boolean data type

#define MAX_BULLETS 10


/* Bullet structure to hold position and active state */
typedef struct {
    Vector2 pos;
    bool active;
} Bullet;


/* Function prototypes for bullet management */
void FireBullet(Bullet bullets[], Vector2 playerPos, float playerWidth);

void UpdateBullets(Bullet bullets[]);

void DrawBullets(Bullet bullets[]);
