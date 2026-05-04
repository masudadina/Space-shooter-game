#pragma once
/*
 * Starfield.hpp
 *
 * C → C++ conversion:
 *   C  : typedef struct Star + free functions InitStars /
 *        UpdateStars / DrawStars + raw array Star stars[MAX_STARS]
 *   C++ : struct Star (data) + class Starfield (behaviour)
 *         Starfield owns a std::array — no raw array passed around.
 *
 * SOLID (unchanged):
 *   SRP : Pure visual background effect — no game logic whatsoever.
 *   ISP : Completely isolated; nothing else is needed to scroll stars.
 *   LSP : draw() is const — no state mutation in a draw call.
 */

#include "raylib.h"
#include <array>

static constexpr int MAX_STARS = 100;

/* Plain data — identical to the C struct */
struct Star
{
    Vector2 pos   = { 0.0f, 0.0f };
    float   speed = 1.0f;
};

/*
 * Starfield owns the star array and all star behaviour.
 * Replaces the three C free functions.
 */
class Starfield
{
public:
    /* Constructor replaces InitStars() */
    Starfield(int screenWidth, int screenHeight);

    /* update() replaces UpdateStars() */
    void update(int screenWidth, int screenHeight);

    /* draw() replaces DrawStars() — const: LSP guarantee */
    void draw() const;

private:
    std::array<Star, MAX_STARS> stars_{};
};
