#pragma once       // Ensures this header file avoiding redefinition errors

#include "raylib.h"     // Provides core graphics/game functions and utilities
#include <stdbool.h>    // For using boolean data type

#define MAX_STARS 100


/* Star structure to hold position and speed */
typedef struct {
    Vector2 pos;
    float speed;
} Star;


/* Function prototypes for starfield management */
void InitStars(Star stars[], int screenWidth, int screenHeight);
void UpdateStars(Star stars[], int screenWidth, int screenHeight);
void DrawStars(Star stars[]);

