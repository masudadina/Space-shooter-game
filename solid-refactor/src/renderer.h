#pragma once

/* renderer.h
   SRP : Asset loading / unloading is the only concern of this module.
   ISP : Callers that only need textures include only this header —
         they do not also get audio or collision pulled in.
   DIP : main.c calls Renderer_Init / Renderer_Unload instead of
         calling LoadTexture / UnloadTexture directly */
 

#include "raylib.h"

typedef struct {
    Texture2D playerTexture;
    Texture2D enemyTexture;
    float     playerScale;   /* 0.20f  */
    float     enemyScale;    /* 0.125f */
} Renderer;

void Renderer_Init  (Renderer* r, const char* playerPath,
                                  const char* enemyPath);
void Renderer_Unload(Renderer* r);
