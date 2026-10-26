# Pruning tables

The solver reads its pruning tables from this folder at startup. They are not in git: `KociembaG1` alone is about 141 MB, which is over GitHub's 100 MB file limit. Everything in this folder except this README is ignored by `.gitignore`.

If a file is missing, the program still starts (the loaders do not check whether the file opened), but the solver will not produce correct solutions.

## Generating them

Run `make tables` from the project root, or `./generate-tables` from the build directory. It writes all six files into `resources/data/` and needs about 500 MB of memory. `generate-tables phase1` or `generate-tables phase2` generates only those files.

`make verify-tables` (or `./generate-tables verify`) loads the files with the solver's own code and checks them: the goal must have distance 0, every other position must have a neighbour exactly one move closer, and symmetric positions must have the same distance.

## Format

Each file is a single line of characters. A distance is stored as `'a' + distance`. A symmetry index is stored as `'A' + symmetry` (0-15). A class index is stored as little-endian base 24, using `'a'` to `'x'` as the digits.

| File | Length | Contents |
| --- | --- | --- |
| `KociembaG1Symmetry` | 1,013,760 entries | For each flipUDSlice coordinate (`edgeOri + UDSlice * 2048`): a 4-character class index and a symmetry character |
| `KociembaG1` | 140,908,410 chars | Phase 1 distance, indexed by `cornerOri + classIndex * 2187` |
| `KociembaG2Symmetry` | 40,320 entries | For each corner permutation: a 3-character class index and a symmetry character |
| `KociembaG2Corner` | 40,320 chars | Phase 2 distance of the corner permutation alone |
| `KociembaG2Edge` | 967,680 chars | Phase 2 distance, indexed by `edgeRPerm + edgeMPerm * 40320` |
| `KociembaG2Cubie` | 111,605,760 chars | Phase 2 distance, indexed by `edgeRPermSym + classIndex * 40320` |

## Symmetry classes

Coordinates that one of the 16 symmetries turns into each other belong to the same class, and the smallest coordinate in a class is its representative. The symmetry stored for a coordinate is a symmetry that turns the representative into that coordinate. The solver applies `reverseEquivalences` of it to the other coordinate (corner orientation or edgeRPerm), which turns the whole position into one whose first coordinate is the representative, and looks that up.

A class has fewer than 16 members when some symmetries turn its representative into itself. Then several symmetries lead to the same coordinate and the file stores the lowest of them; the reader also accepts more than one symmetry character and uses the first. The distance tables stay correct no matter which one is stored, because the generator gives every entry in the representative's row that those symmetries map onto each other the same distance.
