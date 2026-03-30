#include "bullet.h"

 /* bullet.c
    SRP : ONE reason to change — bullet movement speed, appearance, or
          firing logic.
    Collision outcomes are NOT handled here (DIP — caller uses collision.h) */
 

void FireBullet(Bullet bullets[], Vector2 playerPos, float playerWidth) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            // Centre bullet on the player sprite horizontally 
            bullets[i].pos    = (Vector2){
                playerPos.x + playerWidth / 2.0f - 2.0f,
                playerPos.y
            };
            bullets[i].active = true;
            break;          // One bullet per key-press 
        }
    }
}

void UpdateBullets(Bullet bullets[]) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) continue;
        bullets[i].pos.y -= 5.0f;
        if (bullets[i].pos.y < 0.0f)
            bullets[i].active = false;
    }
}

// LSP : const guarantees this function never mutates bullet state 
void DrawBullets(const Bullet bullets[]) {
    for (int i = 0; i < MAX_BULLETS; i++)
        if (bullets[i].active)
            DrawCircle((int)bullets[i].pos.x,
                       (int)bullets[i].pos.y, 5, YELLOW);
}
