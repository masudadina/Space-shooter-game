#pragma once

/* Bullet.hpp
   C → C++ conversion:
     C  : typedef struct Bullet + free functions FireBullet / UpdateBullets / DrawBullets
          + raw array Bullet bullets[MAX_BULLETS]
     C++ : struct Bullet (data) + class BulletManager (behaviour)
           BulletManager owns a std::array — no raw array passed around.
  
   SOLID (unchanged):
     SRP : Only bullet state and behaviour live here.
     ISP : No Player or Enemy types pulled in.
     LSP : draw() is const — guaranteed no side-effects on bullet state */
 

#include "raylib.h"
#include <array>

static constexpr int MAX_BULLETS = 10;

/* Plain data — identical to the C struct */
struct Bullet
{
    Vector2 pos    = { 0.0f, 0.0f };
    bool    active = false;
};


/* BulletManager owns the bullet array and all bullet behaviour.
   Replaces the three C free functions */
 
class BulletManager
{
public:
    /* fire() replaces FireBullet() */
    void fire  (Vector2 playerPos, float playerWidth);

    /* update() replaces UpdateBullets() */
    void update();

    /* draw() replaces DrawBullets() — const: LSP guarantee */
    void draw  () const;

    /* Direct access needed by main.cpp for collision checks */
    std::array<Bullet, MAX_BULLETS>& bullets() { return bullets_; }

private:
    std::array<Bullet, MAX_BULLETS> bullets_{};
};
