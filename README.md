# Tetris game.

A **Tetris** game made in the **C/C++** programming languages, using **Raylib** for the graphical interface.

## Features
- Graphical interface developed with Raylib.
- Game logic implemented in C/C++.
- Several difficulties.
- Music support.
- Save best scores.

Best scores are stored locally.

## Requirements
- Operating system: Windows.
- w64devkit compiler (`g++.exe`).
- Raylib pre-installed.

>This project assumes that Raylib is already correctly installed and configured on the system.

# Project Structure

```
Tetris/
├── assets/
│   ├── temporal_tetris_theme.ogg
│   └── tetris_background.png
├── include/
│   ├── bag_random.hpp
│   ├── board.hpp 
│   ├── core.hpp
│   ├── current_piece.hpp
│   ├── lines.hpp
│   ├── pieces.hpp
│   └── render.hpp
├── src/
│   ├── bag_random.cpp # 7-bag system.
│   ├── board.cpp # Board actions.
│   ├── core.cpp # Game logic.
│   ├── current_piece.cpp # Movement of the pieces.
│   ├── lines.cpp # Handling lines disposal.
│   ├── pieces.cpp # Definition of the pieces.
│   ├── render.cpp # Graphics using Raylib.
│   └── tetris.cpp # Gmae loop.
├── README.md
└── .gitignore
```

## Clone, Build and Run
Clone the repository:
```bash
git clone https://github.com/Penpinch/Tetris.git

cd Tetris
```

Compile the project:
```bash
g++ src\tetris.cpp src\core.cpp src\bag_random.cpp src\board.cpp src\current_piece.cpp src\lines.cpp src\pieces.cpp src\render.cpp -I.\include\ -I C:\raylib\raylib\src -L C:\raylib\raylib\src -lraylib -lopengl32 -lgdi32 -lwinmm -o tetris.exe
```

Run the executable:
```bash
.\tetris.exe
```

>Replace `C:\raylib\raylib\src` with the path where Raylib is installed on your system.

## Technologies Used
- C/C++ programming languages.
- Raylib.
- Git and GitHub.

## Contributors
- @Penpinch
- @JET-2707