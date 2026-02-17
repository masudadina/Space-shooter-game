#include "starfield.h"   // Includes the header file for starfield management
#include <stdlib.h>      // For random number generation


/* Initializes the starfield by randomly placing stars across the screen and 
   assigning them random speeds */
void InitStars(Star stars[], int screenWidth, int screenHeight) {
    for (int i = 0; i < MAX_STARS; i++) {
        stars[i].pos = (Vector2){rand() % screenWidth,rand() % screenHeight};
        stars[i].speed = (float)(rand() % 3 + 1);
    }
}


/* Updates the starfield by moving stars downwards and resetting their position 
   to the top when they go off-screen */
void UpdateStars(Star stars[], int screenWidth, int screenHeight) {
    for (int i = 0; i < MAX_STARS; i++) {
        stars[i].pos.y += stars[i].speed;
        if (stars[i].pos.y > screenHeight) {
            stars[i].pos.x = rand() % screenWidth;
            stars[i].pos.y = 0;
            stars[i].speed = (float)(rand() % 3 + 1);
        }
    }
}


/* Draws the stars on the screen as white pixels */
void DrawStars(Star stars[]) {
    for (int i = 0; i < MAX_STARS; i++)
        DrawPixel(stars[i].pos.x, stars[i].pos.y, WHITE);
}
