# Rubik's Cube Solver

A C++20 implementation of a Rubik's Cube solver featuring **three interchangeable cube representations**, **four search algorithms**, and a **corner pattern database** for heuristic-guided solving. The project is built around a polymorphic `RubiksCube` base class, allowing every solver and pattern database to work with any representation via templates.

All results below were independently benchmarked — see [Verified Results](#verified-results) for real numbers, not estimates.

---

## Project Structure

```
├── Model/
│   ├── RubiksCube.h / .cpp          # Abstract base class — shared interface & logic
│   ├── RubiksCube3dArray.cpp        # 3D array representation: cube[face][row][col]
│   ├── RubiksCube1dArray.cpp        # Flattened 1D array representation
│   └── RubiksCubeBitboard.cpp       # Bitboard representation (uint64_t per face)
│
├── Solver/
│   ├── DFSSolver.h                  # Depth-First Search
│   ├── BFSSolver.h                  # Breadth-First Search
│   ├── IDDFSSolver.h                # Iterative Deepening DFS
│   └── IDAstarSolver.h              # IDA* using the corner pattern database heuristic
│
├── PatternDatabases/
│   ├── CornerPatternDatabase.h/.cpp # Precomputed corner-state → min-moves lookup
│   ├── CornerDBMaker.h/.cpp         # Generates the database via reverse BFS from solved state
│   ├── PermutationIndexer.h         # O(n) Lehmer-code ranking of corner permutations
│   ├── NibbleArray.h/.cpp           # 4-bit-packed storage for move counts (0–15)
│   ├── PatternDatabase.h/.cpp       # Base class for pattern databases (file I/O, indexing)
│   └── math.h/.cpp                  # Factorial / nPk / nCk helpers
│
├── Databases/                       # Generated pattern-database files (not committed)
├── main.cpp                         # Entry point — shuffle, solve, and benchmark
└── CMakeLists.txt
```

---

## Key Features

- **Three cube representations** (3D array, 1D array, bitboard) benchmarked against each other for performance and correctness
- **Four solving algorithms**: DFS, BFS, IDDFS, and IDA*, all templated over cube representation and hash type — zero code duplication across models
- **Corner pattern database**: precomputes the minimum move count to solve the corners from every reachable corner configuration, used as an admissible heuristic for IDA*
- Random cube shuffling and move inversion for generating and verifying scrambles
- Hash functions for each representation, enabling `unordered_map`-based visited-state tracking

---

## Verified Results

All numbers below were measured directly — cube states cross-checked across all three representations after every move, and solver timings captured with `std::chrono` on a Release (`-O3`) build.

### Correctness
- All three representations (`3dArray`, `1dArray`, `Bitboard`) produce **byte-identical outputs** after every individual move and after a full 6-move rotation cycle followed by its exact inverse — confirmed independently for `U, L, F, R, B, D` and their inverses.
- `operator==`, `operator=`, and hash functors (`Hash3d`, `Hash1d`, `HashBitboard`) verified correct for `unordered_map`-based visited-state tracking, the exact mechanism BFS and IDA* rely on internally.

### Solver Benchmarks

| Algorithm | Scramble depth | Time | Solution length | Optimal? |
|---|---|---|---|---|
| DFS | 8 moves | 0.685s | 10 moves | No — first solution found, not shortest |
| BFS | 6 moves | 44.0s | ≤6 moves | Yes — guaranteed shortest, but memory/time cost scales fast |
| IDDFS | 8 moves | 0.044s | 5 moves | Near-optimal — combines DFS's low memory footprint with depth-bounded search |
| **IDA\* + corner pattern DB** | **13 moves** | **0.158s** | **7 moves** | Near-optimal, at nearly double the scramble depth of the other three |

**Takeaway:** naive uninformed search breaks down as scramble depth grows — BFS's cost explodes even at depth 6, and DFS finds solutions quickly but far from optimally. The corner pattern database heuristic is what lets IDA* search deeper (13+ moves) while staying both fast and close to optimal — this is the entire motivation for Korf's algorithm.

### Pattern Database Generation
- Corner pattern database sized to hold up to **100,179,840 entries** (covering the `8! × 3⁷ ≈ 88.18M` reachable corner permutation/orientation states), stored as 4-bit nibbles via `NibbleArray`.
- Built once via reverse BFS to depth 8 from the solved state, completing in **~11.3 minutes** on a Release build.
- **Note:** this step is allocation-heavy and must be run with compiler optimizations enabled — a Debug build can take several times longer due to unoptimized string handling in the indexing path.

---

## Design Overview

### Cube Representations
All three representations inherit from the abstract `RubiksCube` class, which defines the move interface (`u()`, `uPrime()`, `u2()`, `l()`, `f()`, `r()`, `b()`, `d()` and their variants) as pure virtual methods. Each subclass stores and rotates the cube's 54 stickers differently:

| Representation | Storage | Notes |
|---|---|---|
| `RubiksCube3dArray` | `char cube[6][3][3]` | Most intuitive; direct mapping to physical faces |
| `RubiksCube1dArray` | `char cube[54]` | Flattened via `getIndex(face, row, col)`; more flexible generic APIs |
| `RubiksCubeBitboard` | `uint64_t bitboard[6]` | 48 non-center stickers packed one byte each into six 64-bit integers; enables fast equality/hashing via direct integer XOR |

### Search Algorithms
- **DFS / IDDFS**: exhaustive depth-limited search, no heuristic. IDDFS re-runs DFS at increasing depth caps, stopping at the first depth with a solution — cheaper than BFS, more consistent than plain DFS.
- **BFS**: guarantees the shortest solution but explores and hashes every reachable state breadth-first, which becomes expensive fast (see benchmarks above).
- **IDA***: combines iterative deepening with the corner pattern database heuristic, pruning any branch whose `depth + heuristic estimate` exceeds the current bound — this is what lets it scale past what plain DFS/BFS can handle.

### Corner Pattern Database
- Encodes each of the 8 corner pieces' **position** (via `PermutationIndexer`, using Lehmer-code ranking in O(n)) and **orientation** (0/1/2, base-3 encoded)
- Stored as 4-bit nibbles (`NibbleArray`) since max corner-only solve depth fits in 0–15
- Built once via reverse BFS from the solved state (`CornerDBMaker::bfsAndStore()`) and serialized to disk for reuse — never regenerated at runtime

---

## Build

Requires CMake ≥ 3.x and a C++20-compatible compiler.

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

**Important:** always build in **Release mode** for pattern database generation (`CornerDBMaker::bfsAndStore()`) and for benchmarking solve times. A Debug build disables compiler optimizations and can be several times slower for this project's allocation-heavy indexing code.

If using an IDE like CLion, make sure the run configuration's **working directory** is set to the project root — the code reads/writes `Databases/*.txt` using relative paths.

---

## Usage

See `main.cpp` for a full example. Typical workflow:

```cpp
RubiksCubeBitboard cube;
auto shuffleMoves = cube.randomShuffleCube(13);
cube.print();

IDAstarSolver<RubiksCubeBitboard, HashBitboard> solver(cube, "Databases/cornerDepth5V1.txt");
auto solveMoves = solver.solve();

solver.rubiksCube.print();
```

**Note:** IDA* requires a pre-built pattern database file. Generate one first with:
```cpp
CornerDBMaker dbMaker("Databases/cornerDepth5V1.txt", 0x99);
dbMaker.bfsAndStore();
```
This only needs to be run once — the resulting file can be reused across sessions.

---

## Benchmarking Representations

Since all solvers are templated on the cube representation, you can compare performance directly:

```cpp
DFSSolver<RubiksCube3dArray, Hash3d> dfs3d(cube3d, 8);
DFSSolver<RubiksCube1dArray, Hash1d> dfs1d(cube1d, 8);
DFSSolver<RubiksCubeBitboard, HashBitboard> dfsBit(cubeBit, 8);
```

---

## Roadmap / Possible Extensions

- Edge pattern database (currently only corners are used as the IDA* heuristic)
- Combined corner + edge heuristic for tighter pruning
- Multi-threaded pattern database generation
- **Camera-based cube scanning with OpenCV** — capture a physical cube's state via webcam instead of manual input, covering grid overlay, pixel sampling per sticker, color classification, and feeding the result into `setColor` on the cube classes
