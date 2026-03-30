#pragma once

/* collision.h
   ISP : Collision detection is its OWN header — callers that don't need
         it never include it.
   DIP : main.c (high-level) calls CheckCollision() through this
         abstraction instead of calling raylib's CheckCollisionRecs()
         directly, so the underlying library is hidden behind an interface.
   OCP : New collision shapes (e.g. circles) can be added here without
         changing any caller */
 

#include "raylib.h"
#include <stdbool.h>

// Core overlap test — wraps raylib behind an abstraction (DIP) 
bool CheckCollision(Rectangle a, Rectangle b);

// Named rect builders keep magic numbers out of main.c (SRP) 
Rectangle MakeBulletRect(float x, float y);
Rectangle MakePlayerRect(float x, float y, float texW, float texH, float scale);
Rectangle MakeEnemyRect (float x, float y, float texW, float texH, float scale);
Rectangle MakeShieldPickupRect(float x, float y);
