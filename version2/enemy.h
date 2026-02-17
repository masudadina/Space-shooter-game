#pragma once       // Ensures this header file avoiding redefinition errors

#include "raylib.h"
#include <stdbool.h>

#define MAX_ENEMIES 10


/* Enemy structure to hold position,active state and speed */
typedef struct {
    Vector2 pos;
    bool active;
    float speed;
} Enemy;


/* Function prototypes for enemy management */
void InitEnemies(Enemy enemies[], float speed, int screenWidth);
void UpdateEnemies(Enemy enemies[],float speed,int screenWidth,int screenHeight,Vector2 playerPos);
void DrawEnemies(Enemy enemies[], Texture2D texture, float scale);


