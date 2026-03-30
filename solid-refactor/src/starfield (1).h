#pragma once

/* starfield.h
   SRP : Pure visual background effect — no game logic whatsoever.
   ISP : Completely isolated; no other header is needed to scroll stars.
   LSP : DrawStars takes a const array — no state mutation in a draw call */
 

#include "raylib.h"

#define MAX_STARS 100

typedef struct {
    Vector2 pos;
    float   speed;
} Star;

void InitStars  (Star stars[], int screenWidth, int screenHeight);
void UpdateStars(Star stars[], int screenWidth, int screenHeight);

/* LSP : const — drawing must not mutate star state */
void DrawStars  (const Star stars[]);
