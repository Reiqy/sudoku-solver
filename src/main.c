#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"
#include "solver.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: '%s <sudoku>'\n", argv[0]);
        exit(1);
    }
    else
    {
        Sudoku s = readSudokuFromFile(argv[1]);
        printSudoku(&s);
        solveSudoku(&s);
        printf("Sudoku %s.\n", isSudokuSolved(&s) ? "was solved" : "could not be solved");
        printSudoku(&s);
        freeSudoku(&s);
    }
    return 0;
}
