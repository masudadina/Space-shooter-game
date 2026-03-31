#include "Player.hpp"

/*
 * Player.cpp
 * SRP : ONE reason to change — how the player moves or is drawn.
 */

Player::Player(int screenWidth, int screenHeight)
    : pos{ static_cast<float>(screenWidth / 2),
           static_cast<float>(screenHeight - 60) },
      speed(5.0f)
{}

void Player::update(int screenWidth, int screenHeight)
{
    if (IsKeyDown(KEY_LEFT)  && pos.x > 0.0f)
        pos.x -= speed;
    if (IsKeyDown(KEY_RIGHT) && pos.x < static_cast<float>(screenWidth  - 64))
        pos.x += speed;
    if (IsKeyDown(KEY_UP)    && pos.y > 0.0f)
        pos.y -= speed;
    if (IsKeyDown(KEY_DOWN)  && pos.y < static_cast<float>(screenHeight - 64))
        pos.y += speed;
}

/* LSP : const — this method never mutates player state */
void Player::draw(Texture2D texture, float scale, bool shieldActive) const
{
    DrawTextureEx(texture, pos, 0.0f, scale, WHITE);
    if (shieldActive) {
        DrawCircleLines(
            static_cast<int>(pos.x + texture.width  * scale / 2.0f),
            static_cast<int>(pos.y + texture.height * scale / 2.0f),
            25, SKYBLUE);
    }
}
