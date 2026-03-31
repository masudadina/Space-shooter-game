#include "Enemy.hpp"
#include <cstdlib>


/* Enemy.cpp
   SRP : ONE reason to change — enemy movement, spawn, or appearance.
   OCP : respawn() is the single place to edit for spawn behaviour.
   LSP : All enemies honour the same movement contract (drift + fall) */
 

/* OCP : centralised respawn — extend here, never in main.cpp */
void EnemyManager::respawn(Enemy& enemy, float speed, int screenWidth)
{
    enemy.pos    = { static_cast<float>(rand() % (screenWidth - 64)),
                     -50.0f - static_cast<float>(rand() % 150) };
    enemy.speed  = speed;
    enemy.active = true;
}

void EnemyManager::init(float speed, int screenWidth)
{
    for (auto& e : enemies_)
        respawn(e, speed, screenWidth);
}

void EnemyManager::update(float speed, int screenWidth,
                           int screenHeight, Vector2 playerPos)
{
    for (auto& e : enemies_) {
        if (!e.active) continue;

        /* LSP : every enemy obeys the same contract — drift then fall */
        if (rand() % 100 < 50)
            e.pos.x += (playerPos.x > e.pos.x) ? 1.0f : -1.0f;

        e.pos.y += e.speed;

        if (e.pos.y > static_cast<float>(screenHeight))
            respawn(e, speed, screenWidth);
    }
}

/* LSP : const — no mutation of enemy state inside a draw call */
void EnemyManager::draw(Texture2D texture, float scale) const
{
    for (const auto& e : enemies_)
        if (e.active)
            DrawTextureEx(texture, e.pos, 0.0f, scale, WHITE);
}
