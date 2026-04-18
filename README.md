# fractals

A collection of fractal visualization and implemented in c++.

---

## Dragon Curve
The current implementation focuses on the **Dragon Curve** (also known as the Heighway Dragon), a recursive fractal constructed by repeatedly folding a line segment.
It includes various different forms of the curve: 
- Simple Dragon Curve,
- Area filled D-Curve,
- Boundary of the Dragon Curve and
- Snowflake-like fractal using four dragon curves(composite fractal)

---

### Requirements

- OpenGL
- GLUT (FreeGLUT) 
- GLFW → Only for the composite fractal (snowflake)
- C++ Compiler (g++, clang, or MSVC)

---

### Installation

#### Ubuntu / Linux
sudo apt update  
sudo apt install freeglut3-dev libglfw3-dev

---

#### Windows
- Install MinGW (or use MSVC)
- Install FreeGLUT for OpenGL 
- Download and set up GLFW 
- Configure include and library paths manually

---

#### macOS
brew install freeglut  
brew install glfw

---
