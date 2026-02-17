#include "raylib.h"     //Includes all necessary header files for game management,random 
#include "player.h"     //number generation,time calculation and input/output operations 
#include "bullet.h"     
#include "enemy.h"
#include "shield.h"
#include "starfield.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>           //./Debug/SpaceShooter.exe



/* Main entry point of the game */
int main(void)
{
    const int screenWidth = 1600;
    const int screenHeight = 850;

    /* Initialize the game window and audio device,set target FPS
        and seed the random number generator */
    InitWindow(screenWidth, screenHeight, "Space Shooter");
    InitAudioDevice();
    SetTargetFPS(60);

    srand((unsigned int)time(NULL));

    /* Load textures and sounds for the game*/
    Texture2D playerTexture = LoadTexture("assets/player.png");
    Texture2D enemyTexture  = LoadTexture("assets/enemy.png");
    Sound shootSound        = LoadSound("assets/shoot.wav");
    Sound explosionSound    = LoadSound("assets/explosion.wav");
    Sound levelupSound      = LoadSound("assets/levelup.wav");


    /* Define and initialize game objects */
    Player player;
    Bullet bullets[MAX_BULLETS] = {0};
    Enemy enemies[MAX_ENEMIES] = {0};
    Shield shield;
    Star stars[MAX_STARS];


    /* Initialize game objects with their respective initialization functions */
    InitPlayer(&player, screenWidth, screenHeight);
    InitEnemies(enemies, 2.0f, screenWidth);
    InitShield(&shield);
    InitStars(stars, screenWidth, screenHeight);


    /* Game state variables */
    bool startScreen = true;
    bool shieldActive = false;
    int score = 0 , highScore = 0 , level = 1;
    double lastLevelUpTime = GetTime();


    // Start screen loop until the player presses [ENTER] to start the game
    while (startScreen && !WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("SPACE SHOOTER", screenWidth/2 - 120, screenHeight/2 - 60, 40, WHITE);
        DrawText("Press [ENTER] to Start", screenWidth/2 - 140, screenHeight/2 + 10, 20, GRAY);
        DrawText("Use Arrow Keys to Move,Space to Shoot",screenWidth/2-200,screenHeight/2+50,20,LIGHTGRAY);
        EndDrawing();

        if (IsKeyPressed(KEY_ENTER)) startScreen = false;
    }

    // Main game loop runs until the window is closed
    while (!WindowShouldClose()) 
    {
        double currentTime = GetTime();

        UpdatePlayer(&player, screenWidth, screenHeight);

        // Level up every 60 seconds and play level up sound
        if (currentTime - lastLevelUpTime > 60.0) {
            level++;
            lastLevelUpTime = currentTime;
            PlaySound(levelupSound);
        }

        // Fire bullet when space is pressed and play shooting sound
        if (IsKeyPressed(KEY_SPACE)) {
            FireBullet(bullets, player.pos, playerTexture.width*0.2f);
            PlaySound(shootSound);
        }


        // Update all game objects
        UpdateBullets(bullets);
        UpdateEnemies(enemies, 2.0f + 0.5*(level-1), screenWidth, screenHeight, player.pos);
        UpdateStars(stars, screenWidth, screenHeight);
        UpdateShield(&shield, &shieldActive, currentTime);


        // Check for collisions between bullets and enemies
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (!bullets[i].active) continue;
            for (int j = 0; j < MAX_ENEMIES; j++) {
                if (!enemies[j].active) continue;


                // Create rectangles for collision detection between bullet and enemy
                Rectangle bulletRec = { bullets[i].pos.x - 2, bullets[i].pos.y - 2, 4, 4 };
                Rectangle enemyRec  = { enemies[j].pos.x, enemies[j].pos.y, enemyTexture.width*0.125f, 
                                        enemyTexture.height*0.125f };


                /* If a collision is detected , deactivate the bullet, reset the enemy's position
                   and speed,play explosion sound */
                if (CheckCollisionRecs(bulletRec, enemyRec)) {
                    bullets[i].active = false;
                    enemies[j].pos = (Vector2){ rand() % (screenWidth - (int)(enemyTexture.width*0.125f)), -50 - rand()%150 };
                    enemies[j].speed = 2.0f + 0.5*(level-1);
                    PlaySound(explosionSound);

                    /* 60% chance to spawn a shield power-up */
                    if (rand()%100 < 60) {
                        shield.pos = enemies[j].pos;
                        shield.active = true;
                        shield.expireTime = currentTime + 5.0;
                    }

                    score += 10;
                    if (score > highScore) highScore = score;
                }
            }
        }


        // Check for collisions between player and enemies
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (!enemies[i].active) {
                continue;
            }

            // Create rectangles for collision detection between player and enemy
            Rectangle playerRec = { player.pos.x, player.pos.y, playerTexture.width*0.2f, 
                                    playerTexture.height*0.2f };
            Rectangle enemyRec  = { enemies[i].pos.x, enemies[i].pos.y, enemyTexture.width*0.125f, 
                                    enemyTexture.height*0.125f };

            /* If a collision is detected and shield is not active,reset the game */
            if (CheckCollisionRecs(playerRec, enemyRec)) {
                if (!shieldActive) {
                    InitPlayer(&player, screenWidth, screenHeight);
                    InitEnemies(enemies, 2.0f + 0.5*(level-1), screenWidth);
                    score = 0;
                    level = 1;
                    lastLevelUpTime = currentTime;
                }
                else {
                    shieldActive = false;          // Deactivate shield on collision
                }
                break;
            }
        }


        /* Check for collision between player and shield power-up,activate shield if collected */
        if (shield.active) {
            Rectangle playerRec = { player.pos.x, player.pos.y, playerTexture.width*0.2f, 
                                    playerTexture.height*0.2f };
            Rectangle shieldRec  = { shield.pos.x - 10, shield.pos.y - 10, 20, 20 };

            if (CheckCollisionRecs(playerRec, shieldRec)) {
                shieldActive = true;
                shield.active = false;
            }
        }


        // Begin drawing the game frame
        BeginDrawing();
        ClearBackground(BLACK);

        DrawStars(stars);
        DrawPlayer(&player, playerTexture, 0.2f, shieldActive);
        DrawBullets(bullets);
        DrawEnemies(enemies, enemyTexture, 0.125f);
        DrawShield(&shield);

        DrawText(TextFormat("Score: %d", score), 10, 10, 20, WHITE);
        DrawText(TextFormat("High Score: %d", highScore), 10, 40, 20, GOLD);
        DrawText(TextFormat("Level: %d", level), 10, 70, 20, WHITE);

        EndDrawing();
    }


    /* Unload textures and sounds,close audio device and window before exiting */
    UnloadTexture(playerTexture);
    UnloadTexture(enemyTexture);
    UnloadSound(shootSound);
    UnloadSound(explosionSound);
    UnloadSound(levelupSound);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

