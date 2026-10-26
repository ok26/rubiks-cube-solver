# Rubik's Cube Solver

A 3D Rubik's cube (OpenGL) that can be turned by hand and solved with Kociemba's two-phase algorithm. An older Thistlethwaite solver is also still in the code.

## Building

Requirements:

- A C++17 compiler (on macOS: `xcode-select --install`)
- CMake 3.16 or newer (on macOS: `brew install cmake`)
- Git and an internet connection the first time you configure (GLFW and GLM are downloaded automatically)

```sh
cmake -S . -B build
cmake --build build --parallel
```

or simply `make`, which runs the same two commands. `make clean` removes the build directory.

The build defaults to `Release`, since the solver is very slow without optimizations. Use `-DCMAKE_BUILD_TYPE=Debug` (or `make BUILD_TYPE=Debug`) when you want to debug.

On Linux, GLFW additionally needs the X11 development packages (`libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl-dev` on Ubuntu).

## Running

The program loads everything in `resources/` relative to the directory it is started from. The build creates a `build/resources` link, so both of these work:

```sh
cd build && ./rubiks-cube-solver
./build/rubiks-cube-solver
```

`make run` builds and starts it. The solver also needs the pruning tables described in [`resources/data/README.md`](resources/data/README.md).

## Controls

| Key | Action |
| --- | --- |
| Mouse / scroll | Look around / zoom |
| W A S D, Space, Left Ctrl | Move the camera |
| R L U N F B | Turn the right, left, up, down, front, back face |
| Left Shift + face key | Turn the face counter-clockwise |
| Arrow keys | Rotate the whole cube |
| Enter | Fast 100-move scramble |
| F5 | Random scramble |
| F1 | Solve the cube |
| T | Cycle the solver time limit (10 ms to 30 s) |
| F2 | Benchmark: solve 1000 random cubes (blocks the window) |
| Y | Print the cube state to the terminal |
| Esc | Quit |

## Project layout

```
src/
  main.cpp                window, render loop and keyboard handling
  cube/                   the logical cube: state, moves, scrambling
  graphics/               OpenGL rendering: shader, camera, 3D cube, animation
  solver/                 search code shared by the solvers
    kociemba/             two-phase solver, coordinates, move and pruning tables
    thistlewaite/         four-phase solver and its pattern databases
  util/                   small helpers (combinatorics, progress bar)
external/                 third-party code kept in the repo (glad, stb_image)
resources/                shaders, textures, vertex data and pruning tables
```
