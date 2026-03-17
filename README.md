# 🧙‍♂️ Wizard Battle 2.0
A robust, terminal-based C++ combat engine featuring elemental magic, strategic AI, and a modular architecture.

## 📜 Overview
**Wizard Battle 2.0** is a complete architectural rewrite of the original 1.0 game that I worked on myself. It shifts from a basic procedural script to a professional **Namespace-driven engine** using **Object-Oriented Programming (OOP)** principles. Command unique wizards, manage spell charges, and duel a "smart" NPC in a turn-based tactical battle.

---

## 🚀 Key Upgrades (v1.0 vs v2.0)

| Feature | Version 1.0 | Version 2.0 |
| :--- | :--- | :--- |
| **Logic Structure** | Giant `main()` function | Modular `BattleEngine` Namespace |
| **Data Management** | Parallel Arrays / Structs | `std::map` with Enum categories |
| **NPC Intelligence** | Purely Random | Priority-based AI (Shields when low HP) |
| **Input Handling** | Prone to crashes/turn-skips | Input Validation "Gatekeeper" Loops |
| **Randomness** | `rand()` and `srand()` | Modern `<random>` (Mersenne Twister) |

---

## 🛠️ Game Features
* **Elemental System:** Choose between **Fire**, **Water**, and **Earth** wizards, each with unique strengths, weaknesses, and resistances.
* **Strategic Spell Categories:**
    * **Attack:** Deals direct damage modified by elemental types.
    * **Shield:** Reduces incoming damage for the next turn.
    * **Buff:** Doubles the damage of the next offensive spell.
* **Resource Management:** Every spell has limited "uses." Strategy is required to manage high-damage spells vs. utility moves.
* **Robust Input Shielding:** The engine prevents crashes from invalid text input (cin protection) and out-of-range spell selections.

---

## 📂 Project Structure
* `Classes.h / .cpp`: Defines the `Wizard` class, `SpellInfo` structures, and factory functions.
* `Functions.h / .cpp`: Contains the `BattleEngine` namespace logic (Combat math, AI, UI rendering).
* `main.cpp`: The game controller that manages the high-level battle sequence and game state.

---

## 🎮 How to Play

### 1. Requirements
* A C++ compiler (e.g., `g++` or `clang`).
* Support for C++11 or higher.

### 2. Compilation
Compile all project files together:
  `bash`
`g++ main.cpp Classes.cpp Functions.cpp -o WizardBattle`

### 3. Run the Game
`./WizardBattle` 

### 4. Gameplay Instructions
    
    Select a Caster: Choose your elemental wizard from the roster.

    Choose Spells: Type the number corresponding to the spell you wish to cast.

    Win Condition: Reduce the NPC's Health Points (HP) to 0 before they do the same to you!

## 🛡️ Future Roadmap

    [ ] Add "Heal" and "Status Effect" (Poison/Burn) spell types.

    [ ] Add a local multiplayer (2-Player) mode.

## 📄 License

This project is open-source and available for educational use.
