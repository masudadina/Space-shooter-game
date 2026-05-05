# Space Shooter Game - MVC Architecture Documentation

## Table of Contents
1. [MVC Architecture Overview](#mvc-architecture-overview)
2. [Layer-by-Layer Breakdown](#layer-by-layer-breakdown)
3. [Folder Structure](#folder-structure)
4. [Game Features](#game-features)
5. [SOLID Principles Implementation](#solid-principles-implementation)

---

## Layer-by-Layer Breakdown

### 🎮 **CONTROLLER LAYER**

#### `GameController` (`GameController.hpp` / `GameController.cpp`)
The main game controller that orchestrates the entire game loop and manages interactions between models and views.

**Responsibilities:**
- Initializes all game components (Model and View)
- Runs the main game loop with four distinct phases:
  1. `processInput()` - Handles player input
  2. `update()` - Updates all game entities
  3. `handleCollisions()` - Manages collision detection and logic
  4. `render()` - Displays everything to screen
- Manages difficulty scaling via `enemySpeed()` function that increases enemy speed with each level

**Key Features:**
- Screen dimensions: 1600x850 pixels
- 60 FPS target
- Dynamically adjusts enemy speed based on game level

**Game Loop:**
```
while (game running) {
    processInput() → Player movement and shooting
    update()       → Update all entities and game state
    handleCollisions() → Check collisions and apply effects
    render()       → Draw everything to screen
}
```

---

### 📊 **MODEL LAYER**

The Model layer contains all game entities and business logic. Each component follows the Single Responsibility Principle.

#### `Player` (`Player.hpp` / `Player.cpp`)
Represents the player character and manages player-specific behavior.

**Attributes:**
- `pos` (Vector2) - Current position on screen
- `speed` (float) - Movement speed (5.0 units/frame)

**Responsibilities:**
- Initialize player at screen center-bottom
- Handle movement input (LEFT, RIGHT, UP, DOWN keys)
- Enforce screen boundary constraints
- Draw player sprite with optional shield visual

**SOLID Compliance:**
- **SRP**: Only handles player position and movement
- **LSP**: `draw()` is const - never mutates player state during rendering

---

#### `Bullet` & `BulletManager` (`Bullet.hpp` / `Bullet.cpp`)
Manages all player projectiles in the game.

**Structure:**
- `struct Bullet` - Plain data container (position, active state)
- `class BulletManager` - Encapsulates bullet array and behavior

**Max Bullets:** 10 concurrent bullets

**Responsibilities:**
- Fire bullets from player position on SPACE key press
- Update bullet positions (move upward at 5.0 units/frame)
- Deactivate bullets when they leave screen
- Render active bullets as yellow circles (radius 5)

**SOLID Compliance:**
- **SRP**: Bullet movement and firing only - collision outcomes are caller's responsibility
- **ISP**: No dependencies on Player or Enemy types
- **LSP**: `draw()` is const - no side effects

---

#### `Enemy` & `EnemyManager` (`Enemy.hpp` / `Enemy.cpp`)
Manages all enemy entities and their behavior.

**Structure:**
- `struct Enemy` - Plain data container (position, active state, speed)
- `class EnemyManager` - Owns array of enemies and all enemy behavior
- **Max Enemies:** 10 concurrent enemies

**Responsibilities:**
- Initialize enemies at game start
- Update enemy positions:
  - Drift toward or away from player (50% chance per frame)
  - Fall downward at speed determined by current game level
  - Respawn above screen when they exit bottom
- Respawn enemies when destroyed by bullets
- Render enemy sprites

**Respawn Algorithm:**
- Random X position within screen width
- Spawn above screen (Y = -50 to -200 pixels)
- Speed adjusted per current level

**SOLID Compliance:**
- **SRP**: Only enemy movement and spawning
- **OCP**: `respawn()` method encapsulates spawn behavior - new patterns extend here, never main.cpp
- **LSP**: All enemies follow same movement contract - draw() is const

---

#### `GameState` (`GameState.hpp` / `GameState.cpp`)
Manages all game progression and scoring data.

**Attributes:**
- `score` (int) - Current score
- `highScore` (int) - Best score achieved (persists across deaths)
- `level` (int) - Current difficulty level (starts at 1)
- `lastLevelUpTime` (double) - Timestamp of last level increase

**Responsibilities:**
- Track player score (incremented by 10 points per enemy killed)
- Maintain high score across game sessions
- Manage level progression (level increases every 60 seconds)
- Reset game state for new game

**SOLID Compliance:**
- **SRP**: Only score and level management - no other concerns
- **OCP**: Level-up condition defined in `tryLevelUp()` - change difficulty rules without touching main.cpp

---

#### `ShieldPickup` (`ShieldPickup.hpp` / `ShieldPickup.cpp`)
Manages protective shield power-ups that drop from defeated enemies.

**Attributes:**
- `pos` (Vector2) - Current position
- `active` (bool) - Whether pickup is active and visible
- `expireTime` (double) - When shield expires (timestamp)

**Responsibilities:**
- Spawn shields at killed enemy positions (60% chance per kill)
- Display shield as green circle (radius 10)
- Deactivate expired shields
- Track player shield status separately via `updatePlayerShield()` static method

**Drop Duration:** 5 seconds

**SOLID Compliance:**
- **SRP**: Shield pickup lifecycle only - player shield state is separate concern
- **OCP**: Drop duration injected as parameter to `spawn()`
- **DIP**: Abstracts timer comparison so main.cpp never does raw time arithmetic
- **LSP**: `draw()` is const - no mutations during rendering

---

#### `Starfield` (`Starfield.hpp` / `Starfield.cpp`)
Manages scrolling starfield background effect.

**Structure:**
- `struct Star` - Plain data container (position, speed)
- `class Starfield` - Owns array of stars and scrolling behavior
- **Max Stars:** 100

**Responsibilities:**
- Initialize random star positions and speeds at game start
- Scroll stars downward at varying speeds (1-3 units/frame)
- Wrap stars around screen (respawn at top when exiting bottom)
- Render as white pixels

**SOLID Compliance:**
- **SRP**: Pure visual background effect only
- **ISP**: Completely isolated - self-contained background
- **LSP**: `draw()` is const - no state mutations

---

### 🎨 **VIEW LAYER**

The View layer handles all rendering and audio output, separated from game logic.

#### `Renderer` (`Renderer.hpp` / `Renderer.cpp`)
Manages game asset loading and rendering configuration.

**Responsibilities:**
- Load sprite textures at initialization:
  - Player sprite from "assets/player.png"
  - Enemy sprite from "assets/enemy.png"
- Store texture scaling factors:
  - Player scale: 0.20x (20% of original size)
  - Enemy scale: 0.125x (12.5% of original size)
- Automatic cleanup via destructor (RAII pattern)

**SOLID Compliance:**
- **SRP**: Texture asset management only
- **ISP**: Separate from audio and collision systems
- **DIP**: Main.cpp uses this abstraction instead of raylib directly

**C++ Enhancement - RAII:**
- Destructor automatically unloads textures
- Exception-safe resource cleanup

---

#### `AudioManager` (`AudioManager.hpp` / `AudioManager.cpp`)
Manages sound effects and playback.

**Sound Effects:**
- `shoot_` - Sound played when player fires (from "assets/shoot.wav")
- `explosion_` - Sound played when enemy destroyed (from "assets/explosion.wav")
- `levelup_` - Sound played when level increases (from "assets/levelup.wav")

**Responsibilities:**
- Load all sound files at initialization
- Play appropriate sounds based on game events
- Automatic cleanup via destructor (RAII pattern)

**SOLID Compliance:**
- **SRP**: Sound asset management and playback only
- **ISP**: Fully separate from visual rendering
- **DIP**: Main.cpp calls named methods (`playShoot()`, etc.) instead of raylib functions

**C++ Enhancement - RAII:**
- Destructor automatically unloads all sounds
- Safe resource management

---

### 🔍 **UTILITY LAYER**

#### `Collision` (`Collision.hpp` / `Collision.cpp`)
Namespace containing collision detection utilities and rectangle builders.

**Functions:**
- `check(Rectangle a, Rectangle b)` - Wrapper around raylib's collision detection
- `makeBulletRect(x, y)` - Creates 4x4 collision rect for bullets
- `makePlayerRect(x, y, texW, texH, scale)` - Creates scaled player collision rect
- `makeEnemyRect(x, y, texW, texH, scale)` - Creates scaled enemy collision rect
- `makeShieldPickupRect(x, y)` - Creates 20x20 collision rect for shields

**Responsibilities:**
- Encapsulate collision detection algorithm
- Keep magic numbers for collision sizing out of main.cpp
- Standardize collision rect creation across all entity types

**SOLID Compliance:**
- **SRP**: Only collision detection and rect sizing
- **DIP**: Abstracts raylib behind a custom interface
- **OCP**: New collision shapes can be added without changing existing code
- **ISP**: Callers that don't need collision never include this header

---

## Folder Structure

```
SpaceShooter/
│
├── 📄 Main Entry Point
│   └── main.cpp                    # Program entry point - initializes and runs GameController
│
├── 🎮 Controller Layer
│   ├── GameController.hpp          # Main controller - orchestrates game loop
│   └── GameController.cpp          # Controller implementation
│
├── 📊 Model Layer - Game Entities
│   ├── Player.hpp / Player.cpp     # Player entity and movement
│   ├── Enemy.hpp / Enemy.cpp       # Enemy entities and behavior
│   ├── Bullet.hpp / Bullet.cpp     # Projectile management
│   ├── ShieldPickup.hpp / ShieldPickup.cpp  # Shield power-up system
│   ├── Starfield.hpp / Starfield.cpp        # Background scrolling stars
│   └── GameState.hpp / GameState.cpp        # Score and level progression
│
├── 🎨 View Layer - Rendering & Audio
│   ├── Renderer.hpp / Renderer.cpp           # Texture loading and asset management
│   └── AudioManager.hpp / AudioManager.cpp   # Sound effect management
│
├── 🔍 Utility Layer - Collision & Logic
│   ├── Collision.hpp / Collision.cpp         # Collision detection utilities
│
├── 📂 assets/
│   ├── player.png                   # Player sprite texture
│   ├── enemy.png                    # Enemy sprite texture
│   ├── shoot.wav                    # Shooting sound effect
│   ├── explosion.wav                # Enemy destruction sound effect
│   └── levelup.wav                  # Level up notification sound
│
└── 📂 Debug/ & x64/
    └── Build artifacts and compiled executables
```

---

## File Organization by Layer

### **🎮 CONTROLLER LAYER (Orchestration)**
| File | Responsibility |
|------|-----------------|
| `GameController.cpp` | Main game loop, input processing, collision handling, entity updates, rendering coordination |

---

### **📊 MODEL LAYER (Game Logic & Data)**
| File | Responsibility |
|------|-----------------|
| `Player.cpp` | Player movement and position management |
| `Bullet.cpp` | Bullet firing, movement, and lifecycle management |
| `Enemy.cpp` | Enemy AI, movement, spawning, and respawning |
| `GameState.cpp` | Score tracking, level progression, and game state |
| `ShieldPickup.cpp` | Shield power-up spawning, expiration, and player shield logic |
| `Starfield.cpp` | Background star generation and scrolling animation |

---

### **🎨 VIEW LAYER (Presentation & Audio)**
| File | Responsibility |
|------|-----------------|
| `Renderer.cpp` | Texture loading, asset management, and rendering configuration (RAII cleanup) |
| `AudioManager.cpp` | Sound effect loading, playback triggers, and audio cleanup (RAII) |

---

### **🔍 UTILITY LAYER (Support & Helpers)**
| File | Responsibility |
|------|-----------------|
| `Collision.cpp` | Collision detection and rectangle builder functions |

---

### **📄 Entry Point**
| File | Responsibility |
|------|-----------------|
| `main.cpp` | Program initialization - creates and runs GameController |

---

## Game Features

### 🎮 **Core Gameplay**

1. **Player Control**
   - Move left/right/up/down with arrow keys
   - Screen-bounded movement (player stays within screen)
   - Fire bullets with SPACE key
   - Up to 10 concurrent bullets

2. **Enemy System**
   - 10 enemies spawn continuously from top of screen
   - Enemies drift toward/away from player randomly
   - Enemies fall downward with increasing speed per level
   - Destroyed enemies respawn above screen

3. **Shooting Mechanics**
   - One bullet fires per SPACE press
   - Bullets travel upward automatically
   - Bullets deactivate when leaving top of screen
   - Direct hit on enemy = 10 points score

4. **Collision System**
   - Bullet-vs-Enemy collision detection
   - Pixel-perfect collision via scaled rectangles
   - Shield pickup collision with player
   - Enemy-exit-screen detection

### 🛡️ **Power-Up System**

**Shield Pickups**
- Drop from 60% of destroyed enemies
- Last for 5 seconds after pickup
- Visual indicator: Blue circle around player when active
- Persists through multiple lives if active

### 📊 **Progression System**

**Level & Difficulty**
- Level increases every 60 seconds of gameplay
- Enemy speed increases with each level:
  - Level 1: Base speed 2.0 units/frame
  - Level 2+: Speed = 2.0 + 0.5 × (level - 1)
- Level displayed in-game
- Difficulty scales infinitely

**Scoring**
- 10 points per enemy killed
- Score accumulates throughout game session
- High score persists across deaths
- Score displayed in top-left corner

### 🎬 **Visual Effects**

1. **Starfield Background**
   - 100 scrolling stars
   - Varying scroll speeds for depth effect
   - Infinite scrolling illusion

2. **Player Graphics**
   - Scaled sprite (20% of original)
   - Blue shield circle when protected

3. **Enemy Graphics**
   - Scaled sprites (12.5% of original)
   - Continuous animation via update cycle

4. **Bullet Graphics**
   - Yellow circles (5-pixel radius)
   - Simple but clear visuals

5. **Shield Graphics**
   - Green circle (10-pixel radius)
   - Visible pickup indicator

### 🔊 **Audio Features**

1. **Sound Effects**
   - Shoot sound on SPACE press
   - Explosion sound on enemy destruction
   - Level-up sound every 60 seconds
   - All sounds use 44.1kHz WAV format

### 🎯 **Game Constants**

| Parameter | Value | Purpose |
|-----------|-------|---------|
| Screen Width | 1600 px | Play area width |
| Screen Height | 850 px | Play area height |
| Target FPS | 60 | Smooth gameplay |
| Max Players | 1 | Single player game |
| Max Bullets | 10 | Projectile limit |
| Max Enemies | 10 | Enemy limit |
| Max Stars | 100 | Background stars |
| Base Enemy Speed | 2.0 units/frame | Starting difficulty |
| Level Up Interval | 60 seconds | Progression timer |
| Shield Duration | 5 seconds | Power-up duration |
| Shield Spawn Chance | 60% | Drop rate from kills |
| Player Movement Speed | 5.0 units/frame | Base movement speed |
| Bullet Speed | 5.0 units/frame | Upward velocity |
| Points Per Kill | 10 | Score reward |

---

## SOLID Principles Implementation

### **S - Single Responsibility Principle**
Each class has ONE reason to change:
- `Player` - Only if player mechanics change
- `Enemy` - Only if enemy AI changes
- `Bullet` - Only if bullet physics change
- `GameState` - Only if scoring/leveling changes
- `Renderer` - Only if texture management changes
- `AudioManager` - Only if sound management changes
- `Collision` - Only if collision algorithm changes

### **O - Open/Closed Principle**
Classes are open for extension, closed for modification:
- New enemy spawn patterns: extend `EnemyManager::respawn()`
- New level-up logic: modify only `GameState::tryLevelUp()`
- New collision shapes: add to `Collision` namespace
- Game loop unaffected by internal changes

### **L - Liskov Substitution Principle**
All classes honor their contracts:
- All `draw()` methods are const - never mutate state
- All `update()` methods modify state but don't draw
- All managers can be replaced without changing caller

### **I - Interface Segregation Principle**
Classes don't depend on what they don't need:
- `Player` doesn't know about `Enemy`
- `Bullet` doesn't know about `Collision` details
- `Renderer` is separate from `AudioManager`
- `Starfield` is independent

### **D - Dependency Inversion Principle**
High-level modules don't depend on low-level details:
- GameController depends on abstractions, not raw raylib
- `Collision` namespace abstracts raylib's CheckCollisionRecs()
- Audio playback abstracted as named methods
- Texture paths passed to constructors (injected dependencies)

---

## Build & Run

### Requirements
- C++17 or later
- raylib library (graphics and audio)
- CMake (if using CMake build) or MSVC (Visual Studio)

### Execution
The game initializes with `GameController` from `main.cpp`:
1. Creates game window (1600x850)
2. Loads all assets from "assets/" folder
3. Initializes all game entities
4. Runs main loop until window closes
5. Cleans up all resources via destructors

### Controls
- **LEFT/RIGHT/UP/DOWN** - Move player
- **SPACE** - Fire bullet
- **ESC** or close window - Exit game

---

## Architecture Decisions

### Why MVC?
- **Separation of Concerns**: Game logic, rendering, and input are independent
- **Testability**: Models can be tested without rendering
- **Maintainability**: Changes to one layer don't affect others
- **Scalability**: Easy to add new entities or visual effects

### Why C++ Classes Over C Structs?
- Encapsulation: Private data with public interfaces
- RAII: Automatic resource cleanup via constructors/destructors
- const-correctness: Enforce no state mutations during rendering
- Better organization: Methods stay with data

### Why Namespace for Collision?
- Groups related functions without forcing unnecessary objects
- Cleaner than global functions
- More organized than static methods
- Follows standard C++ patterns for utilities

---

# SpaceShooter — UML Class Diagram

```mermaid
classDiagram

    %% ───────────────────────────────
    %% Entry point
    %% ───────────────────────────────
    class main {
        +main() int
    }

    %% ───────────────────────────────
    %% GameController
    %% ───────────────────────────────
    class GameController {
        -SCREEN_W : int = 1600
        -SCREEN_H : int = 850
        -player : Player
        -bullets : BulletManager
        -enemies : EnemyManager
        -shield : ShieldPickup
        -stars : Starfield
        -gs : GameState
        -rend : Renderer
        -audio : AudioManager
        -shieldActive : bool
        +GameController()
        +run() void
        -processInput() void
        -update() void
        -handleCollisions() void
        -render() void
        -enemySpeed(level int) float
    }

    %% ───────────────────────────────
    %% Player
    %% ───────────────────────────────
    class Player {
        +pos : Vector2
        +speed : float
        +Player(screenWidth int, screenHeight int)
        +update(screenWidth int, screenHeight int) void
        +draw(texture Texture2D, scale float, shieldActive bool) void
    }

    %% ───────────────────────────────
    %% Bullet (data struct)
    %% ───────────────────────────────
    class Bullet {
        <<struct>>
        +pos : Vector2
        +active : bool
    }

    %% ───────────────────────────────
    %% BulletManager
    %% ───────────────────────────────
    class BulletManager {
        -bullets_ : array~Bullet, 10~
        +fire(playerPos Vector2, playerWidth float) void
        +update() void
        +draw() void
        +bullets() array~Bullet~&
    }

    %% ───────────────────────────────
    %% Enemy (data struct)
    %% ───────────────────────────────
    class Enemy {
        <<struct>>
        +pos : Vector2
        +active : bool
        +speed : float
    }

    %% ───────────────────────────────
    %% EnemyManager
    %% ───────────────────────────────
    class EnemyManager {
        -enemies_ : array~Enemy, 10~
        +init(speed float, screenWidth int) void
        +update(speed float, screenWidth int, screenHeight int, playerPos Vector2) void
        +respawn(enemy Enemy&, speed float, screenWidth int) void
        +draw(texture Texture2D, scale float) void
        +enemies() array~Enemy~&
    }

    %% ───────────────────────────────
    %% ShieldPickup
    %% ───────────────────────────────
    class ShieldPickup {
        +pos : Vector2
        +active : bool
        +expireTime : double
        +ShieldPickup()
        +spawn(killPos Vector2, currentTime double, durationSeconds double) void
        +update(currentTime double) void
        +draw() void
        +updatePlayerShield(shieldActive bool&, s ShieldPickup, currentTime double)$ void
    }

    %% ───────────────────────────────
    %% Starfield
    %% ───────────────────────────────
    class Star {
        <<struct>>
        +pos : Vector2
        +speed : float
    }

    class Starfield {
        -stars_ : array~Star, 100~
        +Starfield(screenWidth int, screenHeight int)
        +update(screenWidth int, screenHeight int) void
        +draw() void
    }

    %% ───────────────────────────────
    %% GameState
    %% ───────────────────────────────
    class GameState {
        +score : int
        +highScore : int
        +level : int
        +lastLevelUpTime : double
        +GameState(startTime double)
        +addScore(points int) void
        +tryLevelUp(currentTime double, intervalSeconds double) bool
        +reset(currentTime double) void
    }

    %% ───────────────────────────────
    %% Renderer
    %% ───────────────────────────────
    class Renderer {
        +playerTexture : Texture2D
        +enemyTexture : Texture2D
        +playerScale : float
        +enemyScale : float
        +Renderer(playerPath string, enemyPath string)
        +~Renderer()
    }

    %% ───────────────────────────────
    %% AudioManager
    %% ───────────────────────────────
    class AudioManager {
        -shoot_ : Sound
        -explosion_ : Sound
        -levelup_ : Sound
        +AudioManager(shootPath string, explosionPath string, levelupPath string)
        +~AudioManager()
        +playShoot() void
        +playExplosion() void
        +playLevelUp() void
    }

    %% ───────────────────────────────
    %% Collision namespace (free functions)
    %% ───────────────────────────────
    class Collision {
        <<namespace>>
        +check(a Rectangle, b Rectangle)$ bool
        +makeBulletRect(x float, y float)$ Rectangle
        +makePlayerRect(x float, y float, texW float, texH float, scale float)$ Rectangle
        +makeEnemyRect(x float, y float, texW float, texH float, scale float)$ Rectangle
        +makeShieldPickupRect(x float, y float)$ Rectangle
    }

    %% ───────────────────────────────
    %% Relationships
    %% ───────────────────────────────

    %% Entry point creates GameController
    main ..> GameController : creates

    %% GameController composes all model/view objects
    GameController *-- Player         : player
    GameController *-- BulletManager  : bullets
    GameController *-- EnemyManager   : enemies
    GameController *-- ShieldPickup   : shield
    GameController *-- Starfield      : stars
    GameController *-- GameState      : gs
    GameController *-- Renderer       : rend
    GameController *-- AudioManager   : audio

    %% GameController uses Collision namespace
    GameController ..> Collision : uses

    %% Managers own their data structs
    BulletManager *-- Bullet  : owns array of
    EnemyManager  *-- Enemy   : owns array of
    Starfield     *-- Star    : owns array of
```

---

### Relationship key

| Symbol | Meaning |
|--------|---------|
| `*--`  | Composition (owner holds member by value) |
| `..>`  | Dependency / uses |
| `$`    | Static method |
| `<<struct>>` | Plain data type (no behaviour) |
| `<<namespace>>` | Free-function grouping (no instances) |


*This MVC architecture provides a clean, extensible foundation for game development, maintaining clear boundaries between game logic, presentation, and control flow.*
