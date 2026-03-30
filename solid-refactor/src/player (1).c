#include "player.h"

/* player.c
   SRP : ONE reason to change — how the player moves or is drawn */
 
void InitPlayer(Player* player, int screenWidth, int screenHeight) {
    player->pos   = (Vector2){ (float)(screenWidth / 2),
                               (float)(screenHeight - 60) };
    player->speed = 5.0f;
}

void UpdatePlayer(Player* player, int screenWidth, int screenHeight) {
    if (IsKeyDown(KEY_LEFT)  && player->pos.x > 0.0f)
        player->pos.x -= player->speed;
    if (IsKeyDown(KEY_RIGHT) && player->pos.x < (float)(screenWidth  - 64))
        player->pos.x += player->speed;
    if (IsKeyDown(KEY_UP)    && player->pos.y > 0.0f)
        player->pos.y -= player->speed;
    if (IsKeyDown(KEY_DOWN)  && player->pos.y < (float)(screenHeight - 64))
        player->pos.y += player->speed;
}


/* LSP : const pointer — this function never changes player state */
void DrawPlayer(const Player* player, Texture2D texture,
                float scale, bool shieldActive) {
    DrawTextureEx(texture, player->pos, 0.0f, scale, WHITE);
    if (shieldActive) {
        DrawCircleLines(
            (int)(player->pos.x + texture.width  * scale / 2.0f),
            (int)(player->pos.y + texture.height * scale / 2.0f),
            25, SKYBLUE);
    }
}
