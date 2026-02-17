#include "shield.h"     // Includes the header file for shield management
#include <stdlib.h>     // For random number generation


/* Initializes the shield by setting its active state to false, 
   expire time to 0 and position to (0,0) */
void InitShield(Shield *shield) {
    shield->active = false;
    shield->expireTime = 0;
    shield->pos = (Vector2){0,0};
}


/* Updates the shield by checking if the current time has exceeded 
   the expire time.If it has ,deactivates the player's shield */
void UpdateShield(Shield *shield, bool *playerShield, double currentTime) {
    if (*playerShield && currentTime > shield->expireTime) *playerShield = false;
}


/* Draws the shield as a green circle if it is active */
void DrawShield(Shield *shield) {
    if (shield->active)
        DrawCircle(shield->pos.x, shield->pos.y, 10, GREEN);
}
