# 🚀 Space Shooter — C++ SOLID Edition

> A **raylib** arcade game converted from C to **modern C++17**, with every
> [SOLID](https://en.wikipedia.org/wiki/SOLID) design principle applied and
> explained. Same gameplay. Cleaner architecture. 

<div align="center">

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue?style=flat-square&logo=cplusplus)
![raylib](https://img.shields.io/badge/raylib-5.x-orange?style=flat-square)
![SOLID](https://img.shields.io/badge/SOLID-%E2%9C%93-green?style=flat-square)
![License](https://img.shields.io/badge/license-MIT-lightgrey?style=flat-square)

</div>

---

## 📋 Table of Contents

1. [Game Overview](#-game-overview)
2. [Architecture at a Glance](#-architecture-at-a-glance)
3. [Complete File Map](#-complete-file-map)
4. [C to C++ Conversion Table](#-c-to-c-conversion-table)
5. [Module-by-Module Comparison](#-module-by-module-comparison)
6. [SOLID Principles Deep Dive](#-solid-principles-deep-dive)
7. [Game Loop Flowchart](#-game-loop-flowchart)
8. [RAII Lifecycle Diagram](#-raii-lifecycle-diagram)
9. [Collision System Diagram](#-collision-system-diagram)
10. [How to Build and Run](#-how-to-build-and-run)
11. [Git Workflow](#-git-workflow)
12. [AI Prompts Used](#-ai-prompts-used)

---

## 🎮 Game Overview

Space Shooter is a top-down arcade game built with **raylib**.

| Control     | Action              |
|-------------|---------------------|
| `← → ↑ ↓` | Move the spaceship  |
| `Space`     | Fire a bullet       |
| `Enter`     | Start the game      |

**Gameplay features:**
- Enemies fall from the top and drift toward the player
- Shoot enemies to earn **+10 points** per kill
- **Shield pickup** (green circle) drops on **60% of kills** and absorbs one hit
- Every **60 seconds** the level increases and enemies speed up
- High score is preserved across deaths

---

## 🗺 Architecture at a Glance

```
┌──────────────────────────────────────────────────────────────────┐
│                         main.cpp                                 │
│             (Game loop orchestration only — SRP)                 │
│                                                                  │
│  ┌───────────┐ ┌───────────┐ ┌───────────┐ ┌─────────────────┐  │
│  │ GameState │ │ Renderer  │ │AudioMgr   │ │namespace        │  │
│  │ score     │ │ textures  │ │ sounds    │ │Collision        │  │
│  │ level     │ │ RAII      │ │ RAII      │ │check()          │  │
│  └───────────┘ └───────────┘ └───────────┘ │make*Rect()      │  │
│                                             └─────────────────┘  │
│  ┌───────────┐ ┌───────────┐ ┌───────────┐ ┌─────────────────┐  │
│  │  Player   │ │  Bullet   │ │  Enemy    │ │  ShieldPickup   │  │
│  │  class    │ │  Manager  │ │  Manager  │ │  class          │  │
│  │  draw()   │ │std::array │ │std::array │ │  static method  │  │
│  │  const    │ │draw()const│ │draw()const│ │  draw() const   │  │
│  └───────────┘ └───────────┘ └───────────┘ └─────────────────┘  │
│                                                                  │
│                       ┌───────────┐                              │
│                       │ Starfield │                              │
│                       │ std::array│                              │
│                       │draw()const│                              │
│                       └───────────┘                              │
└──────────────────────────────┬───────────────────────────────────┘
                               │ interfaces only — DIP
                    ┌──────────▼──────────┐
                    │     raylib 5.x      │
                    │  graphics + audio   │
                    └─────────────────────┘
```

**Key rule:** `main.cpp` calls only module interfaces. It never calls
raylib directly for collision, audio, or asset management. That is
**Dependency Inversion (DIP)** in action.

---

## 📁 Complete File Map

```
SpaceShooterCPP/
│
├── assets/                      ← Runtime assets (unchanged from C)
│   ├── player.png
│   ├── enemy.png
│   ├── shoot.wav
│   ├── explosion.wav
│   └── levelup.wav
│
├── .vscode/
│   └── tasks.json               ← g++ -std=c++17 build task
│
├── .gitignore
├── README.md
│
├── GameState.hpp  / GameState.cpp      ← Score + level     [SRP, OCP]
├── Renderer.hpp   / Renderer.cpp       ← Textures          [SRP, ISP, DIP, RAII]
├── AudioManager.hpp / AudioManager.cpp ← Sounds            [SRP, ISP, DIP, RAII]
├── Collision.hpp  / Collision.cpp      ← Overlap tests     [SRP, ISP, DIP, OCP]
├── Player.hpp     / Player.cpp         ← Player            [SRP, LSP]
├── Bullet.hpp     / Bullet.cpp         ← Bullets           [SRP, ISP, LSP]
├── Enemy.hpp      / Enemy.cpp          ← Enemies           [SRP, OCP, LSP, DIP]
├── ShieldPickup.hpp / ShieldPickup.cpp ← Shield pickup     [all 5 SOLID]
├── Starfield.hpp  / Starfield.cpp      ← Background        [SRP, ISP, LSP]
└── main.cpp                            ← Game loop         [SRP, OCP, DIP]
```

**10 modules · 20 source files · 5 assets · 1 build config**

---

## 🔄 C to C++ Conversion Table

Every change made — nothing else was altered.

| Concept | C version | C++ version |
|---|---|---|
| Data + behaviour | `typedef struct` + free functions | `class` with member methods |
| Initialisation | `GameState_Init(&gs, t)` | Constructor `GameState gs(t)` |
| Cleanup | `Renderer_Unload(&r)` — must remember | `~Renderer()` destructor — automatic |
| Asset safety | Easy to forget `UnloadTexture` | RAII: structurally impossible to forget |
| Arrays | `Bullet bullets[MAX_BULLETS] = {0}` | `std::array<Bullet, MAX_BULLETS> bullets_{}` |
| Loops | `for (int i = 0; i < MAX; i++)` | `for (auto& b : bullets_)` |
| Const enforcement | `const Bullet bullets[]` parameter | `void draw() const` method |
| Grouping stateless helpers | Free functions `CheckCollision()` | `namespace Collision { check() }` |
| String paths | `const char*` | `const std::string&` |
| Casts | `(float)x` | `static_cast<float>(x)` |
| Private data | Convention only | `private:` — compiler enforced |
| `bool` type | `#include <stdbool.h>` required | Built into C++ |
| stdlib headers | `#include <stdlib.h>` | `#include <cstdlib>` |
| File extensions | `.h` / `.c` | `.hpp` / `.cpp` |
| Naming style | `GameState_AddScore()` | `gs.addScore()` |

---

## 🔁 Module-by-Module Comparison

### 1. GameState

```
┌──────────────────────────────┬──────────────────────────────────┐
│  C  (game_state.h)           │  C++  (GameState.hpp)            │
├──────────────────────────────┼──────────────────────────────────┤
│  typedef struct {            │  class GameState {               │
│    int score, highScore;     │  public:                         │
│    int level;                │    int score, highScore, level;  │
│    double lastLevelUpTime;   │    double lastLevelUpTime;       │
│  } GameState;                │                                  │
│                              │    GameState(double startTime);  │
│  void GameState_Init(...)    │    void addScore(int points);    │
│  void GameState_AddScore(...)│    bool tryLevelUp(...);         │
│  bool GameState_TryLevelUp(.)│    void reset(double t);         │
│  void GameState_Reset(...)   │  };                              │
└──────────────────────────────┴──────────────────────────────────┘
```

**C — calling code:**
```c
GameState gs;
GameState_Init(&gs, GetTime());

if (currentTime - gs.lastLevelUpTime > 60.0) {   // raw arithmetic in main
    gs.level++;
    gs.lastLevelUpTime = currentTime;
    PlaySound(levelupSound);                      // direct raylib call
}
score += 10;
if (score > highScore) highScore = score;         // raw logic in main
```

**C++ — calling code:**
```cpp
GameState gs(GetTime());                          // constructed, not init'd

if (gs.tryLevelUp(currentTime, 60.0))            // OCP: rule lives in class
    audio.playLevelUp();                          // DIP: no PlaySound here

gs.addScore(10);                                  // SRP: score logic in class
```

---

### 2. Renderer

```
┌──────────────────────────────┬──────────────────────────────────┐
│  C  (renderer.h)             │  C++  (Renderer.hpp)             │
├──────────────────────────────┼──────────────────────────────────┤
│  typedef struct {            │  class Renderer {                │
│    Texture2D playerTexture;  │  public:                         │
│    Texture2D enemyTexture;   │    Texture2D playerTexture;      │
│    float playerScale;        │    Texture2D enemyTexture;       │
│    float enemyScale;         │    float playerScale, enemyScale;│
│  } Renderer;                 │                                  │
│                              │    Renderer(string& p, string& e)│
│  void Renderer_Init(...)     │    ~Renderer();  // RAII         │
│  void Renderer_Unload(...)   │  };                              │
└──────────────────────────────┴──────────────────────────────────┘
```

**C — manual lifecycle (forgettable):**
```c
Renderer rend;
Renderer_Init(&rend, "assets/player.png", "assets/enemy.png");
// ... 150 lines later, must remember:
Renderer_Unload(&rend);   // easy to forget = GPU memory leak
```

**C++ — RAII lifecycle (automatic):**
```cpp
Renderer rend("assets/player.png", "assets/enemy.png"); // loads here
// ... game loop ...
// ~Renderer() runs automatically — UnloadTexture called for both
```

---

### 3. AudioManager

**C — public Sound fields, anyone can write to them:**
```c
typedef struct {
    Sound shoot;       // public — anyone could write audio.shoot = ...
    Sound explosion;
    Sound levelup;
} AudioManager;

Audio_PlayShoot(&audio);    // must pass pointer every time
```

**C++ — private Sound fields, const methods only:**
```cpp
class AudioManager {
public:
    void playShoot    () const;    // const = cannot accidentally change state
    void playExplosion() const;
    void playLevelUp  () const;
private:
    Sound shoot_;      // private — callers cannot touch raw Sound objects
    Sound explosion_;
    Sound levelup_;
};

audio.playShoot();             // clean call, no pointer needed
```

---

### 4. Collision

**C — free functions, no grouping:**
```c
bool      CheckCollision       (Rectangle a, Rectangle b);
Rectangle MakeBulletRect       (float x, float y);
Rectangle MakePlayerRect       (float x, float y, float w, float h, float s);
Rectangle MakeEnemyRect        (float x, float y, float w, float h, float s);
Rectangle MakeShieldPickupRect (float x, float y);

if (CheckCollision(bRect, eRect)) { ... }        // ambiguous — what namespace?
```

**C++ — `namespace Collision` makes grouping explicit:**
```cpp
namespace Collision {
    bool      check              (Rectangle a, Rectangle b);
    Rectangle makeBulletRect     (float x, float y);
    Rectangle makePlayerRect     (float x, float y, float w, float h, float s);
    Rectangle makeEnemyRect      (float x, float y, float w, float h, float s);
    Rectangle makeShieldPickupRect(float x, float y);
}

if (Collision::check(bRect, eRect)) { ... }      // instantly clear what this is
```

> **Why `namespace` and not `class`?**
> Collision has no state. Making it a class would require either
> instantiating a pointless object or making every method `static`.
> A `namespace` is the correct C++ idiom for stateless helpers.

---

### 5. Player

**C — non-const DrawPlayer, separate init call:**
```c
void DrawPlayer(Player *player, Texture2D t, float s, bool shield);
// compiler cannot stop DrawPlayer from writing to *player

Player player;
InitPlayer(&player, screenWidth, screenHeight);   // two lines to set up
DrawPlayer(&player, playerTexture, 0.2f, shieldActive);
```

**C++ — constructor + const draw:**
```cpp
class Player {
public:
    Player(int w, int h);                             // replaces InitPlayer()
    void update(int w, int h);
    void draw(Texture2D t, float s, bool shield) const; // const = LSP
};

Player player(SCREEN_W, SCREEN_H);                    // one line to set up
player.draw(rend.playerTexture, rend.playerScale, shieldActive);
```

---

### 6. BulletManager

**C — raw array passed to every function:**
```c
Bullet bullets[MAX_BULLETS] = {0};    // raw array, manual zero-init

FireBullet  (bullets, player.pos, playerTexture.width * 0.2f);
UpdateBullets(bullets);
DrawBullets  (bullets);               // non-const in original C version

for (int i = 0; i < MAX_BULLETS; i++) {
    if (!bullets[i].active) continue;
    // collision check with index arithmetic
}
```

**C++ — BulletManager owns the array:**
```cpp
BulletManager bullets;                // std::array default-initialised

bullets.fire  (player.pos, width);
bullets.update();
bullets.draw  ();                     // const method — LSP

for (auto& b : bullets.bullets()) {  // range-for, no index, no overflow risk
    if (!b.active) continue;
    // collision check
}
```

---

### 7. EnemyManager

**C — respawn duplicated twice in main.c:**
```c
// This block appeared in TWO places in main.c
enemies[j].pos = (Vector2){
    rand() % (screenWidth - (int)(enemyTexture.width * 0.125f)),
    -50 - rand() % 150
};
enemies[j].speed = 2.0f + 0.5 * (level - 1);
```

**C++ — `respawn()` is a member, called by reference:**
```cpp
// Enemy.cpp — one canonical place
void EnemyManager::respawn(Enemy& enemy, float speed, int screenWidth)
{
    enemy.pos   = { static_cast<float>(rand() % (screenWidth - 64)),
                    -50.0f - static_cast<float>(rand() % 150) };
    enemy.speed  = speed;
    enemy.active = true;
}

// main.cpp — one clean call, no pointer arithmetic
enemies.respawn(e, enemySpeed(gs.level), SCREEN_W);  // e is a reference
```

---

### 8. ShieldPickup

The most transformed module. Five C free functions become one class:

```
C free functions                    C++ class
─────────────────────────────────   ──────────────────────────────────────
ShieldPickup_Init(&s)           →   ShieldPickup() constructor
ShieldPickup_Spawn(&s, pos, t)  →   s.spawn(pos, time, duration)
ShieldPickup_Update(&s, t)      →   s.update(currentTime)
ShieldPickup_Draw(&s)           →   s.draw()  [const]
PlayerShield_Update(&b, &s, t)  →   ShieldPickup::updatePlayerShield(b,s,t)
                                    ↑ static — no instance needed
```

**C — raw field access, pointer everywhere:**
```c
ShieldPickup shield;
ShieldPickup_Init(&shield);

// In collision handler — raw field writes scattered in main.c
if (rand() % 100 < 60) {
    shield.pos        = killPos;
    shield.active     = true;
    shield.expireTime = currentTime + 5.0;   // magic number in main
}
PlayerShield_Update(&shieldActive, &shield, currentTime);
```

**C++ — encapsulated, no raw field access needed:**
```cpp
ShieldPickup shield;                          // default constructed

// In collision handler — one call, duration is a parameter (OCP)
if (rand() % 100 < 60)
    shield.spawn(killPos, currentTime, 5.0);

// Static method — no instance pointer, still part of the class (DIP)
ShieldPickup::updatePlayerShield(shieldActive, shield, currentTime);
```

---

### 9. Starfield

The C version had a subtle bug: `DrawStars(Star stars[])` was not `const`.
C++ fixes this, and the constructor replaces the separate `InitStars()` call.

**C — draw was not const, separate init:**
```c
Star stars[MAX_STARS];
InitStars(stars, screenWidth, screenHeight);    // separate call required
DrawStars(stars);                               // NOT const — silent risk
```

**C++ — constructor + const draw:**
```cpp
Starfield stars(SCREEN_W, SCREEN_H);    // initialised in constructor
stars.draw();                           // const — compiler prevents all mutation
```

---

## 🧱 SOLID Principles Deep Dive

### S — Single Responsibility Principle

> *"A module should have one, and only one, reason to change."*

```
┌──────────────────────┬────────────────────────────────────────────┐
│ File                 │ Its single reason to change                │
├──────────────────────┼────────────────────────────────────────────┤
│ main.cpp             │ Game loop structure changes                │
│ GameState.cpp        │ Scoring or level-progression rules         │
│ Renderer.cpp         │ Texture format or loading library          │
│ AudioManager.cpp     │ Sound format or audio library              │
│ Collision.cpp        │ Overlap algorithm or rectangle sizing      │
│ Player.cpp           │ Player movement or visual appearance       │
│ Bullet.cpp           │ Bullet speed, appearance, or firing logic  │
│ Enemy.cpp            │ Enemy movement, spawn rules, or appearance │
│ ShieldPickup.cpp     │ Pickup spawn, expiry, or display           │
│ Starfield.cpp        │ Background star speed or visual style      │
└──────────────────────┴────────────────────────────────────────────┘
```

Before the refactor, `main.c` had **9 reasons to change** — scoring,
leveling, collision maths, rectangle building, asset loading, sound calls,
shield timing, enemy respawning, and drawing. One change touched everything.

---

### O — Open/Closed Principle

> *"Open for extension, closed for modification."*

```
Extension point          Location             How to extend
─────────────────────    ─────────────────    ──────────────────────────
Difficulty curve         enemySpeed() in      Change return formula —
                         main.cpp             zero other files change

Level-up rule            GameState::          Switch from time-based to
                         tryLevelUp()         score-based — main.cpp untouched

Shield drop duration     ShieldPickup::       Change the durationSeconds
                         spawn() parameter    value at call site

Enemy respawn pattern    EnemyManager::       Add new logic — init()
                         respawn()            and update() untouched
```

Example — changing the difficulty curve:
```cpp
// Current
static float enemySpeed(int level) {
    return 2.0f + 0.5f * static_cast<float>(level - 1);
}

// Want exponential scaling? Change only this function:
static float enemySpeed(int level) {
    return 1.5f * std::pow(1.2f, static_cast<float>(level - 1));
}
// EnemyManager, GameState, main loop — all untouched.
```

---

### L — Liskov Substitution Principle

> *"Objects of a subtype must be substitutable for their base type
> without altering correctness."*

In C++, LSP is enforced through **`const` member methods**.

```
C:   const Bullet bullets[]        ← const on parameter (weak)
C++: void BulletManager::draw() const  ← const on method (strong)
```

The `const` keyword after a method name tells the compiler: **this method
cannot modify any member variable**. This is stronger than the C version,
where `const` only protected the parameter, not the entire object.

Every draw method in the project uses this guarantee:

```cpp
void BulletManager::draw ()                              const;
void EnemyManager::draw  (Texture2D t, float s)         const;
void Player::draw        (Texture2D t, float s, bool b)  const;
void ShieldPickup::draw  ()                              const;
void Starfield::draw     ()                              const;
```

The contract is clear: calling `draw()` on any object **cannot** change
the game state. Any caller can depend on this — enforced at compile time.

---

### I — Interface Segregation Principle

> *"Clients should not be forced to depend on interfaces they do not use."*

```
AudioManager.hpp  declares:  AudioManager ctor/dtor + 3 play methods
                  does NOT:  mention Renderer, Enemy, or Collision

Collision.hpp     declares:  check() + 4 make*Rect() helpers
                  does NOT:  mention Player, Bullet, or Audio

ShieldPickup.hpp  declares:  ShieldPickup class + 4 methods + 1 static
                  does NOT:  mention GameState or Renderer

Starfield.hpp     declares:  Star struct + Starfield class + 3 methods
                  does NOT:  mention any other game object
```

A translation unit that only needs collision detection includes only
`Collision.hpp`. It does not get `AudioManager`, `Renderer`, or `Enemy`
dragged in with it. If you add a new module, existing headers are never
modified — only new ones are created.

---

### D — Dependency Inversion Principle

> *"High-level modules should not depend on low-level modules.
> Both should depend on abstractions."*

`main.cpp` is the high-level policy. It depends on **class interfaces**,
never on raylib:

```
main.cpp calls               Instead of (raw raylib)
──────────────────────────   ────────────────────────────────
Renderer(path, path)     →   LoadTexture() x2
~Renderer() [auto]       →   UnloadTexture() x2
audio.playShoot()        →   PlaySound(shootSound)
audio.playExplosion()    →   PlaySound(explosionSound)
audio.playLevelUp()      →   PlaySound(levelupSound)
Collision::check(a, b)   →   CheckCollisionRecs(a, b)
Collision::make*Rect(.)  →   Inline Rectangle{ ... } literals
gs.addScore(10)          →   score += 10; if(score > high)...
gs.tryLevelUp(t, 60.0)   →   if(currentTime - lastTime > 60.0)
ShieldPickup::update...  →   if(currentTime > expireTime)
enemies.respawn(e, ...)  →   3-line inline reset block x2
shield.spawn(...)        →   shield.pos=...; active=true; ...
```

Swapping raylib for a different library only requires changing the module
`.cpp` files. `main.cpp` never changes.

---

## 🔄 Game Loop Flowchart

```
                    ┌─────────────────────┐
                    │    Program Start     │
                    │  InitWindow()        │
                    │  InitAudioDevice()   │
                    │  srand()             │
                    └──────────┬──────────┘
                               │
              ┌────────────────▼────────────────┐
              │  RAII Constructors run:          │
              │  Renderer  ← LoadTexture x2      │
              │  AudioMgr  ← LoadSound x3        │
              │  Player, BulletMgr, EnemyMgr,    │
              │  ShieldPickup, Starfield,         │
              │  GameState                        │
              └────────────────┬────────────────┘
                               │
                    ┌──────────▼──────────┐
                    │   Start Screen      │
                    │  Draw title text    │
                    │  Wait for ENTER     │
                    └──────────┬──────────┘
                               │ ENTER pressed
                    ┌──────────▼──────────┐
                    │  Main Game Loop     │◄──────────────────┐
                    └──────────┬──────────┘                   │
                               │                             │
               ┌───────────────▼────────────────┐            │
               │  1. player.update()             │            │
               │     arrow key input             │            │
               └───────────────┬────────────────┘            │
               ┌───────────────▼────────────────┐            │
               │  2. gs.tryLevelUp()             │            │
               │     60 sec elapsed?             │            │
               │     yes → audio.playLevelUp()   │            │
               └───────────────┬────────────────┘            │
               ┌───────────────▼────────────────┐            │
               │  3. SPACE pressed?              │            │
               │     bullets.fire()              │            │
               │     audio.playShoot()           │            │
               └───────────────┬────────────────┘            │
               ┌───────────────▼────────────────┐            │
               │  4. bullets.update()            │            │
               │     enemies.update()            │            │
               │     stars.update()              │            │
               │     shield.update()             │            │
               │     ShieldPickup::update...     │            │
               └───────────────┬────────────────┘            │
               ┌───────────────▼────────────────┐            │
               │  5. Bullet vs Enemy collision   │            │
               │     hit? ──► respawn enemy      │            │
               │             playExplosion()     │            │
               │             60%: shield.spawn() │            │
               │             gs.addScore(10)     │            │
               └───────────────┬────────────────┘            │
               ┌───────────────▼────────────────┐            │
               │  6. Player vs Enemy collision   │            │
               │     hit, no shield?             │            │
               │       ──► reset player          │            │
               │           reset enemies         │            │
               │           gs.reset()            │            │
               │     hit, shield active?         │            │
               │       ──► shieldActive = false  │            │
               └───────────────┬────────────────┘            │
               ┌───────────────▼────────────────┐            │
               │  7. Player vs ShieldPickup      │            │
               │     collect? shieldActive=true  │            │
               └───────────────┬────────────────┘            │
               ┌───────────────▼────────────────┐            │
               │  8. BeginDrawing()              │            │
               │     stars.draw()       const    │            │
               │     player.draw()      const    │            │
               │     bullets.draw()     const    │            │
               │     enemies.draw()     const    │            │
               │     shield.draw()      const    │            │
               │     DrawText(HUD)               │            │
               │     EndDrawing()                │            │
               └───────────────┬────────────────┘            │
                               │                             │
                     WindowShouldClose? ────────── No ───────┘
                               │ Yes
                    ┌──────────▼──────────┐
                    │   Shutdown          │
                    │  CloseAudioDevice() │
                    │  CloseWindow()      │
                    │  ~AudioManager()    │
                    │  ~Renderer()        │
                    └─────────────────────┘
```

---

## ♻ RAII Lifecycle Diagram

```
  Timeline of a single program run:
  ════════════════════════════════════════════════════════════════

  main() begins
      │
      ├─► Renderer rend(...)        constructor: LoadTexture x2 ✓
      │
      ├─► AudioManager audio(...)   constructor: LoadSound x3 ✓
      │
      ├─► Player, BulletManager,
      │   EnemyManager, ShieldPickup,
      │   Starfield, GameState       constructors: set initial state ✓
      │
      ├──────────────────────────────────────────────────────────
      │             GAME LOOP RUNS (60 fps)
      ├──────────────────────────────────────────────────────────
      │
      ├─► CloseAudioDevice()        explicit (raylib global)
      ├─► CloseWindow()             explicit (raylib global)
      │
      │   } end of main() scope
      │
      ├─► ~AudioManager() runs      UnloadSound x3 ← automatic ✓
      └─► ~Renderer() runs          UnloadTexture x2 ← automatic ✓

  ════════════════════════════════════════════════════════════════

  C version required:               C++ version:
  ──────────────────────────────    ────────────────────────────
  Audio_Unload(&audio);   ← MUST    automatic — cannot forget
  Renderer_Unload(&rend); ← MUST    automatic — cannot forget
  (forget either = silent leak)     (forget = impossible)
```

---

## 💥 Collision System Diagram

```
  Every frame — 3 collision checks:
  ════════════════════════════════════════════════════════════════

  CHECK 1: Bullet vs Enemy
  ─────────────────────────────────────────────────────────────
  for each active Bullet b:
    │
    bRect = Collision::makeBulletRect(b.pos.x, b.pos.y)
    │       → Rectangle{ x-2, y-2, 4, 4 }
    │
    for each active Enemy e:
      │
      eRect = Collision::makeEnemyRect(e.pos.x, e.pos.y, w, h, scale)
      │       → Rectangle{ x, y, texW*scale, texH*scale }
      │
      Collision::check(bRect, eRect)?
      │   YES ──► b.active = false
      │            enemies.respawn(e, speed, SCREEN_W)   [OCP]
      │            audio.playExplosion()                  [DIP]
      │            rand%100 < 60 → shield.spawn(...)     [OCP]
      │            gs.addScore(10)                        [SRP]
      │            break
      │   NO  ──► continue to next enemy


  CHECK 2: Player vs Enemy
  ─────────────────────────────────────────────────────────────
  pRect = Collision::makePlayerRect(player.pos, w, h, scale)

  for each active Enemy e:
    eRect = Collision::makeEnemyRect(e.pos, w, h, scale)
    │
    Collision::check(pRect, eRect)?
    │   YES + !shieldActive ──► player = Player(W, H)   reset
    │                            enemies.init(speed, W)  reset
    │                            gs.reset(time)          [SRP]
    │   YES + shieldActive  ──► shieldActive = false     absorb
    │   NO                  ──► continue


  CHECK 3: Player vs ShieldPickup
  ─────────────────────────────────────────────────────────────
  if shield.active:
    pRect = Collision::makePlayerRect(...)
    sRect = Collision::makeShieldPickupRect(shield.pos.x, shield.pos.y)
    │       → Rectangle{ x-10, y-10, 20, 20 }
    │
    Collision::check(pRect, sRect)?
        YES ──► shieldActive  = true
                shield.active = false

  ════════════════════════════════════════════════════════════════
```

---

## 🛠 How to Build and Run

### Prerequisites

| Tool | Version | Notes |
|---|---|---|
| [raylib](https://www.raylib.com/) | 5.x | System-wide install |
| g++ | C++17 support | MinGW on Windows; system g++ on Linux/macOS |
| VS Code | Any | C/C++ extension recommended |

### Build in VS Code

```
Ctrl + Shift + B
```

Runs **Build SpaceShooter (C++)** from `.vscode/tasks.json`.

### Build in terminal

```bash
g++ -std=c++17 \
    main.cpp Renderer.cpp AudioManager.cpp Collision.cpp \
    GameState.cpp Player.cpp Bullet.cpp Enemy.cpp \
    ShieldPickup.cpp Starfield.cpp \
    -lraylib -lm -o SpaceShooter
```

### Run

```bash
./SpaceShooter        # Linux / macOS
./Debug/SpaceShooter.exe      # Windows
```

> Run from inside the `SpaceShooterCPP/` folder so `assets/` resolves correctly.

---

## 🌿 Git Workflow

```bash
# Branch from the SOLID C version
git checkout solid-refactor
git checkout -b cpp-conversion

# One commit per converted module — clean, traceable history
git add GameState.hpp GameState.cpp
git commit -m "cpp: game_state → GameState class with constructor/reset"

git add Renderer.hpp Renderer.cpp
git commit -m "cpp: renderer → Renderer class with RAII destructor"

git add AudioManager.hpp AudioManager.cpp
git commit -m "cpp: audio → AudioManager RAII + private fields + const methods"

git add Collision.hpp Collision.cpp
git commit -m "cpp: collision → namespace Collision with make*Rect helpers"

git add Player.hpp Player.cpp
git commit -m "cpp: player → Player class; draw() is now a const method"

git add Bullet.hpp Bullet.cpp
git commit -m "cpp: bullet → BulletManager with std::array"

git add Enemy.hpp Enemy.cpp
git commit -m "cpp: enemy → EnemyManager with std::array + respawn()"

git add ShieldPickup.hpp ShieldPickup.cpp
git commit -m "cpp: shield → ShieldPickup class; updatePlayerShield() static"

git add Starfield.hpp Starfield.cpp
git commit -m "cpp: starfield → Starfield class; draw() now const (C was missing)"

git add main.cpp .vscode/tasks.json .gitignore README.md
git commit -m "cpp: main.c → main.cpp; RAII removes all manual Init/Unload calls"

# Push to GitHub
git push origin cpp-conversion
```

Open a **Pull Request** from `cpp-conversion` → `solid-refactor` on GitHub.

---

## 🤖 AI Prompts Used

### Prompt 1 — Full C to C++ conversion

```
I have a SOLID-refactored Space Shooter game in C using raylib.
Here are all source files: [attach all .h and .c files]

Convert the entire project to C++17 with these exact rules:
1. Every C typedef struct + free functions → C++ class with
   constructor and destructor
2. Use RAII: constructor loads assets, destructor unloads automatically
3. Free functions like Audio_PlayShoot(&a) → const member methods
   like a.playShoot()
4. Collision free functions → namespace Collision { check(), make*Rect() }
5. Raw C arrays → std::array with range-based for loops
6. C-style casts (float)x → static_cast<float>(x)
7. const parameter → const member method: void draw() const
8. File extensions: .hpp and .cpp, PascalCase filenames
9. Keep ALL 5 SOLID principles with SRP/OCP/LSP/ISP/DIP comments
10. Do NOT add any new gameplay features — pure language conversion only
11. Include .vscode/tasks.json using g++ -std=c++17
12. Keep identical gameplay — same speed, scoring, and controls
```

---

### Prompt 2 — README with diagrams

```
Write a complete professional GitHub README.md for my C++17 SOLID
Space Shooter project. Include:

1. ASCII architecture diagram showing all modules and how main.cpp
   sits above them with raylib below
2. Full C vs C++ conversion table covering every language-level change
3. Side-by-side C vs C++ code comparisons for EVERY module using
   exact code from the real files
4. Explanation of why namespace was used for Collision instead of class
5. A section for each SOLID principle with ASCII table or diagram
   showing exactly where it is applied in the C++ code
6. ASCII game loop flowchart showing all 8 steps with decision branches
7. ASCII RAII lifecycle diagram showing constructor/destructor timing
8. ASCII collision system diagram showing all 3 collision checks
9. Build instructions using g++ -std=c++17
10. Git workflow with one commit per module
11. The AI prompts that generated this project
```

---

### Prompt 3 — Add a new enemy type following OCP

```
In my C++17 Space Shooter, add a BossEnemy that:
- Is larger than regular enemies (scale 0.3 instead of 0.125)
- Has 3 hit points and needs 3 bullets to kill
- Moves in a horizontal sine wave pattern
- Drops a guaranteed shield pickup when killed

Rules — follow OCP strictly:
- Do NOT modify EnemyManager, Enemy struct, or main.cpp
- Create new files only: BossEnemy.hpp and BossEnemy.cpp
- Follow SRP: BossEnemy.cpp handles only boss state and behaviour
- Follow ISP: BossEnemy.hpp declares only boss-related things
- Follow DIP: main.cpp calls BossEnemy methods, not raw fields
- Follow LSP: BossEnemy::draw() must be const

Show the complete new files and the minimal additions to main.cpp.
```

---

### Prompt 4 — Convert to smart pointers

```
In my C++17 Space Shooter, convert Renderer and AudioManager in
main.cpp to use std::unique_ptr for explicit ownership:

  std::unique_ptr<Renderer>     rend;
  std::unique_ptr<AudioManager> audio;

Rules:
- Use std::make_unique<>() for construction
- Access members with rend->playerTexture and audio->playShoot()
- Explain why std::unique_ptr is appropriate here vs std::shared_ptr
- Keep all SOLID principles intact
- Show only the changed parts of main.cpp
```

---

## 📊 Complete Conversion Summary

| C module | C++ module | Key C++ feature applied |
|---|---|---|
| `game_state.h/c` | `GameState.hpp/cpp` | Constructor replaces Init() |
| `renderer.h/c` | `Renderer.hpp/cpp` | RAII destructor replaces Unload() |
| `audio.h/c` | `AudioManager.hpp/cpp` | RAII + `const` methods + `private` fields |
| `collision.h/c` | `Collision.hpp/cpp` | `namespace` — no class needed |
| `player.h/c` | `Player.hpp/cpp` | Constructor + `const` draw method |
| `bullet.h/c` | `Bullet.hpp/cpp` | `BulletManager` + `std::array` |
| `enemy.h/c` | `Enemy.hpp/cpp` | `EnemyManager` + `std::array` + ref respawn |
| `shield.h/c` | `ShieldPickup.hpp/cpp` | `static` helper method |
| `starfield.h/c` | `Starfield.hpp/cpp` | Constructor + `const` draw (bug fix from C) |
| `main.c` | `main.cpp` | RAII, range-for, `static_cast`, no manual cleanup |

---

## 🔧 Technologies

| Tool | Role |
|---|---|
| **C++17** | Language |
| **raylib 5.x** | Graphics and audio |
| **g++ / MinGW** | Compiler |
| **std::array** | Fixed-size arrays — replaces raw C arrays |
| **RAII** | Automatic resource management |
| **VS Code** | Editor |
| **Git + GitHub** | Version control |

---

*Converted from the C SOLID version as part of Advanced Programming Lab —
demonstrating how SOLID Object-Oriented Design principles translate
naturally and more powerfully into idiomatic C++17.*
