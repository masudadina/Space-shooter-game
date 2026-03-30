#pragma once

  /*audio.h
  SRP : Sound asset management is the only concern here.
  ISP : Completely separate from renderer.h and collision.h — a module
        that needs only audio includes only this header.
  DIP : main.c calls Audio_PlayShoot() etc., never PlaySound() directly */
 

#include "raylib.h"

typedef struct {
    Sound shoot;
    Sound explosion;
    Sound levelup;
} AudioManager;

void Audio_Init        (AudioManager* a, const char* shootPath,
                                         const char* explosionPath,
                                         const char* levelupPath);

void Audio_Unload (AudioManager* a);

void Audio_PlayShoot (const AudioManager* a);

void Audio_PlayExplosion(const AudioManager* a);

void Audio_PlayLevelUp (const AudioManager* a);
