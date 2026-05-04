#include "Renderer.hpp"

/*
 * Renderer.cpp
 * SRP : ONE reason to change — how textures are loaded and freed.
 */

Renderer::Renderer(const std::string& playerPath,
                   const std::string& enemyPath)
    : playerScale(0.20f), enemyScale(0.125f)
{
    playerTexture = LoadTexture(playerPath.c_str());
    enemyTexture  = LoadTexture(enemyPath.c_str());
}

/* RAII : destructor guarantees cleanup even if an exception occurs */
Renderer::~Renderer()
{
    UnloadTexture(playerTexture);
    UnloadTexture(enemyTexture);
}
