# 🚀 Space Shooter Game (MVC Architecture)

## 📌 Executive Summary
This project is a 2D Space Shooter Game developed in C++ using the raylib library.  
The system has been structured using the **Model-View-Controller (MVC)** design pattern to ensure modularity, maintainability  and clear separation of concerns.

- Game logic is handled in the **Model**
- Rendering and audio are handled in the **View**
- Input handling and game flow are managed by the **Controller**

---

## 🎯 Objectives

- Apply **MVC architecture** in a real-time game project
- Separate game logic from rendering and input handling
- Improve code organization and scalability
- Build a fully functional and interactive game

---

## 🧱 Architecture Overview
        +-------------------+
        |     Controller    |
        | (GameController)  |
        +---------+---------+
                  |
    +-------------+-------------+
    |                           |

+-------v--------+ +-------v--------+
| Model | | View |
| (Game Logic) | | (Rendering/UI) |
+----------------+ +----------------+




---

## 🧠 Model (Game Logic)

The Model represents the core game data and logic.

### Responsibilities:
- Maintain game state
- Handle player movement and actions
- Manage enemies, bullets, and collisions
- Implement scoring and game rules

### Key Components:
- `Player`
- `EnemyManager`
- `BulletManager`
- `GameState`
- `Collision`
- `ShieldPickup`

---

## 🖥️ View (Rendering Layer)

The View is responsible for displaying the game to the player.

### Responsibilities:
- Render graphics (player, enemies, bullets)
- Display UI elements (score, game over screen)
- Handle audio playback

### Key Components:
- `Renderer`
- `AudioManager`

---

## 🎮 Controller (Game Flow)

The Controller connects Model and View and manages the overall game flow.

### Responsibilities:
- Handle user input (keyboard)
- Update game state
- Control game loop
- Coordinate between Model and View

### Key Component:
- `GameController`

---

## 🔄 Game Flow

1. Initialize window and game resources  
2. Controller starts the game loop  
3. User input is captured  
4. Controller updates Model  
5. View renders updated game state  
6. Loop continues until game ends  

---

## ⚙️ How to Run the Project

### 🧾 Requirements:
- C++ compiler (g++)
- raylib library installed

---


### ▶️ Compile:

```bash
g++ *.cpp -o game -lraylib -lopengl32 -lgdi32 -lwinmm


▶️ Run:
./Debug/SpaceShooter.exe 


                                            SpaceShooter/
                                            │
                                            ├── main.cpp
                                            ├── GameController.cpp
                                            │
                                            ├── Model/
                                            │   ├── Player.cpp
                                            │   ├── EnemyManager.cpp
                                            │   ├── BulletManager.cpp
                                            │   ├── GameState.cpp
                                            │   └── Collision.cpp
                                            │
                                            ├── View/
                                            │   ├── Renderer.cpp
                                            │   └── AudioManager.cpp
                                            │
                                            ├── assets/
                                            │   ├── textures/
                                            │   ├── sounds/
                                            │   └── fonts/




⚠️ Known Limitations :
  Some rendering functions are implemented inside model classes (practical simplification)
  Input handling partially exists in Player (can be further improved for strict MVC)
  Not fully optimized for large-scale extension


🔮 Future Improvements :
  Move all input handling fully into Controller
  Move all rendering strictly into View
  Add levels and difficulty scaling
  Implement menu system (Start / Pause / Exit)
  Improve collision detection efficiency


🧪 Testing :
  Game tested for:
  Player movement
  Shooting mechanics
  Enemy spawning
  Collision detection
  Game over condition








