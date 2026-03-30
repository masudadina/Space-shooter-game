#pragma once

/* bullet.h
   SRP : Only bullet state and behaviour declared here.
   ISP : No player or enemy types pulled in — only what a bullet needs.
   LSP : DrawBullets takes a const array — the contract is that drawing
         never mutates state; any caller can rely on this guarantee */
 

#include "raylib.h"
#include <stdbool.h>

#define MAX_BULLETS 10

typedef struct {
    Vector2 pos;
    bool    active;
} Bullet;

// Fire one bullet from the player's position 
void FireBullet  (Bullet bullets[], Vector2 playerPos, float playerWidth);

// Move all active bullets upward; deactivate off-screen ones 
void UpdateBullets(Bullet bullets[]);

// Draw all active bullets — const enforces no side-effects (LSP) 
void DrawBullets (const Bullet bullets[]);
