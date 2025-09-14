
# How to run / build / play — step-by-step

---

# 1) Prerequisites (applies to all platforms)

* You need the `Breakoutv0.cpp` file in a folder (call it `PixelShatter`).
* The project uses FreeGLUT (or system GLUT) + OpenGL. Install the development headers/libs for GLUT/freeglut.
* A C++ compiler (g++/clang on macOS & Linux, mingw or Visual Studio on Windows).
* Optional: an editor/IDE (VS Code, Visual Studio, CLion, etc.).

---

# 2) Build & Run on Linux (Ubuntu / Debian)

1. Open a terminal.
2. Install dependencies:

```bash
sudo apt update
sudo apt install build-essential freeglut3-dev libglu1-mesa-dev mesa-common-dev
```

3. Put `Breakoutv0.cpp` in a folder and `cd` to it.
4. Compile (recommended flags):

```bash
g++ -std=c++17 -O2 -Wall Breakoutv0.cpp -o PixelShatter -lGL -lGLU -lglut -lm
```

(If you prefer debug symbols: replace `-O2` with `-g`.)
5\. Run:

```bash
./PixelShatter
```

---

# 3) Build & Run on macOS

Two common routes:

**A. Using system GLUT frameworks (simple but may be deprecated on some macOS versions):**

```bash
clang++ -std=c++17 Breakoutv0.cpp -o PixelShatter -framework OpenGL -framework GLUT
./PixelShatter
```

**B. Using Homebrew + freeglut (recommended if frameworks fail):**

1. Install brew + freeglut:

```bash
brew install freeglut
```

2. Compile (adjust include/lib paths if brew installed elsewhere):

```bash
g++ -std=c++17 Breakoutv0.cpp -o PixelShatter -I/usr/local/include -L/usr/local/lib -lglut -lGLU -lGL
./PixelShatter
```

If the linker complains, check `brew --prefix freeglut` for correct include/lib paths.

---

# 4) Build & Run on Windows

## Option A — MSYS2 / MinGW (recommended if you like unix-like tools)

1. Install MSYS2 from msys2.org, then open **MSYS2 MinGW 64-bit** shell.
2. Update and install:

```bash
pacman -Syu            # may need to close and re-open shell after update
pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-freeglut
```

3. `cd` to your source folder (use the MinGW shell path).
4. Compile:

```bash
g++ -std=c++17 Breakoutv0.cpp -o PixelShatter.exe -lfreeglut -lopengl32 -lglu32 -lgdi32 -lwinmm
```

5. Run:

```bash
./PixelShatter.exe
```

If the program fails because a DLL is not found, copy the `freeglut*.dll` from `C:\msys64\mingw64\bin` next to your `.exe` or add that bin folder to PATH.

## Option B — Visual Studio (MSVC)

* Use vcpkg to install freeglut (easy integration):

```powershell
# in Powershell
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg.exe install freeglut:x64-windows
.\vcpkg.exe integrate install
```

* Create a new Visual C++ project, add `Breakoutv0.cpp`, set C++17, and Visual Studio will pick up freeglut via vcpkg. Link `opengl32.lib` and `glu32.lib` if needed.

---

# 5) Single-command quick run (Linux example)

If you want a one-liner to compile & run (Linux):

```bash
g++ -std=c++17 -O2 Breakoutv0.cpp -o PixelShatter -lGL -lGLU -lglut && ./PixelShatter
```

---

# 6) Gameplay / Controls (what each key & mouse does)

This matches the code in `Breakoutv0.cpp`:

**Main Menu**

* `N` = New Game
* `C` = Credits
* `Q` = Quit

**Game Over Screen**

* `R` = Restart
* `M` = Menu

**Credits Screen**

* `B` = Back to Menu

**In-Game / Global Controls**

* Mouse movement = Move paddle (passive motion)
* `Space` = Launch ball (also left click)
* Left mouse click = Launch ball (works while in PLAYING and ball not launched)
* Right mouse click = Opens color customization menu (GLUT popup menu)
* `P` = Pause / Resume
* `Esc` = Back to menu (appears in keyboard handler)

**HUD**

* Score, Lives, Level display at the top-left
* Pause shows overlay and text

---

# 7) How the UI maps to screen coordinates (brief)

* Window width/height constants: `WIDTH = 1200, HEIGHT = 800`. Mouse coordinates are converted to OpenGL −1..1 in `mouseMove()` and `handleClick()` so the clickable rectangles in screen coordinates are mapped manually in code. If you change `WIDTH`/`HEIGHT`, the mapping remains correct because code uses those constants.

---

# 8) Edit / customize the game (common tweaks)

* Change window size: edit `WIDTH` / `HEIGHT`.
* Change paddle/ball sizes: `paddleWidth`, `paddleHeight`, `ballRadius`.
* Add/remove bricks or change rows/cols: `ROWS`, `COLS`, `brickWidth`, `brickHeight`, and `resetBricks()`.
* Change colors: edit `colors` array (RGB floats).
* Add new keyboard shortcuts: edit `keyboard()` function.
* Change menu text or layout: edit `drawMenu()`, `drawGameOver()`, `drawCredits()`.

---

# 9) Quick debugging tips

* **“undefined reference to `glutInit`”** → you forgot to link `-lglut` / `-lfreeglut` / `-lopengl32`.
* **Header not found `GL/freeglut.h`** → install freeglut dev package or check include path.
* **On macOS: `GL/freeglut.h` missing** → use `#include <GLUT/glut.h>` and compile with frameworks OR install freeglut via Homebrew and fix include paths.
* If the window appears black: ensure `glutMainLoop()` is being called and `display()` registered.
* Add `-g` flag to compile for debugger; run `gdb ./PixelShatter` or run inside VS Code / Visual Studio.

---

# 10) Optional: minimal CMakeLists.txt

Drop this file next to `Breakoutv0.cpp` (may require adjusting the `find` depending on platform):

```cmake
cmake_minimum_required(VERSION 3.10)
project(PixelShatter)
set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_path(FREEGLUT_INCLUDE_DIR GL/freeglut.h)
find_library(FREEGLUT_LIBRARY NAMES freeglut glut)

include_directories(${FREEGLUT_INCLUDE_DIR})
add_executable(PixelShatter Breakoutv0.cpp)
target_link_libraries(PixelShatter ${FREEGLUT_LIBRARY} ${OPENGL_LIBRARIES})
```

Then:

```bash
mkdir build && cd build
cmake .. && cmake --build .
./PixelShatter
```

(You may need to install freeglut and point CMake if it can’t find it.)

---

# 11) Packaging / distributing

* On Linux/macOS: zip or tar the binary and include the `freeglut` .so/.dylib when needed (or instruct users to `apt install freeglut3`).
* On Windows: include `freeglut.dll` next to your `PixelShatter.exe`.

---

# 12) Quick VS Code tasks.json for compile & run (Linux example)

Create `.vscode/tasks.json`:

```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "build PixelShatter",
      "type": "shell",
      "command": "g++ -std=c++17 Breakoutv0.cpp -o PixelShatter -lGL -lGLU -lglut -lm",
      "group": { "kind": "build", "isDefault": true }
    },
    {
      "label": "run PixelShatter",
      "type": "shell",
      "command": "./PixelShatter",
      "dependsOn": "build PixelShatter"
    }
  ]
}
```

Use `Run Task` → `run PixelShatter`.

---





---

# 🎮 How to Build & Run PixelShatter on Windows

---

## 🔹 1. Prerequisites

* A **C++17 compiler** (either **MSYS2/MinGW** or **Visual Studio**).
* **FreeGLUT** (needed for OpenGL/GLUT functions).
* A text editor or IDE (VS Code, Visual Studio, CLion, etc.).

---

## 🔹 2. Option A — MSYS2 + MinGW (recommended)

This is the easiest way to get GNU tools + freeglut on Windows.

1. **Install MSYS2**

   * Download from: [https://www.msys2.org](https://www.msys2.org)
   * Install it and open the **MSYS2 MinGW 64-bit** shell.

2. **Update & install required packages**
   In the MSYS2 MinGW 64-bit terminal:

   ```bash
   pacman -Syu
   # Close terminal, reopen MSYS2 MinGW 64-bit, then run:
   pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-freeglut
   ```

3. **Navigate to your project folder**
   Example:

   ```bash
   cd /c/Users/YourName/Desktop/PixelShatter
   ```

4. **Compile the game**

   ```bash
   g++ -std=c++17 Breakoutv0.cpp -o PixelShatter.exe -lfreeglut -lopengl32 -lglu32 -lgdi32 -lwinmm
   ```

5. **Run the game**

   ```bash
   ./PixelShatter.exe
   ```

⚠️ If it complains about missing `freeglut.dll`, copy it from:

```
C:\msys64\mingw64\bin\freeglut.dll
```

into the same folder as your `PixelShatter.exe`.

---

## 🔹 3. Option B — Visual Studio (MSVC + vcpkg)

If you prefer Visual Studio:

1. **Install Visual Studio** with **Desktop Development with C++** workload.
2. **Install vcpkg** (dependency manager from Microsoft):

   ```powershell
   git clone https://github.com/microsoft/vcpkg.git
   cd vcpkg
   .\bootstrap-vcpkg.bat
   .\vcpkg.exe install freeglut:x64-windows
   .\vcpkg.exe integrate install
   ```
3. **Open Visual Studio**, create a new **Console C++ Project**, and add `Breakoutv0.cpp`.
4. **Enable C++17** in Project Settings → C/C++ → Language → C++ Language Standard.
5. **Link libraries**
   In Project Properties → Linker → Input → Additional Dependencies, add:

   ```
   opengl32.lib
   glu32.lib
   freeglut.lib
   ```
6. Build and run inside Visual Studio.

---

## 🔹 4. Controls & Gameplay

### Main Menu

* `N` = New Game
* `C` = Credits
* `Q` = Quit

### Game Over Screen

* `R` = Restart
* `M` = Back to Menu

### Credits Screen

* `B` = Back to Menu

### In-Game

* Mouse movement = Move paddle
* `Space` = Launch ball
* Left mouse click = Launch ball
* Right mouse click = Color customization menu
* `P` = Pause / Resume
* `Esc` = Return to main menu

---

## 🔹 5. Troubleshooting

* **Error: cannot find `GL/freeglut.h`** → You didn’t install freeglut. Use `pacman -S mingw-w64-x86_64-freeglut` in MSYS2 or `vcpkg install freeglut:x64-windows` in VS.
* **Missing DLLs when running** → Copy `freeglut.dll` from `C:\msys64\mingw64\bin` to your game folder.
* **Black screen** → Ensure you’re calling `glutMainLoop()` (already in `Breakoutv0.cpp`).

---

✅ With this setup, you can build and run PixelShatter smoothly on **Windows 10/11**.

---
