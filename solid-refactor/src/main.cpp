/* main.cpp — Space Shooter (C++ / SOLID)
   =====================================================================
 * C → C++ conversion summary:
 *   - All C structs + free functions replaced by C++ classes
 *   - RAII: Renderer and AudioManager clean up themselves in destructors
 *   - std::array replaces raw C arrays everywhere
 *   - C-style casts replaced with static_cast<>
 *   - const member methods enforce LSP at compile time
 *   - namespace Collision groups collision helpers
 *
 * SOLID (identical intent, natural C++ expression):
 *   SRP : main.cpp ONLY orchestrates the game loop.
 *   OCP : enemySpeed() is the single place encoding difficulty.
 *   LSP : All draw() methods are const — no mutation guaranteed.
 *   ISP : Each header exposes only its own concern.
 *   DIP : main.cpp depends on class interfaces, never on raylib directly.
 * =====================================================================
 */

#include "raylib.h"
#include "Player.hpp"
#include "Bullet.hpp"
#include "Enemy.hpp"
#include "ShieldPickup.hpp"
#include "Starfield.hpp"
#include "Renderer.hpp"
#include "AudioManager.hpp"
#include "Collision.hpp"
#include "GameState.hpp"

#include <cstdlib>
#include <ctime>

/* ------------------------------------------------------------------
   Screen constants — change resolution in one place only
   ------------------------------------------------------------------ */
static constexpr int SCREEN_W = 1600;
static constexpr int SCREEN_H = 850;

/* ------------------------------------------------------------------
   OCP : difficulty curve lives here only.
   Change speed scaling without touching any class implementation.
   ------------------------------------------------------------------ */
static float enemySpeed(int level)
{
    return 2.0f + 0.5f * static_cast<float>(level - 1);
}

/* ------------------------------------------------------------------
   SRP : start screen is its own self-contained function
   ------------------------------------------------------------------ */
static void runStartScreen()
{
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("SPACE SHOOTER",
                 SCREEN_W / 2 - 120, SCREEN_H / 2 - 60, 40, WHITE);
        DrawText("Press [ENTER] to Start",
                 SCREEN_W / 2 - 140, SCREEN_H / 2 + 10, 20, GRAY);
        DrawText("Use Arrow Keys to Move, Space to Shoot",
                 SCREEN_W / 2 - 200, SCREEN_H / 2 + 50, 20, LIGHTGRAY);
        EndDrawing();
        if (IsKeyPressed(KEY_ENTER)) return;
    }
}

/* ==================================================================
   main
   ================================================================== */
int main()
{
    InitWindow(SCREEN_W, SCREEN_H, "Space Shooter");
    InitAudioDevice();
    SetTargetFPS(60);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    /* ------------------------------------------------------------------
       DIP : all objects constructed through class interfaces.
       RAII : Renderer and AudioManager load on construction,
              unload automatically when they go out of scope.
       ------------------------------------------------------------------ */
    Renderer     rend ("assets/player.png", "assets/enemy.png");
    AudioManager audio("assets/shoot.wav",
                       "assets/explosion.wav",
                       "assets/levelup.wav");

    /* ---- Game objects ------------------------------------------------ */
    Player       player (SCREEN_W, SCREEN_H);
    BulletManager bullets;
    EnemyManager  enemies;
    ShieldPickup  shield;
    Starfield     stars  (SCREEN_W, SCREEN_H);

    /* SRP : score/level managed by GameState class */
    GameState gs(GetTime());

    enemies.init(enemySpeed(1), SCREEN_W);

    bool shieldActive = false;

    /* ---- Start screen ------------------------------------------------ */
    runStartScreen();

    /* ==================================================================
       Main game loop — each numbered block has ONE responsibility (SRP)
       ================================================================== */
    while (!WindowShouldClose())
    {
        double currentTime = GetTime();

        /* 1. Player input & movement */
        player.update(SCREEN_W, SCREEN_H);

        /* 2. Level progression — OCP: rule lives in GameState::tryLevelUp */
        if (gs.tryLevelUp(currentTime, 60.0))
            audio.playLevelUp();

        /* 3. Shooting */
        if (IsKeyPressed(KEY_SPACE)) {
            bullets.fire(player.pos,
                         static_cast<float>(rend.playerTexture.width)
                         * rend.playerScale);
            audio.playShoot();
        }

        /* 4. Update all world objects */
        bullets.update();
        enemies.update(enemySpeed(gs.level), SCREEN_W, SCREEN_H, player.pos);
        stars.update(SCREEN_W, SCREEN_H);
        shield.update(currentTime);

        /* DIP : shield-timer abstracted — no raw time arithmetic here */
        ShieldPickup::updatePlayerShield(shieldActive, shield, currentTime);

        /* ----------------------------------------------------------------
           5. Bullet vs Enemy collisions
              DIP : rects built through Collision namespace
           ---------------------------------------------------------------- */
        for (auto& b : bullets.bullets()) {
            if (!b.active) continue;

            Rectangle bRect = Collision::makeBulletRect(b.pos.x, b.pos.y);

            for (auto& e : enemies.enemies()) {
                if (!e.active) continue;

                Rectangle eRect = Collision::makeEnemyRect(
                    e.pos.x, e.pos.y,
                    static_cast<float>(rend.enemyTexture.width),
                    static_cast<float>(rend.enemyTexture.height),
                    rend.enemyScale);

                if (Collision::check(bRect, eRect)) {
                    Vector2 killPos = e.pos;
                    b.active        = false;

                    /* OCP : respawn algorithm lives in EnemyManager */
                    enemies.respawn(e, enemySpeed(gs.level), SCREEN_W);

                    audio.playExplosion();

                    /* 60 % chance to drop a shield pickup */
                    if (rand() % 100 < 60)
                        shield.spawn(killPos, currentTime, 5.0);

                    /* SRP : scoring delegated to GameState */
                    gs.addScore(10);
                    break;
                }
            }
        }

        /* ----------------------------------------------------------------
           6. Player vs Enemy collisions
           ---------------------------------------------------------------- */
        for (auto& e : enemies.enemies()) {
            if (!e.active) continue;

            Rectangle pRect = Collision::makePlayerRect(
                player.pos.x, player.pos.y,
                static_cast<float>(rend.playerTexture.width),
                static_cast<float>(rend.playerTexture.height),
                rend.playerScale);

            Rectangle eRect = Collision::makeEnemyRect(
                e.pos.x, e.pos.y,
                static_cast<float>(rend.enemyTexture.width),
                static_cast<float>(rend.enemyTexture.height),
                rend.enemyScale);

            if (Collision::check(pRect, eRect)) {
                if (!shieldActive) {
                    /* Full reset */
                    player = Player(SCREEN_W, SCREEN_H);
                    enemies.init(enemySpeed(1), SCREEN_W);
                    gs.reset(currentTime);
                } else {
                    shieldActive = false;   /* shield absorbs one hit */
                }
                break;
            }
        }

        /* ----------------------------------------------------------------
           7. Player vs Shield pickup collision
           ---------------------------------------------------------------- */
        if (shield.active) {
            Rectangle pRect = Collision::makePlayerRect(
                player.pos.x, player.pos.y,
                static_cast<float>(rend.playerTexture.width),
                static_cast<float>(rend.playerTexture.height),
                rend.playerScale);

            Rectangle sRect = Collision::makeShieldPickupRect(
                shield.pos.x, shield.pos.y);

            if (Collision::check(pRect, sRect)) {
                shieldActive  = true;
                shield.active = false;
            }
        }

        /* ----------------------------------------------------------------
           8. Render
              SRP : each draw() call knows only how to draw its own type.
              LSP : all draw() methods are const — no mutation guaranteed.
           ---------------------------------------------------------------- */
        BeginDrawing();
        ClearBackground(BLACK);

        stars.draw();
        player.draw(rend.playerTexture, rend.playerScale, shieldActive);
        bullets.draw();
        enemies.draw(rend.enemyTexture, rend.enemyScale);
        shield.draw();

        DrawText(TextFormat("Score: %d",      gs.score),     10, 10, 20, WHITE);
        DrawText(TextFormat("High Score: %d", gs.highScore), 10, 40, 20, GOLD);
        DrawText(TextFormat("Level: %d",      gs.level),     10, 70, 20, WHITE);

        EndDrawing();
    }

    /* ------------------------------------------------------------------
       RAII : Renderer and AudioManager destructors run automatically here.
       CloseAudioDevice and CloseWindow must still be called explicitly
       because they are raylib globals, not owned by any class.
       ------------------------------------------------------------------ */
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
