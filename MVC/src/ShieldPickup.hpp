#pragma once
/*
 * ShieldPickup.hpp
 *
 * C → C++ conversion:
 *   C  : typedef struct ShieldPickup + free functions
 *        ShieldPickup_Init / ShieldPickup_Spawn / ShieldPickup_Update /
 *        ShieldPickup_Draw / PlayerShield_Update
 *   C++ : class ShieldPickup — constructor replaces Init(),
 *         methods replace the free functions.
 *         PlayerShield_Update becomes a static utility method.
 *
 * SOLID (unchanged):
 *   SRP  : Shield pickup lifecycle is the ONLY concern of this class.
 *          The player's shieldActive bool lives in main.cpp — separated.
 *   ISP  : Callers that don't need the shield never include this header.
 *   OCP  : spawn() encapsulates drop duration as a parameter.
 *   LSP  : draw() is const — no mutation inside a draw call.
 *   DIP  : updatePlayerShield() abstracts the raw time comparison so
 *          main.cpp never does arithmetic against expireTime directly.
 */

#include "raylib.h"

class ShieldPickup
{
public:
    Vector2 pos;
    bool    active;
    double  expireTime;

    /* Constructor replaces ShieldPickup_Init() */
    ShieldPickup();

    /* OCP : drop duration is a parameter */
    void spawn(Vector2 killPos, double currentTime, double durationSeconds);

    /* Deactivates pickup when its window closes */
    void update(double currentTime);

    /* LSP : const — draw never mutates pickup state */
    void draw() const;

    /*
     * DIP : main.cpp delegates the timer comparison here instead of
     *       doing  if (currentTime > expireTime)  inline.
     */
    static void updatePlayerShield(bool& shieldActive,
                                   const ShieldPickup& s,
                                   double currentTime);
};
