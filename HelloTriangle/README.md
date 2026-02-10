# OpenGL Triangle Rendering (GLFW + GLAD)

This project demonstrates **modern OpenGL (3.3 Core Profile)** using **GLFW** and **GLAD** to render **two cyan triangles** on an **orange background** using **vertex and fragment shaders**.

The program is part of the **Computer Graphics and Multimedia Lab** and focuses on:

* OpenGL context creation
* Shader compilation & linking
* VAO/VBO configuration
* Rendering using `glDrawArrays`

---

## Output Description

* **Window Title:** Reyazul Islam
* **Resolution:** 800 × 600
* **Background Color:** Orange
* **Triangle Color:** Cyan
* **Rendering Mode:** Filled triangles
* **Exit Key:** `R`

---

## Features Demonstrated

* OpenGL **3.3 Core Profile**
* GLSL **Vertex & Fragment Shaders**
* Shader error checking
* VAO & VBO usage
* Multiple triangles using `GL_TRIANGLES`
* Keyboard input handling
* Viewport resizing callback

---

## Shader Overview

### Vertex Shader

* Accepts vertex positions via `layout(location = 0)`
* Converts 3D coordinates to clip space

### Fragment Shader

* Outputs a **cyan color** using `FragColor`

---

## Controls

| Key | Action           |
| --: | ---------------- |
|   R | Close the window |

---

## Project Structure (Reference)

> **Note:**
> Only `main.cpp` is uploaded to GitHub.
> The following structure is the **expected local project layout**, based on:
> [https://github.com/LUZAYER/Computer-Graphics-and-Multimedia-Lab/tree/main/HelloWindowClearColored](https://github.com/LUZAYER/Computer-Graphics-and-Multimedia-Lab/tree/main/HelloWindowClearColored)

```
HelloWindowClearColored/
├── Makefile
├── ReadMe.md
│
├── build/
│   ├── main          # Linux executable
│   ├── main.exe      # Windows executable
│   └── glfw3.dll     # GLFW runtime (Windows)
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
    └── main.cpp      # (Uploaded to GitHub)
```

---

## Build Instructions

### Linux

#### Dependencies

```bash
sudo apt update
sudo apt install -y libglfw3-dev libgl1-mesa-dev pkg-config
```

#### Build & Run

```bash
make linux
./build/main
```

---

### Windows (MSYS2 + MinGW)

1. Download **GLFW 64-bit Windows binaries**
   [https://www.glfw.org/download.html](https://www.glfw.org/download.html)

2. Copy `glfw3.dll` into:

   * `build/`
   * `lib/`

3. Install MSYS2
   [https://www.msys2.org/](https://www.msys2.org/)

4. Open **MSYS2 MINGW64** terminal:

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

## OpenGL Version Check

### Windows

* Run `dxdiag` → Display tab
* Or use **GPU-Z**
* Minimum required: **OpenGL 3.3**

### Linux / macOS

```bash
glxinfo | grep "OpenGL renderer"
```

---

## Student Tasks (Practice)

1. Render an **upside-down green triangle** on a **black background**
2. Render a **right-angled magenta triangle** on a **white background**

---

## Learning Outcomes

After completing this program, you should understand:

* Modern OpenGL initialization
* Shader pipeline basics
* GPU buffer management
* Attribute specification
* Basic real-time rendering loop

---

## Author

**Reyazul Islam, ID- 0432320005101146**

Cybersecurity Engineer | CTF Player | Reverse Engineering & Web Penetration Enthusiast

---
