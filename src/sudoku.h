//
// Created by Reiqy on 01.07.2021.
//

#ifndef SUDOKUSOLVER_SUDOKU_H
#define SUDOKUSOLVER_SUDOKU_H

#include <stdint.h>
#include <stdbool.h>

#define SUDOKU_SQUARE_SIZE (9)
#define SUDOKU_SMALL_SQUARE_SIZE (3)
#define SUDOKU_ARRAY_LENGTH (SUDOKU_SQUARE_SIZE * SUDOKU_SQUARE_SIZE)

typedef struct Sudoku
{
    uint8_t *numbers;
} Sudoku;

void initSudoku(Sudoku *s);
void freeSudoku(Sudoku *s);

Sudoku readSudokuFromFile(const char *filename);

void printSudoku(Sudoku *s);

bool isSudokuBounded(Sudoku *s);
bool isSudokuValid(Sudoku *s);
bool isSudokuComplete(Sudoku *s);
bool isSudokuSolved(Sudoku *s);

void clearNumberSet(bool *set, size_t length);
void addNumberSet(bool *set, uint8_t number, size_t length);
bool isNumberPresent(bool *set, uint8_t number, size_t length);

#endif //SUDOKUSOLVER_SUDOKU_H
