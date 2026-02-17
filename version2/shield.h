#pragma once     // Ensures this header file avoiding redefinition errors

#include "raylib.h"
#include <stdbool.h>


/* Shield structure to hold position,active state and expire time */
typedef struct {
    Vector2 pos;
    bool active;
    double expireTime;
} Shield;


/* Function prototypes for shield management */
void InitShield(Shield *shield);
void UpdateShield(Shield *shield, bool *playerShield, double currentTime);
void DrawShield(Shield *shield);

