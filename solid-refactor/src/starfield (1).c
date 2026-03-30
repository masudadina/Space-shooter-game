#include "starfield.h"
#include <stdlib.h>


 /* starfield.c
   SRP : ONE reason to change — background star appearance or scroll speed */
 

void InitStars(Star stars[], int screenWidth, int screenHeight) {
    for (int i = 0; i < MAX_STARS; i++) {
        stars[i].pos   = (Vector2){ (float)(rand() % screenWidth),
                                    (float)(rand() % screenHeight) };
        stars[i].speed = (float)(rand() % 3 + 1);
    }
}

void UpdateStars(Star stars[], int screenWidth, int screenHeight) {
    for (int i = 0; i < MAX_STARS; i++) {
        stars[i].pos.y += stars[i].speed;
        if (stars[i].pos.y > (float)screenHeight) {
            stars[i].pos.x = (float)(rand() % screenWidth);
            stars[i].pos.y = 0.0f;
            stars[i].speed = (float)(rand() % 3 + 1);
        }
    }
}

/* LSP : const — no side-effects in a draw call */
void DrawStars(const Star stars[]) {
    for (int i = 0; i < MAX_STARS; i++)
        DrawPixel((int)stars[i].pos.x, (int)stars[i].pos.y, WHITE);
}
