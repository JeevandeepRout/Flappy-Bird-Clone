# 🐦 Flappy Bird Clone (SFML)

A Flappy Bird-inspired 2D game developed in C++ using the SFML graphics library. Players control a bird that must navigate through moving pipes while avoiding collisions. The game features multiple difficulty levels, score tracking, high-score persistence, and a simple menu-driven interface.

---

## ✨ Features

* Three difficulty levels (Easy, Medium, Hard)
* Bird movement with gravity and jump mechanics
* Dynamic pipe generation
* Moving pipe gaps on higher difficulties
* Real-time score tracking
* High score persistence using file storage
* Collision detection system
* Pause and resume functionality
* Menu screen for difficulty selection
* Game Over screen with restart option

---

## 🛠️ Technologies Used

* C++
* SFML (Simple and Fast Multimedia Library)
* Object-Oriented Programming (OOP)
* File Handling
* Game Physics
* Event-Driven Programming

---

## 📁 Project Structure

```text id="9h9j5k"
FlappyBird/
│
├── assets/
│   ├── asset1.jpeg
│   └── asset2.jpeg
│
├── flappy.cpp
└── README.md
```

---

## 🚀 Getting Started

### Prerequisites

* C++ Compiler (GCC, MinGW, MSVC, etc.)
* SFML Library installed and configured

### Installation

1. Clone the repository:

```bash id="4blj1t"
git clone <repository-url>
```

2. Navigate to the project directory:

```bash id="jg14el"
cd FlappyBird
```

3. Compile the project:

```bash id="r8z7ef"
g++ flappy.cpp -o FlappyBird \
-lsfml-assets \
-lsfml-window \
-lsfml-system
```

4. Run the executable:

```bash id="v18zdu"
./FlappyBird
```

---

## 📸 Preview

The game starts with a menu screen where players can select a difficulty level. During gameplay, the bird must pass through gaps between moving pipes while avoiding collisions. The score increases with each successful pipe pass, and the highest score is saved for future sessions.

### Controls

| Key   | Action         |
| ----- | -------------- |
| Space | Flap / Jump    |
| P     | Pause / Resume |
| Enter | Start Game     |
| R     | Return to Menu |
| 1     | Easy Mode      |
| 2     | Medium Mode    |
| 3     | Hard Mode      |

---

## 🎯 Learning Outcomes

This project helps strengthen skills in:

* Game Loop Implementation
* Object-Oriented Programming in C++
* Event Handling and Keyboard Input
* Gravity and Movement Physics
* Collision Detection Techniques
* Texture, Sprite, and Font Management
* File Handling for Data Persistence
* Game State Management (Menu, Playing, Paused, Game Over)
* 2D Game Development with SFML

---

⭐ If you found this project useful, consider starring the repository!
