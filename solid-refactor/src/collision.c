#include "collision.h"


/* collision.c
   SRP : ONE reason to change — how overlap between two rectangles
         is detected, and how those rectangles are sized */
 

bool CheckCollision(Rectangle a, Rectangle b) {
    return CheckCollisionRecs(a, b);
}

Rectangle MakeBulletRect(float x, float y) {
    return (Rectangle){ x - 2.0f, y - 2.0f, 4.0f, 4.0f };
}

Rectangle MakePlayerRect(float x, float y,
                         float texW, float texH, float scale) {
    return (Rectangle){ x, y, texW * scale, texH * scale };
}

Rectangle MakeEnemyRect(float x, float y,
                        float texW, float texH, float scale) {
    return (Rectangle){ x, y, texW * scale, texH * scale };
}

Rectangle MakeShieldPickupRect(float x, float y) {
    return (Rectangle){ x - 10.0f, y - 10.0f, 20.0f, 20.0f };
}
