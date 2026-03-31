#include "AudioManager.hpp"

/* AudioManager.cpp
 * SRP : ONE reason to change — how sounds are loaded, freed, and played */
 

AudioManager::AudioManager(const std::string& shootPath,
                           const std::string& explosionPath,
                           const std::string& levelupPath)
{
    shoot_     = LoadSound(shootPath.c_str());
    explosion_ = LoadSound(explosionPath.c_str());
    levelup_   = LoadSound(levelupPath.c_str());
}

AudioManager::~AudioManager()
{
    UnloadSound(shoot_);
    UnloadSound(explosion_);
    UnloadSound(levelup_);
}

void AudioManager::playShoot    () const { PlaySound(shoot_);     }
void AudioManager::playExplosion() const { PlaySound(explosion_); }
void AudioManager::playLevelUp  () const { PlaySound(levelup_);   }
