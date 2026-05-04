#include "GameController.hpp"
#include <cstdlib>
#include <ctime>

GameController::GameController()
    : player(SCREEN_W, SCREEN_H),
      stars(SCREEN_W, SCREEN_H),
      gs(GetTime()),
      rend("assets/player.png", "assets/enemy.png"),
      audio("assets/shoot.wav", "assets/explosion.wav", "assets/levelup.wav"),
      shieldActive(false)
{
    enemies.init(enemySpeed(1), SCREEN_W);
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

float GameController::enemySpeed(int level)
{
    return 2.0f + 0.5f * (level - 1);
}

void GameController::processInput()
{
    player.update(SCREEN_W, SCREEN_H);

    if (IsKeyPressed(KEY_SPACE)) {
        bullets.fire(player.pos,
            rend.playerTexture.width * rend.playerScale);
        audio.playShoot();
    }
}

void GameController::update()
{
    double currentTime = GetTime();

    if (gs.tryLevelUp(currentTime, 60.0))
        audio.playLevelUp();

    bullets.update();
    enemies.update(enemySpeed(gs.level), SCREEN_W, SCREEN_H, player.pos);
    stars.update(SCREEN_W, SCREEN_H);
    shield.update(currentTime);

    ShieldPickup::updatePlayerShield(shieldActive, shield, currentTime);
}

void GameController::handleCollisions()
{
    double currentTime = GetTime();

    // Bullet vs Enemy
    for (auto& b : bullets.bullets()) {
        if (!b.active) continue;

        Rectangle bRect = Collision::makeBulletRect(b.pos.x, b.pos.y);

        for (auto& e : enemies.enemies()) {
            if (!e.active) continue;

            Rectangle eRect = Collision::makeEnemyRect(
                e.pos.x, e.pos.y,
                rend.enemyTexture.width,
                rend.enemyTexture.height,
                rend.enemyScale);

            if (Collision::check(bRect, eRect)) {
                Vector2 killPos = e.pos;
                b.active = false;

                enemies.respawn(e, enemySpeed(gs.level), SCREEN_W);
                audio.playExplosion();

                if (rand() % 100 < 60)
                    shield.spawn(killPos, currentTime, 5.0);

                gs.addScore(10);
                break;
            }
        }
    }
}

void GameController::render()
{
    BeginDrawing();
    ClearBackground(BLACK);

    stars.draw();
    player.draw(rend.playerTexture, rend.playerScale, shieldActive);
    bullets.draw();
    enemies.draw(rend.enemyTexture, rend.enemyScale);
    shield.draw();

    DrawText(TextFormat("Score: %d", gs.score), 10, 10, 20, WHITE);

    EndDrawing();
}

void GameController::run()
{
    InitWindow(SCREEN_W, SCREEN_H, "Space Shooter");
    InitAudioDevice();
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        processInput();     // Controller
        update();           // Model update
        handleCollisions(); // Logic
        render();           // View
    }

    CloseAudioDevice();
    CloseWindow();
}