#pragma once

/* shield.h
   SRP  : Shield pickup lifecycle (world object) is its own module.
          The player's shieldActive boolean lives in main.c / GameState —
          the two concerns are intentionally kept apart.
   ISP  : Callers that don't need the shield don't include this header.
   OCP  : ShieldPickup_Spawn() encapsulates spawn duration.  Adding a
          second power-up type means a new struct alongside this one —
          not editing ShieldPickup.
   LSP  : ShieldPickup_Draw takes a const pointer — no mutation in draw.
   DIP  : PlayerShield_Update() abstracts the raw time comparison so
          main.c never does arithmetic against expireTime directly */
 

#include "raylib.h"
#include <stdbool.h>

/* World-space collectible — the green circle dropped on enemy kill */
typedef struct {
    Vector2 pos;
    bool    active;
    double  expireTime;   /* wall-clock time when pickup disappears */
} ShieldPickup;


/* Initialise to inactive / zeroed state */
void ShieldPickup_Init  (ShieldPickup* s);


/* Spawn at kill position; pickup vanishes after durationSeconds */
void ShieldPickup_Spawn (ShieldPickup* s, Vector2 pos,
                         double currentTime, double durationSeconds);


/* Deactivate the pickup if its window has closed */
void ShieldPickup_Update(ShieldPickup* s, double currentTime);


/* LSP : const — draw never mutates pickup state */
void ShieldPickup_Draw  (const ShieldPickup* s);


/* DIP : high-level game loop delegates shield-timer logic here.
         main.c never compares currentTime > expireTime directly */
 
void PlayerShield_Update(bool* shieldActive, const ShieldPickup* s,
                         double currentTime);
