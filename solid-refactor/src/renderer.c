#include "renderer.h"

/* renderer.c
   SRP : ONE reason to change — how textures are loaded and freed */
 

void Renderer_Init(Renderer* r, const char* playerPath,
                                const char* enemyPath) {
    r->playerTexture = LoadTexture(playerPath);
    r->enemyTexture  = LoadTexture(enemyPath);
    r->playerScale   = 0.20f;
    r->enemyScale    = 0.125f;
}

void Renderer_Unload(Renderer* r) {
    UnloadTexture(r->playerTexture);
    UnloadTexture(r->enemyTexture);
}
