# Sudoku Solver

CLI Sudoku solving application written in C99.

## Running the Project

This project uses CMake. To run it you either need to compile it yourself or download a precompiled binary.

### Compiling

*TBD*

### Precompiled

Precompiled binary can be downloaded [here](https://github.com/Reiqy/sudoku-solver/releases/).

### Usage

After you have the executable ready on your system. You can run it from command line by calling

    $ SudokuSolver.exe <path_to_sudoku_file>
    
Note that the name of the executable might be different depending on the target OS.

Try using one of the sudoku text files from the sudoku folder as a starting point!

## Sudoku files

Sudoku text files don't require any specific formatting. They must contain at least 81 digits (0 - 9). The program will read the file ignoring all characters except for the digits and it will use the first 81 digits to construct a sudoku. Ordering is row-major (numbers fill rows first, columns second). Digit 0 is used as a placeholder for empty space in Sudoku puzzle.
