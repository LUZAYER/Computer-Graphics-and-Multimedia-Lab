# HelloWindowClearColored

A minimal **OpenGL 3.3 Core Profile** application using **GLFW** and **GLAD** that opens a window and clears it with a custom background color.
This project is intended as a **Day 1 OpenGL setup** for learning and experimentation on **Linux** and **Windows**.

---

## Preview

* Window title: **Reyazul Islam**
* Resolution: **800 × 600**
* Background color: **Cyan (RGBA: 0.0, 1.0, 1.0, 0.7)**
* Press **R** to close the window

---

## Project Structure

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
│   ├── glad.h        # OpenGL loader header
│   └── glfw3.h       # GLFW header
│
├── lib/
│   └── glfw3.dll     # GLFW DLL (Windows)
│
└── src/
    ├── glad.c        # GLAD loader implementation
    └── main.cpp     # OpenGL application entry point
```

---

## Requirements

### Common

* OpenGL **3.3 or higher**
* C++ compiler with C++11 support
* GLFW
* GLAD

---

## Build Instructions

## 1. Linux

### Dependencies

```bash
sudo apt update
sudo apt install -y libglfw3-dev libgl1-mesa-dev pkg-config
```

### Build & Run

```bash
make linux
./build/main
```

---

## 2. Windows (MinGW via MSYS2)

### Setup

1. Download **GLFW 64-bit Windows binaries**
   [https://www.glfw.org/download.html](https://www.glfw.org/download.html)

2. From the extracted archive:

   * Copy `glfw3.dll` into:

     * `build/`
     * `lib/`
   * Ensure `libglfw3.a` is present in `lib/`

3. Install **MSYS2**
   [https://www.msys2.org/](https://www.msys2.org/)

4. Open **MSYS2 MINGW64** terminal and install tools:

```bash
pacman -S --needed base-devel mingw-w64-x86_64-gcc
```

5. Add MSYS2 `bin` directory to **PATH**:

```
C:\msys64\mingw64\bin
```

### Build & Run

```bash
make win
```

* Output binary: `build/main.exe`
* If Wine is installed, the program will auto-run on Linux.

---

## Controls

| Key | Action           |
| --: | ---------------- |
|   R | Close the window |

---

## Notes

* This project uses **OpenGL Core Profile**, so legacy OpenGL functions are not available.
* On Windows, if the program fails to start:

  * Check your OpenGL version using **GLView**
    [http://www.realtech-vr.com/home/glview](http://www.realtech-vr.com/home/glview)
  * OpenGL version must be **≥ 3.3**
* On Linux, Mesa drivers must support OpenGL 3.3+

---

## Learning Goals

* GLFW window creation
* OpenGL context initialization
* GLAD loader usage
* Render loop basics
* Keyboard input handling
* Framebuffer resize handling

---

## Author

**Reyazul Islam, ID- 0432320005101146**

Cybersecurity Engineer | CTF Player | Reverse Engineering & Web Penetration Enthusiast

---
