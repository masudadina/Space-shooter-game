#include "enemy.h"
#include <stdlib.h>


/* enemy.c
   SRP  : ONE reason to change — enemy movement, appearance, or spawn rules.
   OCP  : RespawnEnemy is the single place to edit if spawn behaviour changes.
   LSP  : All enemies honour the same movement contract (drift + fall) */
 

// OCP: Centralised respawn — extend here, not in main.c 
void RespawnEnemy(Enemy* enemy, float speed, int screenWidth) {
    enemy->pos    = (Vector2){
        (float)(rand() % (screenWidth - 64)),
        -50.0f - (float)(rand() % 150)
    };
    enemy->speed  = speed;
    enemy->active = true;
}

void InitEnemies(Enemy enemies[], float speed, int screenWidth) {
    for (int i = 0; i < MAX_ENEMIES; i++)
        RespawnEnemy(&enemies[i], speed, screenWidth);
}

void UpdateEnemies(Enemy enemies[], float speed,
                   int screenWidth, int screenHeight,
                   Vector2 playerPos) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) continue;

        // LSP : every enemy honours the same contract — drift + fall 
        if (rand() % 100 < 50) {
            enemies[i].pos.x +=
                (playerPos.x > enemies[i].pos.x) ? 1.0f : -1.0f;
        }
        enemies[i].pos.y += enemies[i].speed;

        if (enemies[i].pos.y > (float)screenHeight)
            RespawnEnemy(&enemies[i], speed, screenWidth);
    }
}

// LSP : const — no mutation of enemy state inside a draw call 
void DrawEnemies(const Enemy enemies[], Texture2D texture, float scale) {
    for (int i = 0; i < MAX_ENEMIES; i++)
        if (enemies[i].active)
            DrawTextureEx(texture, enemies[i].pos, 0.0f, scale, WHITE);
}
