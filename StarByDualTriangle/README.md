---

# OpenGL Dual Triangle Rendering (GLFW + GLAD)

This project demonstrates **Modern OpenGL (3.3 Core Profile)** using **GLFW** and **GLAD** to render:

* One **upward triangle**
* One **downward triangle**
* On an **orange background**
* Using programmable **Vertex and Fragment Shaders**

This exercise focuses on understanding the **OpenGL rendering pipeline**, shader compilation, VAO/VBO configuration, and drawing multiple primitives using `glDrawArrays`.

---

## Program Output

* **Window Title:** Reyazul Islam
* **Resolution:** 800 × 600
* **Background Color:** Orange
* **Triangle Color:** Cyan
* **Primitives:** Two triangles (6 vertices total)
* **Exit Key:** `R`

---

## What This Program Demonstrates

### 1️⃣ OpenGL Context Initialization

* GLFW initialization
* OpenGL 3.3 Core Profile configuration
* GLAD loader setup

### 2️⃣ Shader Pipeline

* Vertex Shader creation
* Fragment Shader creation
* Shader compilation & error checking
* Shader program linking
* Program validation

### 3️⃣ GPU Buffer Management

* Vertex Buffer Object (VBO)
* Vertex Array Object (VAO)
* Vertex attribute configuration
* Static draw buffer usage

### 4️⃣ Rendering

* Background clearing with `glClearColor`
* Drawing 6 vertices using:

```cpp
glDrawArrays(GL_TRIANGLES, 0, 6);
```

---

## Geometry Layout

### Upward Triangle

```
(-0.6, -0.45)
( 0.6, -0.45)
( 0.0,  0.9)
```

### Downward Triangle

```
(-0.6,  0.45)
( 0.6,  0.45)
( 0.0, -0.9)
```

Both triangles are rendered in **cyan** using the fragment shader.

---

## Controls

| Key | Action           |
| --: | ---------------- |
|   R | Close the window |

---

## Shader Summary

### Vertex Shader

* Input: `vec3 aPos`
* Output: Clip-space position
* GLSL Version: `#version 330 core`

### Fragment Shader

* Output Color:

```glsl
vec4(0.0, 1.0, 1.0, 1.0)  // Cyan
```

---

## Expected Local Project Structure

> Only `main.cpp` is uploaded to GitHub.
> The full working structure locally should be:

```
HelloWindowClearColored/
├── Makefile
├── ReadMe.md
│
├── build/
│   ├── main
│   ├── main.exe
│   └── glfw3.dll
│
├── include/
│   ├── glad.h
│   └── glfw3.h
│
├── lib/
│   └── glfw3.dll
│
└── src/
    ├── glad.c
    └── main.cpp   (Uploaded)
```

---

## Build Instructions

### Linux

Install dependencies:

```bash
sudo apt update
sudo apt install -y libglfw3-dev libgl1-mesa-dev pkg-config
```

Build:

```bash
make linux
./build/main
```

---

### Windows (MSYS2 + MinGW)

1. Download GLFW 64-bit binaries
   [https://www.glfw.org/download.html](https://www.glfw.org/download.html)

2. Copy `glfw3.dll` to:

   * `build/`
   * `lib/`

3. Install MSYS2
   [https://www.msys2.org/](https://www.msys2.org/)

4. In MSYS2 MINGW64 terminal:

```bash
pacman -S --needed base-devel mingw-w64-x86_64-gcc
```

5. Add to PATH:

```
C:\msys64\mingw64\bin
```

6. Build:

```bash
make win
```

---

## OpenGL Version Requirement

Minimum required: **OpenGL 3.3**

### Check on Windows

* `Win + R → dxdiag`
* Or use GPU-Z

### Check on Linux/macOS

```bash
glxinfo | grep "OpenGL renderer"
```

---

## Author

**Reyazul Islam, ID- 0432320005101146**

Cybersecurity Engineer | CTF Player | Reverse Engineering & Web Penetration Enthusiast

---
