#include "enemy.h"          // Includes the header file for enemy management
#include <stdlib.h>         // For random number generation


/* Initializes the enemies by setting their position,active state and speed*/
void InitEnemies(Enemy enemies[], float speed, int screenWidth) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].pos = (Vector2){ rand() % (screenWidth - 64), -50 - rand() % 150 };
        enemies[i].active = true;
        enemies[i].speed = speed;
    }
}


/* Updates the enemies by moving them downwards and
    adding a chance to move horizontally towards the player*/
void UpdateEnemies(Enemy enemies[],float speed,int screenWidth,int screenHeight,Vector2 playerPos){
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) {
            continue;
        }
        
        if (rand() % 100 < 50) {            // 50% chance to move horizontally towards the player
            if (playerPos.x > enemies[i].pos.x) {
                enemies[i].pos.x += 1;      // Move right if player is to the right 
            }
            else {                          // Move left if player is to the left 
                enemies[i].pos.x -= 1;
            }
        }

        enemies[i].pos.y += enemies[i].speed;   // Move enemy downwards

        // If enemy goes off-screen,reset its position 
        if (enemies[i].pos.y > screenHeight) {
            enemies[i].pos = (Vector2){ rand() % (screenWidth - 64), -50 - rand() % 150 };
            enemies[i].speed = speed;
        }
    }
}


/* Draws all active enemies on the screen using the provided texture and scale */
void DrawEnemies(Enemy enemies[] , Texture2D texture , float scale) {
    for (int i = 0; i < MAX_ENEMIES; i++)
        if (enemies[i].active)
            DrawTextureEx(texture, enemies[i].pos, 0.0f, scale, WHITE);
}
