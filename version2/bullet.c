#include "bullet.h"         // Includes the header file for bullet management


/* Fires a bullet from the player's position by searching the inactive ones, 
placing the bullet at the horizontal center of the player, 
find an inactive bullet to reuse,
Fire only one bullet at a time by using break*/
void FireBullet(Bullet bullets[] , Vector2 playerPos , float playerWidth) {
    for (int i = 0 ; i < MAX_BULLETS ; i++) {
        if (!bullets[i].active) {
            bullets[i].pos = (Vector2){ playerPos.x + playerWidth / 2 - 2 , playerPos.y };
            bullets[i].active = true;
            break;
        }
    }
}


// Move bullets and deactivate off-screen ones
void UpdateBullets(Bullet bullets[]) {
    for (int i = 0 ; i < MAX_BULLETS ; i++) {
        if (bullets[i].active) {
            bullets[i].pos.y -= 5;      // Move bullet upwards
            if (bullets[i].pos.y < 0) {
                bullets[i].active = false;
            }
        }
    }
}


// Draw all active bullets
void DrawBullets(Bullet bullets[]) {
    for (int i = 0 ; i < MAX_BULLETS ; i++) {
        if (bullets[i].active) {
            DrawCircle(bullets[i].pos.x, bullets[i].pos.y, 5, YELLOW);
        }
    }
}
