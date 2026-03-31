#include "ShieldPickup.hpp"

/*
 * ShieldPickup.cpp
 * SRP : ONE reason to change — shield pickup spawn, expiry, and display.
 */

ShieldPickup::ShieldPickup()
    : pos{ 0.0f, 0.0f }, active(false), expireTime(0.0)
{}

/* OCP : spawn duration is injected — change without touching main.cpp */
void ShieldPickup::spawn(Vector2 killPos, double currentTime,
                         double durationSeconds)
{
    pos        = killPos;
    active     = true;
    expireTime = currentTime + durationSeconds;
}

/* LSP : contract — if expired deactivate; otherwise leave alone */
void ShieldPickup::update(double currentTime)
{
    if (active && currentTime > expireTime)
        active = false;
}

/* LSP : const — drawing has no side-effects on pickup state */
void ShieldPickup::draw() const
{
    if (active)
        DrawCircle(static_cast<int>(pos.x),
                   static_cast<int>(pos.y), 10, GREEN);
}

/* DIP : main.cpp delegates timer logic here */
void ShieldPickup::updatePlayerShield(bool& shieldActive,
                                      const ShieldPickup& s,
                                      double currentTime)
{
    if (shieldActive && currentTime > s.expireTime)
        shieldActive = false;
}
