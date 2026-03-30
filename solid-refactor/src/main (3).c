/* main.c  —  Space Shooter  (SOLID-refactored)
   =====================================================================
   SRP  : main.c ONLY orchestrates the game loop.  Every other concern
          (scoring, collision, audio, rendering, shield timing) is
          delegated to a dedicated module.
  
   OCP  : EnemySpeed() is the single place that encodes the difficulty
          curve.  Changing it never touches any module implementation.
  
   LSP  : All Draw* calls receive const pointers/arrays — the contract
          that drawing never mutates state is enforced at compile time.
  
   ISP  : main.c only includes the headers it actually uses.  No module
          pulls in another module's concerns through a fat interface.
  
   DIP  : main.c depends on abstractions only:
            CheckCollision()      instead of CheckCollisionRecs()
            Audio_Play*()         instead of PlaySound()
            Renderer_Init/Unload  instead of LoadTexture/UnloadTexture
            PlayerShield_Update() instead of raw time comparison
            GameState_*()         instead of raw int arithmetic
   ===================================================================== */
 

#include "raylib.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "shield.h"
#include "starfield.h"
#include "renderer.h"
#include "audio.h"
#include "collision.h"
#include "game_state.h"

#include <stdlib.h>
#include <time.h>


//Screen constants — change resolution in exactly one place
static const int SCREEN_W = 1600;
static const int SCREEN_H = 850;


/* OCP — difficulty curve lives here only.
   To change how speed scales with level, edit only this function */
  
static float EnemySpeed(int level) {
    return 2.0f + 0.5f * (float)(level - 1);
}


//SRP — start screen is its own self-contained function
static void RunStartScreen(void) {
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

/* =========================  main  =================================*/
   
int main(void) {
    InitWindow(SCREEN_W, SCREEN_H, "Space Shooter");
    InitAudioDevice();
    SetTargetFPS(60);
    srand((unsigned int)time(NULL));

    /* DIP : asset paths are string constants — swap assets without
             touching any module implementation */
    Renderer     rend;
    AudioManager audio;
    Renderer_Init(&rend,  "assets/player.png",   "assets/enemy.png");
    Audio_Init   (&audio, "assets/shoot.wav",
                           "assets/explosion.wav",
                           "assets/levelup.wav");


    /* ------------------------ Game objects ------------------------ */
    Player       player;
    Bullet       bullets[MAX_BULLETS];
    Enemy        enemies[MAX_ENEMIES];
    ShieldPickup shield;
    Star         stars[MAX_STARS];
    

    /* Zero-initialise arrays so no garbage active-flags */
    for (int i = 0; i < MAX_BULLETS; i++) bullets[i].active = false;
    for (int i = 0; i < MAX_ENEMIES; i++) enemies[i].active = false;


    /* ------------------------- Initialise ------------------------- */
    InitPlayer   (&player, SCREEN_W, SCREEN_H);
    InitEnemies  (enemies, EnemySpeed(1), SCREEN_W);
    ShieldPickup_Init(&shield);
    InitStars    (stars, SCREEN_W, SCREEN_H);

    /* SRP : score/level state lives in its own module */
    GameState gs;
    GameState_Init(&gs, GetTime());

    bool shieldActive = false;

    /* --------------------------- Start screen --------------------- */
    RunStartScreen();


    /* ================================================================
                            Main game loop
            Each numbered block has exactly ONE responsibility (SRP).
       ================================================================ */
    while (!WindowShouldClose()) {

        double currentTime = GetTime();

        /* 1. Player input & movement */
        UpdatePlayer(&player, SCREEN_W, SCREEN_H);

        /* 2. Level progression
              OCP : the rule is inside GameState_TryLevelUp, not here */
        if (GameState_TryLevelUp(&gs, currentTime, 60.0))
            Audio_PlayLevelUp(&audio);

        /* 3. Shooting */
        if (IsKeyPressed(KEY_SPACE)) {
            FireBullet(bullets, player.pos,
                       (float)rend.playerTexture.width * rend.playerScale);
            Audio_PlayShoot(&audio);
        }

        /* 4. Update all world objects */
        UpdateBullets(bullets);
        UpdateEnemies(enemies, EnemySpeed(gs.level),
                      SCREEN_W, SCREEN_H, player.pos);
        UpdateStars  (stars, SCREEN_W, SCREEN_H);
        ShieldPickup_Update(&shield, currentTime);

        /* DIP : shield-timer abstracted — no raw time arithmetic here */
        PlayerShield_Update(&shieldActive, &shield, currentTime);


        /* ------------------------------------------------------------
            5. Bullet vs Enemy collisions
              DIP : rects built through collision.h, not inline
           ------------------------------------------------------------ */
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (!bullets[i].active) continue;

            Rectangle bRect = MakeBulletRect(bullets[i].pos.x,
                                             bullets[i].pos.y);
            for (int j = 0; j < MAX_ENEMIES; j++) {
                if (!enemies[j].active) continue;

                Rectangle eRect = MakeEnemyRect(
                    enemies[j].pos.x, enemies[j].pos.y,
                    (float)rend.enemyTexture.width,
                    (float)rend.enemyTexture.height,
                    rend.enemyScale);

                if (CheckCollision(bRect, eRect)) {

                    Vector2 killPos   = enemies[j].pos;
                    bullets[i].active = false;

                    /* OCP : RespawnEnemy owns the respawn algorithm */
                    RespawnEnemy(&enemies[j], EnemySpeed(gs.level), SCREEN_W);

                    Audio_PlayExplosion(&audio);

                    /* 60 % chance to drop a shield pickup */
                    if (rand() % 100 < 60)
                        ShieldPickup_Spawn(&shield, killPos, currentTime, 5.0);

                    /* SRP : scoring is GameState's responsibility */
                    GameState_AddScore(&gs, 10);
                    break;
                }
            }
        }


        /* ------------------------------------------------------------
           6. Player vs Enemy collisions
           ------------------------------------------------------------ */
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (!enemies[i].active) continue;

            Rectangle pRect = MakePlayerRect(
                player.pos.x, player.pos.y,
                (float)rend.playerTexture.width,
                (float)rend.playerTexture.height,
                rend.playerScale);

            Rectangle eRect = MakeEnemyRect(
                enemies[i].pos.x, enemies[i].pos.y,
                (float)rend.enemyTexture.width,
                (float)rend.enemyTexture.height,
                rend.enemyScale);

            if (CheckCollision(pRect, eRect)) {
                if (!shieldActive) {
                    /* Full reset */
                    InitPlayer (&player, SCREEN_W, SCREEN_H);
                    InitEnemies(enemies, EnemySpeed(1), SCREEN_W);
                    GameState_Reset(&gs, currentTime);
                } else {
                    shieldActive = false;   /* Shield absorbs one hit */
                }
                break;
            }
        }


        /* ------------------------------------------------------------
           7. Player vs Shield-pickup collision
           ------------------------------------------------------------ */
        if (shield.active) {
            Rectangle pRect = MakePlayerRect(
                player.pos.x, player.pos.y,
                (float)rend.playerTexture.width,
                (float)rend.playerTexture.height,
                rend.playerScale);

            Rectangle sRect = MakeShieldPickupRect(shield.pos.x,
                                                   shield.pos.y);

            if (CheckCollision(pRect, sRect)) {
                shieldActive  = true;
                shield.active = false;
            }
        }


        /* ------------------------------------------------------------
           8. Render
              SRP : each Draw* call knows only how to draw its own type.
              LSP : all Draw* receive const — no mutation guaranteed.
           ------------------------------------------------------------ */
        BeginDrawing();
        ClearBackground(BLACK);

        DrawStars        (stars);
        DrawPlayer       (&player, rend.playerTexture,
                          rend.playerScale, shieldActive);
        DrawBullets      (bullets);
        DrawEnemies      (enemies, rend.enemyTexture, rend.enemyScale);
        ShieldPickup_Draw(&shield);

        /* HUD */
        DrawText(TextFormat("Score: %d",      gs.score),     10, 10, 20, WHITE);
        DrawText(TextFormat("High Score: %d", gs.highScore), 10, 40, 20, GOLD);
        DrawText(TextFormat("Level: %d",      gs.level),     10, 70, 20, WHITE);

        EndDrawing();
    }

    /* ---------------------------- Cleanup ------------------------- */
    Audio_Unload   (&audio);
    Renderer_Unload(&rend);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
