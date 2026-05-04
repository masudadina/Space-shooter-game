#pragma once
/*
 * Player.hpp
 *
 * C → C++ conversion:
 *   C  : typedef struct Player + free functions InitPlayer /
 *        UpdatePlayer / DrawPlayer
 *   C++ : class Player — data + behaviour in one self-contained class.
 *         Constructor replaces InitPlayer().
 *
 * SOLID (unchanged):
 *   SRP : Only player position, movement, and appearance live here.
 *         Shield pickup state lives in ShieldPickup (ISP — separated).
 *   LSP : draw() is const — drawing never changes player state.
 */

#include "raylib.h"

class Player
{
public:
    Vector2 pos;
    float   speed;

    /* Constructor replaces InitPlayer() */
    Player(int screenWidth, int screenHeight);

    /* update() replaces UpdatePlayer() */
    void update(int screenWidth, int screenHeight);

    /* draw() replaces DrawPlayer() — const: LSP guarantee */
    void draw(Texture2D texture, float scale, bool shieldActive) const;
};
