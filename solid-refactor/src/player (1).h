#pragma once

/* player.h
   SRP : Only player position, movement, and appearance declared here.
         Shield pickup state lives in shield.h — the two concerns are
         deliberately separated (ISP).
   LSP : DrawPlayer accepts a const pointer — drawing never changes
         player state; any caller can depend on that guarantee */
 

#include "raylib.h"
#include <stdbool.h>

typedef struct {
    Vector2 pos;
    float   speed;
} Player;

/* Place player at the bottom-centre of the screen */
void InitPlayer  (Player* player, int screenWidth, int screenHeight);

/* Move player based on arrow-key input; clamp to screen edges */
void UpdatePlayer(Player* player, int screenWidth, int screenHeight);

/* LSP : const — draw must not mutate player state */
void DrawPlayer  (const Player* player, Texture2D texture,
                  float scale, bool shieldActive);
