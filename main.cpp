#include <bits/stdc++.h>
#include "Model/RubiksCube3dArray.cpp"
#include "Model/RubiksCube1dArray.cpp"
#include "Solver/DFSSolver.h"
#include "Solver/BFSSolver.h"
#include "Solver/IDDFSSolver.h"
#include "Solver/IDAstarSolver.h"
#include "PatternDatabases/CornerPatternDatabase.h"
#include "PatternDatabases/CornerDBMaker.h"

using namespace std;

int main() {

    // ================= STEP 1: Creation, print, isSolved =================
    /*
    RubiksCube3dArray object3DArray;
    RubiksCube1dArray object1dArray;
    RubiksCubeBitboard objectBitboard;

    object3DArray.print();
    object1dArray.print();
    objectBitboard.print();

    if (object3DArray.isSolved()) cout << "SOLVED\n\n";
    else cout << "NOT SOLVED\n\n";

    if (object1dArray.isSolved()) cout << "SOLVED\n\n";
    else cout << "NOT SOLVED\n\n";

    if (objectBitboard.isSolved()) cout << "SOLVED\n\n";
    else cout << "NOT SOLVED\n\n";
    */


    // ================= STEP 2: Single move (u) consistency =================
    /*
    objectBitboard.u();
    object3DArray.u();
    object1dArray.u();
    objectBitboard.print();
    object3DArray.print();
    object1dArray.print();
    */


    // ================= STEP 3: Full move cycle + inverse =================

    /*
    objectBitboard.l();
    object3DArray.l();
    object1dArray.l();
    objectBitboard.print();
    object3DArray.print();
    object1dArray.print();

    objectBitboard.f();
    object3DArray.f();
    object1dArray.f();
    objectBitboard.print();
    object3DArray.print();
    object1dArray.print();

    objectBitboard.r();
    object3DArray.r();
    object1dArray.r();
    objectBitboard.print();
    object3DArray.print();
    object1dArray.print();

    objectBitboard.b();
    object3DArray.b();
    object1dArray.b();
    objectBitboard.print();
    object3DArray.print();
    object1dArray.print();

    objectBitboard.d();
    object3DArray.d();
    object1dArray.d();
    objectBitboard.print();
    object3DArray.print();
    object1dArray.print();

    if (object3DArray.isSolved()) cout << "SOLVED\n\n"; else cout << "NOT SOLVED\n\n";
    if (object1dArray.isSolved()) cout << "SOLVED\n\n"; else cout << "NOT SOLVED\n\n";
    if (objectBitboard.isSolved()) cout << "SOLVED\n\n"; else cout << "NOT SOLVED\n\n";

    objectBitboard.dPrime();
    object3DArray.dPrime();
    object1dArray.dPrime();

    objectBitboard.bPrime();
    object3DArray.bPrime();
    object1dArray.bPrime();

    objectBitboard.rPrime();
    object3DArray.rPrime();
    object1dArray.rPrime();

    objectBitboard.fPrime();
    object3DArray.fPrime();
    object1dArray.fPrime();

    objectBitboard.lPrime();
    object3DArray.lPrime();
    object1dArray.lPrime();

    objectBitboard.uPrime();
    object3DArray.uPrime();
    object1dArray.uPrime();

    objectBitboard.print();
    object3DArray.print();
    object1dArray.print();

    if (object3DArray.isSolved()) cout << "SOLVED\n\n"; else cout << "NOT SOLVED\n\n";
    if (object1dArray.isSolved()) cout << "SOLVED\n\n"; else cout << "NOT SOLVED\n\n";
    if (objectBitboard.isSolved()) cout << "SOLVED\n\n"; else cout << "NOT SOLVED\n\n";
    */


    // ================= STEP 4: Equality & assignment =================
    /*
    RubiksCube3dArray cube1;
    RubiksCube3dArray cube2;

    if (cube1 == cube2) cout << "Is equal\n"; else cout << "Not Equal\n";

    cube1.randomShuffleCube(1);

    if (cube1 == cube2) cout << "Is equal\n"; else cout << "Not Equal\n";

    cube2 = cube1;

    if (cube1 == cube2) cout << "Is equal\n"; else cout << "Not Equal\n";
    */


    // ================= STEP 5: Hash / unordered_map test =================

    /*
    unordered_map<RubiksCube3dArray, bool, Hash3d> mp1;

    mp1[cube1] = true;
    cube2.randomShuffleCube(8);

    if (mp1[cube1]) cout << "Cube1 is present\n"; else cout << "Cube1 is not present\n";
    if (mp1[cube2]) cout << "Cube2 is present\n"; else cout << "Cube2 is not present\n";
    */


    // ================= STEP 6: DFS Solver (TIMED) =================
    /*RubiksCube3dArray cube;
    cube.print();

    vector<RubiksCube::MOVE> shuffle_moves = cube.randomShuffleCube(8);
    for (auto move : shuffle_moves) cout << cube.getMove(move) << " ";
    cout << "\n";
    cube.print();

    auto dfs_start = chrono::high_resolution_clock::now();

    DFSSolver<RubiksCube3dArray, Hash3d> dfsSolver(cube, 10);
    vector<RubiksCube::MOVE> solve_moves = dfsSolver.solve();

    auto dfs_end = chrono::high_resolution_clock::now();
    double dfs_seconds = chrono::duration<double>(dfs_end - dfs_start).count();

    for (auto move : solve_moves) cout << cube.getMove(move) << " ";
    cout << "\n";
    dfsSolver.rubiksCube.print();
    cout << "DFS solve time: " << dfs_seconds << " seconds\n\n";
    */

    // ================= STEP 7: BFS Solver (TIMED) =================
    /*
    RubiksCubeBitboard cube;
    cube.print();

    vector<RubiksCube::MOVE> shuffle_moves = cube.randomShuffleCube(6);
    for (auto move : shuffle_moves) cout << cube.getMove(move) << " ";
    cout << "\n";
    cube.print();

    auto bfs_start = chrono::high_resolution_clock::now();

    BFSSolver<RubiksCubeBitboard, HashBitboard> bfsSolver(cube);
    vector<RubiksCube::MOVE> solve_moves = bfsSolver.solve();

    auto bfs_end = chrono::high_resolution_clock::now();
    double bfs_seconds = chrono::duration<double>(bfs_end - bfs_start).count();

    for (auto move : solve_moves) cout << cube.getMove(move) << " ";
    cout << "\n";
    bfsSolver.rubiksCube.print();
    cout << "BFS solve time: " << bfs_seconds << " seconds\n\n";

    */

    // ================= STEP 8: IDDFS Solver (TIMED) =================
    /*
    RubiksCubeBitboard cube;
    cube.print();

    vector<RubiksCube::MOVE> shuffle_moves = cube.randomShuffleCube(8);
    for (auto move : shuffle_moves) cout << cube.getMove(move) << " ";
    cout << "\n";
    cube.print();

    auto iddfs_start = chrono::high_resolution_clock::now();

    IDDFSSolver<RubiksCubeBitboard, HashBitboard> iddfsSolver(cube, 8);
    vector<RubiksCube::MOVE> solve_moves = iddfsSolver.solve();

    auto iddfs_end = chrono::high_resolution_clock::now();
    double iddfs_seconds = chrono::duration<double>(iddfs_end - iddfs_start).count();

    for (auto move : solve_moves) cout << cube.getMove(move) << " ";
    cout << "\n";
    iddfsSolver.rubiksCube.print();
    cout << "IDDFS solve time: " << iddfs_seconds << " seconds\n\n";
    */


    // ================= STEP 9: Build the pattern database (run ONCE, TIMED) =================

    /*
    string dbFileName = "Databases/cornerDepth5V1.txt";

    auto db_start = chrono::high_resolution_clock::now();

    CornerDBMaker dbMaker(dbFileName, 0x99);
    dbMaker.bfsAndStore();

    auto db_end = chrono::high_resolution_clock::now();
    double db_seconds = chrono::duration<double>(db_end - db_start).count();

    cout << "Database built.\n";
    cout << "Database generation time: " << db_seconds << " seconds\n\n";

    */
    // ================= STEP 10: IDA* Solver (uses the generated database, TIMED) =================
    /*
    string dbFileName = "Databases/cornerDepth5V1.txt";

    RubiksCubeBitboard cube;
    auto shuffleMoves = cube.randomShuffleCube(13);
    cube.print();
    for (auto move : shuffleMoves) cout << cube.getMove(move) << " ";
    cout << "\n";

    auto ida_start = chrono::high_resolution_clock::now();

    IDAstarSolver<RubiksCubeBitboard, HashBitboard> idaStarSolver(cube, dbFileName);
    auto moves = idaStarSolver.solve();

    auto ida_end = chrono::high_resolution_clock::now();
    double ida_seconds = chrono::duration<double>(ida_end - ida_start).count();

    idaStarSolver.rubiksCube.print();
    for (auto move : moves) cout << cube.getMove(move) << " ";
    cout << "\n";
    cout << "IDA* solve time: " << ida_seconds << " seconds\n\n";
    */

    return 0;
}