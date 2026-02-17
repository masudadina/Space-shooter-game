#pragma once        // Ensures this header file avoiding redefinition errors

#include "raylib.h" 
#include <stdbool.h>


/* Player structure to hold position and speed */
typedef struct {
    Vector2 pos;
    float speed;
} Player;


/* Function prototypes for player management */
void InitPlayer(Player *player , int screenWidth , int screenHeight);
void UpdatePlayer(Player *player, int screenWidth, int screenHeight);
void DrawPlayer(Player *player,Texture2D texture,float scale,bool shieldActive);


