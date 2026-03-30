# 🚀 Space Shooter — SOLID Refactored

> A **raylib** C game refactored from a monolithic codebase into a clean,
> maintainable architecture by applying all five **SOLID** Object-Oriented
> Design principles — in pure C.

---

## 📋 Table of Contents

- [Game Overview](#-game-overview)
- [Project Structure](#-project-structure)
- [Before vs After — The Refactor](#-before-vs-after--the-refactor)
- [SOLID Principles Applied](#-solid-principles-applied)
  - [S — Single Responsibility](#s--single-responsibility-principle-srp)
  - [O — Open/Closed](#o--openclosed-principle-ocp)
  - [L — Liskov Substitution](#l--liskov-substitution-principle-lsp)
  - [I — Interface Segregation](#i--interface-segregation-principle-isp)
  - [D — Dependency Inversion](#d--dependency-inversion-principle-dip)
- [How to Build and Run](#-how-to-build-and-run)
- [Git Workflow](#-git-workflow)
- [AI Prompts Used](#-ai-prompts-that-generated-this-refactor)

---

## 🎮 Game Overview

Space Shooter is a top-down arcade game built with **raylib** in C.

| Control | Action |
|---|---|
| `←` `→` `↑` `↓` Arrow keys | Move the spaceship |
| `Space` | Fire a bullet |
| `Enter` | Start the game |

**Gameplay features:**
- Enemies fall from the top and drift toward the player
- Shoot enemies to earn **+10 points** each
- A **shield power-up** (green circle) drops on 60 % of kills — collect it to absorb one enemy hit
- Every **60 seconds** the level increases and enemies speed up
- High score is preserved across deaths

---

## 📁 Project Structure

```
SpaceShooter/
│
├── assets/                  ← Game assets (loaded at runtime)
│   ├── player.png
│   ├── enemy.png
│   ├── shoot.wav
│   ├── explosion.wav
│   └── levelup.wav
│
├── .vscode/
│   └── tasks.json           ← VS Code build + run tasks
│
│── ORIGINAL FILES (refactored in-place) ──────────────────────
├── bullet.h / bullet.c      ← Bullet: fire, move, draw
├── enemy.h  / enemy.c       ← Enemy: spawn, move, draw  [+ RespawnEnemy]
├── player.h / player.c      ← Player: move, draw
├── shield.h / shield.c      ← Shield pickup lifecycle   [renamed + split]
├── starfield.h / starfield.c← Scrolling star background
├── main.c                   ← Game loop orchestration only
│
│── NEW FILES (added for SOLID) ────────────────────────────────
├── renderer.h / renderer.c  ← Texture asset management      [SRP/ISP/DIP]
├── audio.h    / audio.c     ← Sound asset management        [SRP/ISP/DIP]
├── collision.h / collision.c← Collision detection interface  [SRP/ISP/DIP]
└── game_state.h / game_state.c ← Score + level state        [SRP/OCP]
```

**Total: 14 source files across 10 focused modules** — up from 6 files where
`main.c` alone handled 9 different concerns.

---

## 🔄 Before vs After — The Refactor

### File count and responsibility

| | Before | After |
|---|---|---|
| Total `.c` files | 6 | 10 |
| Lines in `main.c` | ~150 | ~130 (but pure orchestration) |
| Responsibilities in `main.c` | 9 | 1 |
| New focused modules added | — | 4 |
| Raw raylib calls in `main.c` | 14+ | 0 (all behind interfaces) |

---

### main.c — the most dramatic change

**BEFORE** — `main.c` did everything itself: loaded assets, tracked score,
built collision rectangles, called `CheckCollisionRecs()`, called `PlaySound()`,
compared time for level-up, and reset enemy positions inline:

```c
// BEFORE — asset loading scattered in main()
Texture2D playerTexture = LoadTexture("assets/player.png");
Texture2D enemyTexture  = LoadTexture("assets/enemy.png");
Sound shootSound        = LoadSound("assets/shoot.wav");
Sound explosionSound    = LoadSound("assets/explosion.wav");
Sound levelupSound      = LoadSound("assets/levelup.wav");

// BEFORE — score and level as raw variables
int score = 0, highScore = 0, level = 1;
double lastLevelUpTime = GetTime();

// BEFORE — level-up logic inline in the loop
if (currentTime - lastLevelUpTime > 60.0) {
    level++;
    lastLevelUpTime = currentTime;
    PlaySound(levelupSound);
}

// BEFORE — collision rectangles built manually, every time
Rectangle bulletRec = { bullets[i].pos.x - 2, bullets[i].pos.y - 2, 4, 4 };
Rectangle enemyRec  = { enemies[j].pos.x, enemies[j].pos.y,
                        enemyTexture.width * 0.125f,
                        enemyTexture.height * 0.125f };
if (CheckCollisionRecs(bulletRec, enemyRec)) {
    bullets[i].active = false;
    enemies[j].pos = (Vector2){ rand() % (screenWidth - ...), -50 - rand()%150 };
    enemies[j].speed = 2.0f + 0.5*(level-1);
    PlaySound(explosionSound);
    if (rand()%100 < 60) {
        shield.pos = killPos;
        shield.active = true;
        shield.expireTime = currentTime + 5.0;
    }
    score += 10;
    if (score > highScore) highScore = score;
}
```

**AFTER** — `main.c` delegates everything. Every concern has a home:

```c
// AFTER — assets loaded through abstractions (DIP)
Renderer_Init(&rend, "assets/player.png", "assets/enemy.png");
Audio_Init(&audio, "assets/shoot.wav", "assets/explosion.wav", "assets/levelup.wav");

// AFTER — score/level managed by GameState module (SRP)
GameState gs;
GameState_Init(&gs, GetTime());

// AFTER — level-up rule lives in GameState, not here (OCP)
if (GameState_TryLevelUp(&gs, currentTime, 60.0))
    Audio_PlayLevelUp(&audio);

// AFTER — collision rects built by named helpers (DIP/SRP)
Rectangle bRect = MakeBulletRect(bullets[i].pos.x, bullets[i].pos.y);
Rectangle eRect = MakeEnemyRect(enemies[j].pos.x, enemies[j].pos.y,
                                rend.enemyTexture.width,
                                rend.enemyTexture.height,
                                rend.enemyScale);
if (CheckCollision(bRect, eRect)) {
    bullets[i].active = false;
    RespawnEnemy(&enemies[j], EnemySpeed(gs.level), SCREEN_W); // OCP
    Audio_PlayExplosion(&audio);                                 // DIP
    ShieldPickup_Spawn(&shield, killPos, currentTime, 5.0);     // OCP
    GameState_AddScore(&gs, 10);                                 // SRP
}
```

---

### shield.h — renamed and properly split

**BEFORE** — one struct mixed the world pickup with the player's timer:

```c
// BEFORE — shield.h
typedef struct {
    Vector2 pos;
    bool active;
    double expireTime;
} Shield;

void InitShield(Shield *shield);
void UpdateShield(Shield *shield, bool *playerShield, double currentTime);
void DrawShield(Shield *shield);     // takes non-const — could mutate state
```

**AFTER** — renamed to `ShieldPickup`, responsibilities separated,
`DrawShield` is now `const`, and the player-timer logic gets its own function:

```c
// AFTER — shield.h
typedef struct {
    Vector2 pos;
    bool    active;
    double  expireTime;
} ShieldPickup;

void ShieldPickup_Init  (ShieldPickup *s);
void ShieldPickup_Spawn (ShieldPickup *s, Vector2 pos,
                         double currentTime, double durationSeconds);
void ShieldPickup_Update(ShieldPickup *s, double currentTime);
void ShieldPickup_Draw  (const ShieldPickup *s);    // LSP: const enforced

// DIP: main.c calls this instead of comparing currentTime > expireTime itself
void PlayerShield_Update(bool *shieldActive, const ShieldPickup *s,
                         double currentTime);
```

---

### enemy.h — RespawnEnemy extracted (OCP)

**BEFORE** — enemy reset was a 3-line block copy-pasted twice in `main.c`:

```c
// BEFORE — inline in main.c, duplicated twice
enemies[j].pos = (Vector2){
    rand() % (screenWidth - (int)(enemyTexture.width * 0.125f)),
    -50 - rand() % 150
};
enemies[j].speed = 2.0f + 0.5 * (level - 1);
```

**AFTER** — one function owns the algorithm:

```c
// AFTER — enemy.h
void RespawnEnemy(Enemy *enemy, float speed, int screenWidth);

// AFTER — enemy.c
void RespawnEnemy(Enemy *enemy, float speed, int screenWidth) {
    enemy->pos   = (Vector2){
        (float)(rand() % (screenWidth - 64)),
        -50.0f - (float)(rand() % 150)
    };
    enemy->speed  = speed;
    enemy->active = true;
}
```

---

### Draw functions — const added everywhere (LSP)

**BEFORE** — draw functions took mutable pointers; nothing prevented
accidental state mutation inside a draw call:

```c
// BEFORE
void DrawBullets(Bullet bullets[]);
void DrawEnemies(Enemy enemies[], Texture2D texture, float scale);
void DrawPlayer(Player *player, Texture2D texture, float scale, bool shieldActive);
void DrawShield(Shield *shield);
```

**AFTER** — `const` is enforced at compile time across every draw function:

```c
// AFTER
void DrawBullets (const Bullet bullets[]);
void DrawEnemies (const Enemy enemies[], Texture2D texture, float scale);
void DrawPlayer  (const Player *player, Texture2D texture, float scale, bool shieldActive);
void ShieldPickup_Draw(const ShieldPickup *s);
void DrawStars   (const Star stars[]);
```

---

## 🧱 SOLID Principles Applied

### S — Single Responsibility Principle (SRP)

> *"A module should have one, and only one, reason to change."*

Every `.c` file in the refactored project has exactly one job:

| Module | Its single responsibility | Reason it would change |
|---|---|---|
| `main.c` | Game loop orchestration | Loop structure changes |
| `renderer.c` | Texture loading/unloading | Asset format changes |
| `audio.c` | Sound loading/playback | Audio library changes |
| `collision.c` | Overlap detection + rect building | Collision algorithm changes |
| `game_state.c` | Score, high-score, level | Scoring rules change |
| `player.c` | Player movement + drawing | Movement mechanics change |
| `bullet.c` | Bullet firing + movement + drawing | Bullet behaviour changes |
| `enemy.c` | Enemy spawning + movement + drawing | Enemy behaviour changes |
| `shield.c` | Shield pickup lifecycle | Pickup mechanic changes |
| `starfield.c` | Background star scrolling | Visual style changes |

**In the original code,** `main.c` had 9 reasons to change simultaneously.
Touching one concern required understanding the entire 150-line file.

---

### O — Open/Closed Principle (OCP)

> *"Software entities should be open for extension, closed for modification."*

Three concrete examples in this project:

**1. Difficulty curve — `EnemySpeed()` in `main.c`**

```c
// One function. Change the curve — nothing else touches it.
static float EnemySpeed(int level) {
    return 2.0f + 0.5f * (float)(level - 1);
}
// Want exponential scaling? Change only this line.
// return 1.5f * powf(1.2f, (float)(level - 1));
```

**2. Level-up rule — `GameState_TryLevelUp()` in `game_state.c`**

```c
// Currently time-based. Switch to score-based by editing only this function.
bool GameState_TryLevelUp(GameState *gs, double currentTime,
                          double intervalSeconds) {
    if (currentTime - gs->lastLevelUpTime >= intervalSeconds) {
        gs->level++;
        gs->lastLevelUpTime = currentTime;
        return true;
    }
    return false;
}
// main.c never changes — it just calls GameState_TryLevelUp().
```

**3. Enemy respawn — `RespawnEnemy()` in `enemy.c`**

```c
// Want formation spawning? Extend this function — main.c is untouched.
void RespawnEnemy(Enemy *enemy, float speed, int screenWidth) {
    enemy->pos   = (Vector2){
        (float)(rand() % (screenWidth - 64)),
        -50.0f - (float)(rand() % 150)
    };
    enemy->speed  = speed;
    enemy->active = true;
}
```

---

### L — Liskov Substitution Principle (LSP)

> *"Subtypes must be substitutable for their base types without altering
> the correctness of the program."*

In C, LSP is applied through **consistent contracts on functions**.
The key enforcement here is `const` on all draw functions:

```c
// Every Draw* function guarantees: calling it never changes game state.
void DrawBullets (const Bullet bullets[]);
void DrawEnemies (const Enemy enemies[], Texture2D texture, float scale);
void DrawPlayer  (const Player *player, Texture2D texture, float scale, bool shieldActive);
void ShieldPickup_Draw(const ShieldPickup *s);
void DrawStars   (const Star stars[]);
```

The same contract applies to `RespawnEnemy()` — any `Enemy*` can be passed
and the behaviour is identical regardless of prior state:

```c
// LSP: the contract holds for every Enemy, active or inactive
void RespawnEnemy(Enemy *enemy, float speed, int screenWidth);
```

**Before the refactor,** `DrawShield(Shield *shield)` took a non-const pointer.
Nothing stopped the function from accidentally writing to `shield->active`
inside a draw call. The compiler gave no warning.

---

### I — Interface Segregation Principle (ISP)

> *"Clients should not be forced to depend on interfaces they do not use."*

Every header is as small as it can be:

| Header | Declarations inside |
|---|---|
| `renderer.h` | `Renderer` struct · `Renderer_Init` · `Renderer_Unload` |
| `audio.h` | `AudioManager` struct · `Audio_Init` · `Audio_Unload` · 3× `Audio_Play*` |
| `collision.h` | `CheckCollision` · 4× `Make*Rect` |
| `game_state.h` | `GameState` struct · `Init` · `AddScore` · `TryLevelUp` · `Reset` |
| `shield.h` | `ShieldPickup` struct · 4 pickup functions · `PlayerShield_Update` |

A file that only needs to play a sound includes `audio.h` — it does **not**
get `Renderer`, `Enemy`, or collision rectangles dragged in with it.

**Before:** a hypothetical `utils.h` would have forced every file to include
everything. With one giant header, changing any declaration recompiles the
entire project.

---

### D — Dependency Inversion Principle (DIP)

> *"High-level modules should not depend on low-level modules.
> Both should depend on abstractions."*

`main.c` is the high-level policy module. In the original code it depended
**directly** on raylib for 14+ operations. After refactoring, every one of
those dependencies is inverted:

| `main.c` now calls | Instead of (raylib directly) |
|---|---|
| `Renderer_Init(&rend, ...)` | `LoadTexture(...)` ×2 |
| `Renderer_Unload(&rend)` | `UnloadTexture(...)` ×2 |
| `Audio_PlayShoot(&audio)` | `PlaySound(shootSound)` |
| `Audio_PlayExplosion(&audio)` | `PlaySound(explosionSound)` |
| `Audio_PlayLevelUp(&audio)` | `PlaySound(levelupSound)` |
| `CheckCollision(a, b)` | `CheckCollisionRecs(a, b)` |
| `MakeBulletRect(x, y)` | `(Rectangle){ x-2, y-2, 4, 4 }` inline |
| `MakeEnemyRect(x, y, ...)` | `(Rectangle){ x, y, w*0.125, h*0.125 }` inline |
| `GameState_AddScore(&gs, 10)` | `score += 10; if (score > highScore)...` |
| `GameState_TryLevelUp(...)` | `if (currentTime - lastLevelUpTime > 60.0)` |
| `PlayerShield_Update(...)` | `if (*playerShield && currentTime > shield.expireTime)` |
| `RespawnEnemy(&enemies[j], ...)` | 3-line inline reset block ×2 |
| `ShieldPickup_Spawn(...)` | `shield.pos = ...; shield.active = true; shield.expireTime = ...` |

**Consequence:** swapping raylib for a different graphics library only requires
editing the module `.c` files. `main.c` stays completely unchanged.

---

## 🛠 How to Build and Run

### Prerequisites

- [raylib](https://www.raylib.com/) installed on your system
- GCC (MinGW on Windows, system GCC on Linux/macOS)
- VS Code with the C/C++ extension

### Build via VS Code

Press `Ctrl+Shift+B` — this runs the **Build SpaceShooter** task defined
in `.vscode/tasks.json`.

### Build via terminal

```bash
gcc main.c renderer.c audio.c collision.c game_state.c \
    player.c bullet.c enemy.c shield.c starfield.c \
    -lraylib -lm -o SpaceShooter

or,  ./Debug/SpaceShooter.exe
```

### Run

```bash
./SpaceShooter        # Linux / macOS
SpaceShooter.exe      # Windows
```

> **Important:** run from inside the `SpaceShooter/` folder so the
> `assets/` path resolves correctly.

---

## 🌿 Git Workflow

This project follows the instructions for branching and committing one
SOLID principle at a time.

```bash
# Step 1 — original code already on main/trunk

# Step 2 — create the refactor branch
git checkout -b solid-refactor

# Step 3 — commit each SOLID principle separately
git add game_state.h game_state.c
git commit -m "SRP: extract game_state module — score and level have one home"

git add renderer.h renderer.c audio.h audio.c
git commit -m "SRP + ISP: extract renderer and audio into focused single-concern modules"

git add collision.h collision.c
git commit -m "ISP + DIP: extract collision module — main.c no longer calls raylib for overlap tests"

git add enemy.h enemy.c
git commit -m "OCP: add RespawnEnemy() — respawn algorithm lives in exactly one place"

git add bullet.h bullet.c player.h player.c shield.h shield.c starfield.h starfield.c
git commit -m "LSP: enforce const on all Draw* functions — drawing never mutates state"

git add main.c
git commit -m "DIP: main.c depends on module abstractions only — zero direct raylib calls for audio/collision/assets"

git add README.md .vscode/tasks.json .gitignore
git commit -m "docs: add README with SOLID explanation and VS Code build config"

# Step 4 — push to GitHub
git push origin solid-refactor
```

Then open a **Pull Request** from `solid-refactor` → `main` on GitHub.
Each commit maps to exactly one SOLID principle — reviewers can follow
the transformation step by step.

---

## 🤖 AI Prompts That Generated This Refactor

The following prompts were used with an AI assistant (Claude) to produce
the SOLID-refactored version of this project. They are included here so
any team member can reproduce, extend, or re-prompt for further changes.

---

### Prompt 1 — Understanding the task

```
I have a C game project (Space Shooter using raylib) with these files:
main.c, bullet.h/c, enemy.h/c, player.h/c, shield.h/c, starfield.h/c.

Explain step by step how to refactor a C project into SOLID OOD:
Step 1: original code on GitHub trunk
Step 2: create a branch called solid-refactor
Step 3: apply SRP, OCP, LSP, ISP, DIP to the project
Step 4: commit changes to GitHub

Explain each principle clearly with examples before I start coding.
```

---

### Prompt 2 — Full refactor with source files

```
Here are all my C source files for a Space Shooter game using raylib:
[attached: main.c, bullet.h, bullet.c, enemy.h, enemy.c,
           player.h, player.c, shield.h, shield.c,
           starfield.h, starfield.c]

Apply all five SOLID principles properly to this exact project:
- SRP: every .c file must have one reason to change
- OCP: isolate things that change (speed formula, level-up rule, respawn)
- LSP: enforce const on all Draw* functions
- ISP: each header declares only its own concerns
- DIP: main.c must not call raylib directly for audio, collision, or assets

Create new modules where needed (renderer, audio, collision, game_state).
Keep the gameplay 100% identical. Add SOLID comments to every file.
```

---

### Prompt 3 — File structure for VS Code

```
In these screenshots of my Windows Explorer folder, which files should
I keep, which should I replace, and which new files should I add so that
I can run the SOLID-refactored Space Shooter project through VS Code?

Also give me a proper .vscode/tasks.json so I can build and run with
Ctrl+Shift+B without needing Visual Studio.
```

---

### Prompt 4 — README generation

```
Create a professional, attractive, fully informative GitHub README.md
for my SOLID-refactored Space Shooter C project. Include:

1. Game overview with controls table
2. Full project structure diagram
3. Before vs After code comparisons for the most important changes
   (main.c, shield.h, enemy respawn, Draw* functions)
4. A section for each SOLID principle with clear explanation +
   concrete code examples from this actual project
5. Build instructions for VS Code and terminal
6. Git workflow with one commit per principle
7. The AI prompts that were used to generate this refactor

Make it well-structured with headers, tables, and code blocks.
```

---

### Prompt 5 — Extending the project (future use)

```
My Space Shooter C project is now SOLID-refactored with these modules:
renderer, audio, collision, game_state, player, bullet, enemy, shield, starfield.

I want to add a new enemy type called "BossEnemy" that is larger,
has 3 health points, and moves in a sine wave pattern.

Following OCP: add this without modifying any existing module.
Following ISP: create boss_enemy.h with only boss-related declarations.
Following SRP: boss_enemy.c handles only boss state and behaviour.
Following DIP: main.c should call BossEnemy functions, not access fields directly.
```

---

## 📊 SOLID Principle Summary Table

| Principle | Problem it solved | Where applied |
|---|---|---|
| **SRP** | `main.c` had 9 responsibilities | Split into 4 new modules + focused existing ones |
| **OCP** | Respawn/speed/level-up logic duplicated | `RespawnEnemy()`, `EnemySpeed()`, `GameState_TryLevelUp()` |
| **LSP** | Draw functions could mutate state | `const` on all `Draw*` parameters |
| **ISP** | No boundary between audio, render, collision | Separate focused headers for each concern |
| **DIP** | `main.c` called 14+ raylib APIs directly | All behind `renderer`, `audio`, `collision` abstractions |

---

## 🔧 Technologies

- **Language:** C (C99)
- **Graphics/Audio:** [raylib 5.x](https://www.raylib.com/)
- **Build:** GCC / MinGW
- **Editor:** VS Code
- **Version Control:** Git + GitHub

---

*Refactored as part of Advanced Programming Lab — demonstrating SOLID
Object-Oriented Design principles applied to a procedural C codebase.*

