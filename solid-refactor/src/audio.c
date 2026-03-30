#include "audio.h"

/* audio.c
   SRP : ONE reason to change — how sounds are loaded, freed, and played */
 

void Audio_Init(AudioManager* a, const char* shootPath,
                                 const char* explosionPath,
                                 const char* levelupPath) {
    a->shoot     = LoadSound(shootPath);
    a->explosion = LoadSound(explosionPath);
    a->levelup   = LoadSound(levelupPath);
}

void Audio_Unload(AudioManager* a) {
    UnloadSound(a->shoot);
    UnloadSound(a->explosion);
    UnloadSound(a->levelup);
}

void Audio_PlayShoot   (const AudioManager* a) { PlaySound(a->shoot);     }
void Audio_PlayExplosion(const AudioManager* a) { PlaySound(a->explosion); }
void Audio_PlayLevelUp (const AudioManager* a) { PlaySound(a->levelup);   }
