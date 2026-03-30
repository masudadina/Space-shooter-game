#include "shield.h"

/* shield.c
   SRP : ONE reason to change — shield pickup spawn, expiry, and display */
 

void ShieldPickup_Init(ShieldPickup* s) {
    s->active     = false;
    s->expireTime = 0.0;
    s->pos        = (Vector2){ 0.0f, 0.0f };
}


/* OCP : Spawn duration is a parameter — change drop window without
         touching any caller */
void ShieldPickup_Spawn(ShieldPickup* s, Vector2 pos,
                        double currentTime, double durationSeconds) {
    s->pos        = pos;
    s->active     = true;
    s->expireTime = currentTime + durationSeconds;
}


/* LSP : contract — if expired, deactivate; otherwise leave alone */
void ShieldPickup_Update(ShieldPickup* s, double currentTime) {
    if (s->active && currentTime > s->expireTime)
        s->active = false;
}


/* LSP : const — drawing never has side-effects on the pickup */
void ShieldPickup_Draw(const ShieldPickup* s) {
    if (s->active)
        DrawCircle((int)s->pos.x, (int)s->pos.y, 10, GREEN);
}


/* DIP : main.c delegates the timer comparison to this abstraction */
void PlayerShield_Update(bool* shieldActive, const ShieldPickup* s,
                         double currentTime) {
    if (*shieldActive && currentTime > s->expireTime)
        *shieldActive = false;
}
