# AcidTrip
A real-time psychedelic visualizer built in C++ using SDL2, OpenGL 3.3, and GLSL shaders.
# AcidTrip 🌌

A real-time psychedelic visualizer built in C++ using SDL2, OpenGL 3.3, and GLSL shaders.
## 🎥 Demo

![AcidTrip Demo](Acid_Trip.gif)

This project renders GPU-based procedural visuals including liquid distortion,
kaleidoscope symmetry, color cycling, and starfield effects.
---

## ✨ Features

- Fullscreen OpenGL rendering
- Procedural noise (fbm + value noise)
- Kaleidoscope symmetry
- Chromatic aberration
- Scanline + flicker effects
- GPU-based color palette cycling
- Real-time interaction

---

## 🎮 Controls

| Key     | Action                          |
|---------|---------------------------------|
| W / S   | Increase / Decrease warp        |
| A / D   | Decrease / Increase speed       |
| Q / E   | Change kaleidoscope segments    |
| Space   | Freeze / Unfreeze time          |
| R       | Randomize color palette         |
| Esc     | Quit                            |

---
\
## ▶️ Run Locally

### macOS

```bash
brew install cmake sdl2
git clone https://github.com/sdg5-hub/AcidTrip.git
cd AcidTrip
cmake -S . -B build
cmake --build build -j
./build/AcidTrip

## 🛠 Build (macOS)

### Install dependencies
```bash
brew install cmake sdl2


