
# 🎮 PixelShatter

A modern **Breakout-style game** built with **C++17, OpenGL, and FreeGLUT**.  
Control the paddle with your mouse, launch the ball, break the bricks, and chase high scores!  
Includes smooth animations, keyboard shortcuts, and color customization menus.

---

## 📸 Screenshots

<img width="1366" height="683" alt="Screenshot (533)" src="https://github.com/user-attachments/assets/e0e49725-d160-4094-bfee-2d19d833be16" />

<img width="1366" height="696" alt="Screenshot (534)" src="https://github.com/user-attachments/assets/f059dc7d-45ce-46a2-87e6-1b98972ca191" />

---

## 🚀 Features
- Smooth paddle & ball mechanics  
- Dynamic brick layouts with increasing difficulty  
- Color customization via right-click menu  
- Keyboard + Mouse support  
- Animated menu system  
- Cross-platform (Windows / Linux / macOS)  

---

## 🖥️ Build & Run Instructions

### 🔹 Prerequisites
- **C++17 compiler** (g++, clang, or MSVC)  
- **FreeGLUT** development libraries  
- **OpenGL** libraries (usually included with your system)  

---

### 🔹 Windows

#### Option A — MSYS2 + MinGW (recommended)
1. Install [MSYS2](https://www.msys2.org/).  
2. Open **MSYS2 MinGW 64-bit** shell and install dependencies:
   ```bash
   pacman -Syu
   pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-freeglut
```

3. Compile:

   ```bash
   g++ -o PixelShatter PixelShatter.cpp -lfreeglut -lopengl32 -lglu32

   ```
4. Run:

   ```bash
   ./PixelShatter
   ```

   ⚠️ If `freeglut.dll` is missing, copy it from:

   ```
   C:\msys64\mingw64\bin\freeglut.dll
   ```

   into the same folder as `PixelShatter.cpp or exe file`.

#### Option B — Visual Studio + vcpkg

1. Install Visual Studio with **Desktop Development with C++**.
2. Install [vcpkg](https://github.com/microsoft/vcpkg):

   ```powershell
   git clone https://github.com/microsoft/vcpkg.git
   cd vcpkg
   .\bootstrap-vcpkg.bat
   .\vcpkg.exe install freeglut:x64-windows
   .\vcpkg.exe integrate install
   ```
3. Create a new **Console App** project, add `PixelShatter.cpp`.
4. Enable **C++17** and link:

   ```
   opengl32.lib
   glu32.lib
   freeglut.lib
   ```
5. Build & run inside Visual Studio.

---

### 🔹 Linux (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install build-essential freeglut3-dev libglu1-mesa-dev mesa-common-dev
g++ -std=c++17 PixelShatter.cpp -o PixelShatter -lGL -lGLU -lglut -lm
./PixelShatter
```

---

### 🔹 macOS

**Option A — Using Apple’s GLUT framework:**

```bash
clang++ -std=c++17 PixelShatter.cpp -o PixelShatter -framework OpenGL -framework GLUT
./PixelShatter
```

**Option B — Homebrew FreeGLUT (if frameworks fail):**

```bash
brew install freeglut
g++ -std=c++17 PixelShatter.cpp -o PixelShatter -I/usr/local/include -L/usr/local/lib -lglut -lGLU -lGL
./PixelShatter
```

---

## 🎮 Controls

### Main Menu

* `N` → New Game
* `Q` → Quit

### Game Over Screen

* `R` → Restart
* `M` → Return to Menu

### Info/Credits Screen

* `B` → Back to Menu

### In-Game

* **Mouse** → Move paddle
* `Space` → Launch ball
* **Left Click** → Launch ball
* **Right Click** → Open color customization menu
* `P` → Pause / Resume
* `Esc` → Return to main menu

---

## 🛠️ Customize

* Change window size → edit `WIDTH`, `HEIGHT`
* Change paddle/ball size → edit `paddleWidth`, `paddleHeight`, `ballRadius`
* Adjust difficulty → edit ball speed logic in `resetGame()`
* Brick layout → edit `ROWS`, `COLS`, and `resetBricks()`

---

## 🐞 Troubleshooting

* **`undefined reference to glutInit`** → link with `-lfreeglut` (Linux/Windows) or `-framework GLUT` (macOS).
* **`GL/freeglut.h not found`** → install freeglut development headers.
* **Missing DLL on Windows** → copy `freeglut.dll` to your `.exe` folder.
* **Black screen** → check that `glutMainLoop()` is called (already included).

---

## 📜 License

This project is open-source. You may modify and distribute it with attribution.

---

## 🤝 Contributors

* **Rakesh Biswas**
* **Nafis Ahamed**

---

```

---

