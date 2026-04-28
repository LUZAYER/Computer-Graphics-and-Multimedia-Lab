# OpenGL Triangle Color Control (GLFW + GLAD)

This project demonstrates **Modern OpenGL (3.3 Core Profile)** using **GLFW** and **GLAD** to render a single triangle with **interactive color control and animation** using shader uniforms.

It focuses on understanding the **OpenGL rendering pipeline**, **shader compilation**, **uniform variables**, and **real-time input handling**.

---

## Program Output

* **Window Title:** `0432320005101146`
* **Resolution:** 800 × 600
* **Background Color:** Dark Gray (`0.1, 0.1, 0.1`)
* **Rendered Object:** Single triangle
* **Default Behavior:** Animated color transition (cyan ↔ magenta)
* **Exit Key:** `I`

---

## What This Program Demonstrates

### 1️⃣ OpenGL Context Initialization

* GLFW initialization and configuration
* OpenGL 3.3 Core Profile setup
* GLAD function loader initialization
* Framebuffer resize callback handling

---

### 2️⃣ Shader Pipeline (GLSL)

#### Vertex Shader

* Passes vertex position directly to clip space:

```glsl
gl_Position = vec4(aPos, 1.0);
```

#### Fragment Shader

* Uses a **uniform variable** to control color dynamically:

```glsl
uniform vec4 ourColor;
FragColor = ourColor;
```

---

### 3️⃣ GPU Buffer System

* Vertex Array Object (VAO)
* Vertex Buffer Object (VBO)
* Attribute pointer configuration:

  * Location `0` → `vec3 position`
* Static triangle vertex data stored on GPU

---

### 4️⃣ Real-Time Rendering Loop

* Clears screen every frame
* Activates shader program
* Updates uniform color dynamically
* Draws triangle using:

```cpp
glDrawArrays(GL_TRIANGLES, 0, 3);
```

---

## Triangle Geometry

```
Top Vertex    : ( 0.0,  0.5)
Bottom Right  : ( 0.5, -0.5)
Bottom Left   : (-0.5, -0.5)
```

---

## Color Behavior Logic

### 🎨 Default Mode (Animated)

* Uses sine wave over time:
* Smooth transition between:

  * Cyan `(0, 1, 1)`
  * Magenta `(1, 0, 1)`

---

### ⌨️ Controls

| Key   | Action                                       |
| ----- | -------------------------------------------- |
| **W** | Hold → Triangle becomes **white**            |
| **R** | Press → Triangle becomes **permanently red** |
| **I** | Exit application                             |

---

### 🔴 Force Red Mode

If `R` is pressed:

```cpp
forceRed = true;
```

* Overrides all other color logic
* Triangle stays **red permanently**

---

## Shader Uniform Concept

This project demonstrates:

* CPU → GPU communication using **uniform variables**
* Dynamic runtime updates using:

```cpp
glUniform4f(location, r, g, b, a);
```

Uniforms are:

* Global per shader program
* Persistent until modified
* Ideal for real-time parameter control

---

## Rendering Pipeline Overview

1. Initialize GLFW + GLAD
2. Create shader program
3. Upload vertex data (VBO)
4. Configure VAO
5. Enter render loop
6. Handle input (W / R / I)
7. Update uniform color
8. Draw triangle

---

## Framebuffer Callback

Automatically adjusts viewport on resize:

```cpp
glViewport(0, 0, width, height);
```

---

## Project Purpose

This project is designed to practice:

* Modern OpenGL setup (Core Profile)
* Shader compilation & linking
* VAO / VBO management
* Uniform-based animation
* Real-time keyboard interaction
* GPU rendering pipeline fundamentals

---

## Requirements

* OpenGL 3.3+
* GLFW
* GLAD
* C++ compiler (g++, clang, MSVC)

---

## Build Notes (Generic)

Compile with:

```bash
g++ main.cpp glad.c -lglfw -ldl -lGL -o app
```

(Platform-dependent linking may vary)

---

## Author

**Reyazul Islam, ID- 0432320005101146**

Cybersecurity Engineer | CTF Player | Reverse Engineering & Web Penetration Enthusiast

---
