#pragma once

/* AudioManager.hpp
  
   C → C++ conversion:
    C  : typedef struct AudioManager { ... };  + Audio_Init / Audio_Play* / Audio_Unload
     C++ : class AudioManager — constructor loads, destructor unloads (RAII)
  
   SOLID (unchanged):
     SRP : Sound asset management is the ONLY concern here.
     ISP : Fully separate from Renderer.hpp and Collision.hpp.
     DIP : main.cpp calls playShoot() etc., never PlaySound() directly.
  
   C++ bonus — RAII: destructor guarantees sound cleanup automatically.
   File renamed AudioManager to avoid clash with the C struct name */
 

#include "raylib.h"
#include <string>

class AudioManager
{
public:
    /* Constructor replaces Audio_Init() */
    AudioManager(const std::string& shootPath,
                 const std::string& explosionPath,
                 const std::string& levelupPath);

    /* Destructor replaces Audio_Unload() */
    ~AudioManager();

    /* DIP : main.cpp calls these, never PlaySound() directly */
    void playShoot    () const;
    void playExplosion() const;
    void playLevelUp  () const;

private:
    Sound shoot_;
    Sound explosion_;
    Sound levelup_;
};
