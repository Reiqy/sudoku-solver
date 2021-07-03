cmake_minimum_required(VERSION 3.19)
project(SudokuSolver C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSUDOKU_SOLVER_DEBUG")

add_executable(SudokuSolver src/main.c src/sudoku.c src/sudoku.h src/solver.c src/solver.h src/memory.c src/memory.h)