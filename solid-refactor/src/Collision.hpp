#pragma once

/* Collision.hpp
  
   C → C++ conversion:
     C  : free functions CheckCollision(), Make*Rect()
     C++ : namespace Collision — groups related free functions cleanly
           without forcing an object where none is needed.
  
   SOLID (unchanged):
     ISP : Collision detection is its own header — callers that don't
           need it never include it.
     DIP : main.cpp calls Collision::check() instead of
           CheckCollisionRecs() directly — raylib stays decoupled.
     OCP : New collision shapes can be added to the namespace without
           changing any existing caller.
     SRP : ONE reason to change — overlap detection and rect sizing */
 

#include "raylib.h"

namespace Collision
{
    /* DIP — wraps raylib behind an abstraction */
    bool      check             (Rectangle a, Rectangle b);

    /* Named builders keep magic numbers out of main.cpp (SRP) */
    Rectangle makeBulletRect     (float x, float y);
    Rectangle makePlayerRect     (float x, float y, float texW, float texH, float scale);
    Rectangle makeEnemyRect      (float x, float y, float texW, float texH, float scale);
    Rectangle makeShieldPickupRect(float x, float y);
}
