#pragma once
/*
 * Renderer.hpp
 *
 * C → C++ conversion:
 *   C  : typedef struct Renderer { ... };  + Renderer_Init / Renderer_Unload
 *   C++ : class Renderer — constructor loads, destructor unloads (RAII)
 *
 * SOLID (unchanged):
 *   SRP : Texture asset loading/unloading is the ONLY concern here.
 *   ISP : Callers that only need textures include only this header.
 *   DIP : main.cpp uses this class instead of calling LoadTexture directly.
 *
 * C++ bonus — RAII: the destructor automatically unloads textures,
 * so forgetting to call Renderer_Unload() is no longer possible.
 */

#include "raylib.h"
#include <string>

class Renderer
{
public:
    Texture2D playerTexture;
    Texture2D enemyTexture;
    float     playerScale;   /* 0.20f  */
    float     enemyScale;    /* 0.125f */

    /* Constructor replaces Renderer_Init() — loads both textures */
    Renderer(const std::string& playerPath, const std::string& enemyPath);

    /* Destructor replaces Renderer_Unload() — RAII cleanup */
    ~Renderer();
};
