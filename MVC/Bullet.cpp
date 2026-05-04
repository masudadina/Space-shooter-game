#include "Bullet.hpp"

/* Bullet.cpp
   SRP : ONE reason to change — bullet speed, appearance, or firing logic.
         Collision outcomes are the caller's responsibility (Collision.hpp) */
 

void BulletManager::fire(Vector2 playerPos, float playerWidth)
{
    for (auto& b : bullets_) {
        if (!b.active) {
            b.pos    = { playerPos.x + playerWidth / 2.0f - 2.0f,
                         playerPos.y };
            b.active = true;
            break;                          /* one bullet per key-press */
        }
    }
}

void BulletManager::update()
{
    for (auto& b : bullets_) {
        if (!b.active) continue;
        b.pos.y -= 5.0f;
        if (b.pos.y < 0.0f)
            b.active = false;
    }
}

/* LSP : const — drawing has no side-effects on bullet state */
void BulletManager::draw() const
{
    for (const auto& b : bullets_)
        if (b.active)
            DrawCircle(static_cast<int>(b.pos.x),
                       static_cast<int>(b.pos.y), 5, YELLOW);
}
