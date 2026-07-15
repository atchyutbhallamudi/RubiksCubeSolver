# Rubik's Cube Solver 

A C++20 implementation of a Rubik's Cube solver featuring **three interchangeable cube representations**, **four search algorithms**, and a **corner pattern database** for heuristic-guided solving. The project is built around a polymorphic `RubiksCube` base class, allowing every solver and pattern database to work with any representation via templates.

---

## 📂 Project Structure

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

## 🚀 Key Features

- **Three cube representations** (3D array, 1D array, bitboard) benchmarked against each other for performance
- **Four solving algorithms**: DFS, BFS, IDDFS, and IDA*
- **Corner pattern database**: precomputes the minimum move count to solve the corners from every reachable corner configuration, used as an admissible heuristic for IDA*
- **Generic, template-based solvers** — every solver works with any of the three representations via `template<typename T, typename H>`
- Random cube shuffling and move inversion for generating and verifying scrambles
- Hash functions for each representation, enabling `unordered_map`-based visited-state tracking

---

## ⚙️ Design Overview

### Cube Representations
All three representations inherit from the abstract `RubiksCube` class, which defines the move interface (`u()`, `uPrime()`, `u2()`, `l()`, `f()`, `r()`, `b()`, `d()` and their variants) as pure virtual methods. Each subclass stores and rotates the cube's 54 stickers differently:

| Representation | Storage | Notes |
|---|---|---|
| `RubiksCube3dArray` | `char cube[6][3][3]` | Most intuitive; direct mapping to physical faces |
| `RubiksCube1dArray` | `char cube[54]` | Flattened via `getIndex(face, row, col)`; more flexible generic APIs |
| `RubiksCubeBitboard` | `uint64_t bitboard[6]` | One byte per sticker, packed into a 64-bit face; fastest for hashing/equality |

### Search Algorithms
- **DFS / IDDFS**: exhaustive depth-limited search, no heuristic
- **BFS**: guarantees shortest solution but memory-heavy at depth
- **IDA***: combines iterative deepening with the corner pattern database heuristic, pruning branches whose `depth + heuristic` exceeds the current bound

### Corner Pattern Database
- Encodes each of the 8 corner pieces' **position** (via `PermutationIndexer`, using Lehmer-code ranking) and **orientation** (0/1/2, base-3 encoded)
- Total index space: `8! × 3^7 = 40320 × 2187 ≈ 100.18M` entries
- Stored as 4-bit nibbles (`NibbleArray`) since max corner-only solve depth fits in 0–15
- Built once via reverse BFS from the solved state (`CornerDBMaker::bfsAndStore()`) and serialized to disk for reuse

---

## 🛠️ Build

Requires CMake ≥ 3.x and a C++20-compatible compiler.

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

---

## 📖 Usage

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

## 🔍 Benchmarking Representations

Since all solvers are templated on the cube representation, you can compare performance directly:

```cpp
DFSSolver<RubiksCube3dArray, Hash3d> dfs3d(cube3d, 8);
DFSSolver<RubiksCube1dArray, Hash1d> dfs1d(cube1d, 8);
DFSSolver<RubiksCubeBitboard, HashBitboard> dfsBit(cubeBit, 8);
```

---

## 📌 Roadmap / Possible Extensions

- Edge pattern database (currently only corners are used as the IDA* heuristic)
- Combined corner + edge heuristic for tighter pruning
- Multi-threaded pattern database generation
