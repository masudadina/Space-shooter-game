#include "Collision.hpp"

/* Collision.cpp
   SRP : ONE reason to change — how overlaps are detected and how
         collision rectangles are sized for each object type */
 

bool Collision::check(Rectangle a, Rectangle b)
{
    return CheckCollisionRecs(a, b);
}

Rectangle Collision::makeBulletRect(float x, float y)
{
    return Rectangle{ x - 2.0f, y - 2.0f, 4.0f, 4.0f };
}

Rectangle Collision::makePlayerRect(float x, float y,
                                    float texW, float texH, float scale)
{
    return Rectangle{ x, y, texW * scale, texH * scale };
}

Rectangle Collision::makeEnemyRect(float x, float y,
                                   float texW, float texH, float scale)
{
    return Rectangle{ x, y, texW * scale, texH * scale };
}

Rectangle Collision::makeShieldPickupRect(float x, float y)
{
    return Rectangle{ x - 10.0f, y - 10.0f, 20.0f, 20.0f };
}
