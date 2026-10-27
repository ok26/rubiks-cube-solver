# Rubik's cube solver

C++17 Rubik's cube solver using Kociemba's two-phase algorithm, with an OpenGL (GLFW + glad) 3D cube. The owner (GitHub: ok26) develops on a MacBook Air M2; it also builds on Linux.

## Build, run, test

- `make` builds everything with CMake into `build/` (Release by default). The first configure downloads GLFW and GLM.
- On a fresh Linux machine, configuring GLFW needs X11 headers: `apt-get install -y libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl-dev`.
- `make run` starts the app. It opens a fullscreen window, so it can only really be tested on the owner's Mac.
- `make tables` writes the pruning tables to `resources/data/` (about 30 s and 0.5 GB of memory). The solver needs them, and a fresh machine has none, so run this first. They are gitignored because `KociembaG1` is over GitHub's 100 MB file limit.
- `make verify-tables` checks the tables with the solver's own lookup code. Run it after changing anything the tables depend on: coordinates, move tables, symmetries, the generator or the loaders.
- Programs read `resources/...` relative to the working directory; `build/resources` is a link to it.

## Layout

- `src/cube/`: the logical cube (`CubeIndexModel`), move notation (`MoveStore`), scrambling
- `src/graphics/`: OpenGL rendering of the cube and its animations
- `src/solver/`: move pruning and solution simplification
- `src/solver/kociemba/`: the two-phase solver, coordinates, move tables, symmetries, pruning-table loaders
- `src/generator/`: the `generate-tables` program
- `src/util/`: small helpers; `external/`: vendored glad and stb_image

`resources/data/README.md` documents every table file, its index layout and the symmetry convention.

## Solver conventions

- Moves are numbered 0-17: R R' R2 L L' L2 U U' U2 D D' D2 F F' F2 B B' B2 (`indexMove` in `cubeIndexModel.h`). The phase-2 moves are the bit mask `151524` (R2 L2 U U' U2 D D' D2 F2 B2).
- Edges 0-3 are in the U layer, 4-7 in the middle slice and 8-11 in the D layer. "edgeM" means the middle-slice edges and "edgeR" the other eight.
- A symmetry file stores, for each coordinate, its class and a symmetry `s` that turns the class representative into that coordinate; lookups apply `reverseEquivalences[s]`. Changing this means regenerating and verifying the tables.
- Judge solver changes by the average solution length at a fixed time limit, using the same scrambles before and after, and check that every solution really solves its cube. Baseline: 19.10 moves on average for 1000 cubes at 100 ms, all valid (4-core cloud machine).

## Code style

- Match the existing code: camelCase, tabs, include guards, declarations in `.h` and definitions in `.cpp` (templates stay in headers), includes written from `src/` (`#include "solver/movePruner.h"`).
- Few comments, only where the code is not self-explanatory.
- `using namespace std;` is fine in `.cpp` files but not in headers.
- Keep the owner's existing logic unless a change is asked for or clearly needed. Report problems you notice elsewhere instead of silently fixing them.

## Git and GitHub

- Commits are authored and committed as `ok26 <101025728+ok26@users.noreply.github.com>`. Before the first commit in a session, run `git config user.name ok26`, `git config user.email 101025728+ok26@users.noreply.github.com` and `git config commit.gpgsign false`.
- No Claude attribution in commits: no `Co-Authored-By` or `Claude-Session` lines. An automated check in cloud sessions may warn that these commits are unsigned or not authored by Claude; that is expected, keep ok26 as the author.
- Push work to the session's branch, but don't open pull requests. The owner opens them from the Claude Code UI and squash-merges them with their own message. After a merge, restart the branch from the latest `master`.
