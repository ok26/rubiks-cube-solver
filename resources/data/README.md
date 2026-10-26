# Pruning tables

The solvers read their pruning tables from this folder at startup. They are not in git: `KociembaG1` alone is about 141 MB, which is over GitHub's 100 MB file limit. Everything in this folder except this README is ignored by `.gitignore`.

If a file is missing, the program still starts (the loaders do not check whether the file opened), but the solver will not produce correct solutions.

All tables are loaded on startup. Each file is a single line of characters. A distance is stored as `'a' + distance`. A symmetry index is stored as `'A' + symmetry` (0-15). A class index is stored as little-endian base 24, using `'a'` to `'x'` as the digits.

| File | Length | Contents |
| --- | --- | --- |
| `KociembaG1Symmetry` | 1,013,760 entries | For each flipUDSlice coordinate (`edgeOri + UDSlice * 2048`): a 4-character class index followed by one or more symmetry characters (only the first is used) |
| `KociembaG1` | 140,908,410 chars | Phase 1 distance, indexed by `cornerOri + classIndex * 2187` |
| `KociembaG2Symmetry` | 40,320 entries | For each corner permutation: a 3-character class index followed by one or more symmetry characters |
| `KociembaG2Corner` | 40,320 chars | Phase 2 distance of the corner permutation alone |
| `KociembaG2Edge` | 967,680 chars | Phase 2 distance, indexed by `edgeRPerm + edgeMPerm * 40320` |
| `KociembaG2Cubie` | 111,605,760 chars | Phase 2 distance, indexed by `edgeRPermSym + classIndex * 40320` |
