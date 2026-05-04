#include "Starfield.hpp"
#include <cstdlib>

/*
 * Starfield.cpp
 * SRP : ONE reason to change — background star appearance or scroll speed.
 */

/* Constructor replaces InitStars() */
Starfield::Starfield(int screenWidth, int screenHeight)
{
    for (auto& s : stars_) {
        s.pos   = { static_cast<float>(rand() % screenWidth),
                    static_cast<float>(rand() % screenHeight) };
        s.speed = static_cast<float>(rand() % 3 + 1);
    }
}

void Starfield::update(int screenWidth, int screenHeight)
{
    for (auto& s : stars_) {
        s.pos.y += s.speed;
        if (s.pos.y > static_cast<float>(screenHeight)) {
            s.pos.x = static_cast<float>(rand() % screenWidth);
            s.pos.y = 0.0f;
            s.speed = static_cast<float>(rand() % 3 + 1);
        }
    }
}

/* LSP : const — no side-effects in a draw call */
void Starfield::draw() const
{
    for (const auto& s : stars_)
        DrawPixel(static_cast<int>(s.pos.x),
                  static_cast<int>(s.pos.y), WHITE);
}
