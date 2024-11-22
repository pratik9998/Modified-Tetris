# Modified Tetris Game

A modern twist to the classic Tetris game, implemented in C++ using OpenGL and GLUT. This game features enhanced visuals, customizable controls, and added functionalities like score tracking and advanced figure manipulations.

---

## Features
- **Dynamic Figures**: Seven unique Tetris shapes with vibrant colors.
- **Score System**: Tracks and displays the player's score.
- **Customizable Gameplay**: Control the pieces with keyboard inputs for movement, rotation, and dropping.
- **Smooth Rendering**: Designed with OpenGL for real-time graphics.
- **Advanced Mechanics**: Includes full-column clearing logic to increase gameplay complexity.
- **Game Over Detection**: Ensures seamless end-of-game feedback.

---

## Controls
- **`u`**: Move figure up.
- **`d`**: Move figure down.
- **`a`**: Drop the figure to the bottom.
- **`Space`**: Rotate the figure.
- **`ESC`**: Exit the game.

---

## Compilation and Execution
1. Install required libraries: `GL`, `GLU`, and `glut`.
2. Compile the code:
   ```bash
   g++ tetris.cpp -lGL -lGLU -lglut
