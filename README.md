# DashForge 3D

A 3D obstacle-runner game built in C11 with OpenGL 3.3 Core.

> **Queres probarlo?** Descargá la carpeta [`dist/`](dist/) y ejecutá `dashforge3d.exe`. No requiere instalación.

## Features

- Five official levels (Tutorial → Extreme)
- Full level editor with save/load and custom levels
- Map sharing via `.dash` text files
- Progress bar HUD and three camera modes (side, third-person, orbit)
- Particles and skins
- Pause menu, level-complete menu, how-to-play screen
- Debug and Release builds (Release: no console, clean window title)

## Tech stack

| Layer | Library |
|---|---|
| Window / input | GLFW 3.4 |
| OpenGL loader | GLAD (OpenGL 3.3 Core, vendored) |
| Math | cglm 0.9.4 (header-only) |
| Image loading | stb_image (header-only) |
| Font rendering | stb_easy_font (header-only) |
| Build system | CMake 3.20+ / Ninja |
| Compiler | GCC (MinGW-w64 UCRT64 on Windows) |

## Building

### Requirements

- CMake 3.20+
- GCC (MinGW-w64 UCRT64 recommended on Windows)
- Git (to fetch dependencies via CMake FetchContent)
- Ninja (optional but recommended)

### Windows (MSYS2 UCRT64)

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja git
```

**Debug**:
```bash
cmake -S . -B build/debug -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build/debug
./build/debug/dashforge3d.exe
```

**Release**:
```bash
cmake -S . -B build/release -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build/release
./build/release/dashforge3d.exe
```

### Linux / macOS

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/dashforge3d
```

## Controls

### Gameplay

| Key | Action |
|---|---|
| `Space` | Jump |
| `R` | Restart level |
| `Esc` | Pause |
| `F1` / `F2` / `F3` | Camera: Side / Third-person / Orbit |

### Level Editor

> The editor is only available on custom levels. Create one from **Select Level → + NEW LEVEL**.

| Key | Action |
|---|---|
| `Tab` | Toggle editor / test play |
| `Esc` | Back to main menu |
| `1` | Place Block (solid) |
| `2` | Place Spike (lethal) |
| `3` | Place Decoration (visual only, behind track) |
| `4` | Place Start marker (one per map) |
| `5` | Place End marker (one per map) |
| Left / Right | Cycle decoration color (when Deco selected) |
| Left click | Place object |
| Right click | Erase object |
| Scroll wheel | Pan camera horizontally |
| Shift + Scroll | Pan camera vertically |
| Middle drag | Pan camera freely |
| `Ctrl+S` | Save level |

## Sharing maps

1. In **Select Level**, click **OPEN FOLDER** — this opens `assets/levels/custom/` in Explorer.
2. To **export**: copy your `.dash` file from that folder and send it.
3. To **import**: paste a received `.dash` file into that folder, then click **OPEN FOLDER** (which also rescans) or restart the game.

## Level file format

Levels are plain-text `.dash` files in `assets/levels/`:

```
# DashForge 3D level format v1
level_name My Level
start 0.0000 1.0000 0.0000
end   50.0000 1.0000 0.0000

# object <type> <x> <y> <z> <sx> <sy> <sz> <rotY> [color_idx]
object block 0.0000 0.0000 0.0000 1.0000 1.0000 1.0000 0.0000
object spike 5.0000 0.5000 0.0000 1.0000 1.0000 1.0000 0.0000
object decoration 10.0000 2.0000 -2.0000 1.0000 1.0000 1.0000 0.0000 3
object end   50.0000 1.0000 0.0000 1.0000 1.0000 1.0000 0.0000
```

Object types: `block`, `spike`, `decoration`, `start`, `end`  
`color_idx` (0–7) is only used by `decoration`.
