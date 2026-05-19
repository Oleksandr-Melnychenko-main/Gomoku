This repository contains a console-based Gomoku (Five in a Row) game written in C++.
It features both PvP and PvE modes with a smart bot, as well as a save/load system.
The project was developed as the final coursework for my first year of studying Computer Science at Kyiv Polytechnic Institute (KPI).


## Features

### 🎮 Gameplay Modes
* **Player vs Player (PvP):** Play locally with a friend on the same machine, taking turns to place marks (`X` and `O`).
* **Player vs Environment (PvE):** Challenge a smart automated bot that adapts to your moves.

### 🤖 Intelligent AI (Smart Bot)
* Driven by a custom **heuristic evaluation algorithm** that scores the board dynamically.
* **Dual-purpose evaluation:** The bot simultaneously calculates its best offensive options and detects enemy patterns to block them.
* **Fallback mechanism:** Utilizes a randomized move selector if no immediate tactical threats or advantages are present on the board.

### 💾 Save & Load System
* **Real-time Autosave:** The game automatically saves the board state, current turn, and game mode to a local text file (`savegame.txt`) after every single move.
* **Session Persistence:** Features a seamless "Continue" option in the main menu that restores the exact state of your last game, handling dynamic board resizing on the fly.
* **Corrupt File Prevention:** Validates the save file profile before loading to ensure the integrity of the data.

### ⚙️ Customization & UI
* **Dynamic Grid Scaling:** Fully adjustable board sizes ranging from `10x10` up to a massive `70x70` grid (Default is `15x15`).
* **Interactive CLI Interface:** A clean, scannable console menu complete with detailed rule explanations and project credits.
* **ANSI Color Styling:** Uses visual formatting (bolding, italics, and colors) directly in the terminal to distinguish players, highlights, and system errors.
