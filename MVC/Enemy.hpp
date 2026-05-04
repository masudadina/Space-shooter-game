#pragma once

/* Enemy.hpp
 
   C → C++ conversion:
     C  : typedef struct Enemy + free functions InitEnemies /
          UpdateEnemies / RespawnEnemy / DrawEnemies
          + raw array Enemy enemies[MAX_ENEMIES]
     C++ : struct Enemy (data) + class EnemyManager (behaviour)
           EnemyManager owns a std::array — no raw array passed around.
  
   SOLID (unchanged):
     SRP : Only enemy state and behaviour live here.
     OCP : respawn() isolates the respawn algorithm — new spawn
           patterns extend this class, never main.cpp.
     LSP : draw() is const — drawing never mutates enemy state.
     DIP : Speed injected as a parameter; no global dependency */
 

#include "raylib.h"
#include <array>

static constexpr int MAX_ENEMIES = 10;

/* Plain data — identical to the C struct */
struct Enemy
{
    Vector2 pos    = { 0.0f, 0.0f };
    bool    active = false;
    float   speed  = 0.0f;
};


/* EnemyManager owns the enemy array and all enemy behaviour.
   Replaces the four C free functions */
 
class EnemyManager
{
public:
    /* init() replaces InitEnemies() */
    void init  (float speed, int screenWidth);

    /* update() replaces UpdateEnemies() */
    void update(float speed, int screenWidth,
                int screenHeight, Vector2 playerPos);

    /* OCP : respawn() extracted — change spawn behaviour here only */
    void respawn(Enemy& enemy, float speed, int screenWidth);

    /* draw() replaces DrawEnemies() — const: LSP guarantee */
    void draw(Texture2D texture, float scale) const;

    /* Direct access needed by main.cpp for collision checks */
    std::array<Enemy, MAX_ENEMIES>& enemies() { return enemies_; }

private:
    std::array<Enemy, MAX_ENEMIES> enemies_{};
};
