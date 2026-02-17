#include "player.h"     // Includes the header file for player management
#include "bullet.h"     // Includes the header file for bullet management


/* Initializes the player by setting its position to the bottom center of
   the screen and speed */
void InitPlayer(Player *player, int screenWidth, int screenHeight) {
    player->pos = (Vector2){ screenWidth / 2.0f, screenHeight - 60 };
    player->speed = 5.0f;
}


/* Updates the player's position based on keyboard input and ensures it stays
   within screen bounds */
void UpdatePlayer(Player *player, int screenWidth, int screenHeight) {
    if (IsKeyDown(KEY_LEFT) && player->pos.x > 0) {
        player->pos.x -= player->speed;
    }

    if (IsKeyDown(KEY_RIGHT) && player->pos.x < screenWidth - 64) {
        player->pos.x += player->speed;
    }
    if (IsKeyDown(KEY_UP) && player->pos.y > 0) {
        player->pos.y -= player->speed;
    }
    if (IsKeyDown(KEY_DOWN) && player->pos.y < screenHeight - 64) {
        player->pos.y += player->speed;
    }
}


/* Draws the player on the screen using the provided texture and scale.
   If the shield is active, also draws a circle around the player to indicate
   the shield status */
void DrawPlayer(Player *player, Texture2D texture, float scale,bool shieldActive){
    DrawTextureEx(texture, player->pos, 0.0f, scale, WHITE);
    if (shieldActive){
        DrawCircleLines(player->pos.x + texture.width * scale / 2,
                        player->pos.y + texture.height * scale / 2,
                        25, SKYBLUE);
    }
}
