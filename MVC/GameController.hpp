#pragma once

#include "Player.hpp"
#include "Bullet.hpp"
#include "Enemy.hpp"
#include "ShieldPickup.hpp"
#include "Starfield.hpp"
#include "Renderer.hpp"
#include "AudioManager.hpp"
#include "Collision.hpp"
#include "GameState.hpp"

class GameController
{
public:
    GameController();
    void run();

private:
    static constexpr int SCREEN_W = 1600;
    static constexpr int SCREEN_H = 850;

    // Model
    Player player;
    BulletManager bullets;
    EnemyManager enemies;
    ShieldPickup shield;
    Starfield stars;
    GameState gs;

    // View
    Renderer rend;
    AudioManager audio;

    bool shieldActive;

    void processInput();
    void update();
    void handleCollisions();
    void render();
    float enemySpeed(int level);
};