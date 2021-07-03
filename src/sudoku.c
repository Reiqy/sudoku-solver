//
// Created by Reiqy on 01.07.2021.
//

#include "sudoku.h"

#include <stdio.h>
#include <ctype.h>

#include "memory.h"

void initSudoku(Sudoku *s)
{
    s->numbers = allocate(sizeof(uint8_t) * SUDOKU_ARRAY_LENGTH);
}

void freeSudoku(Sudoku *s)
{
    s->numbers = deallocate(s->numbers);
}

Sudoku readSudokuFromFile(const char *filename)
{
    Sudoku result;
    initSudoku(&result);

    FILE *f = fopen(filename, "r");

    if (f == NULL)
    {
        fprintf(stderr, "Error: Unable to open file '%s'!\n", filename);
        exit(EXIT_FAILURE);
    }

    size_t number_count = 0;
    int c;
    while (((c = fgetc(f)) != EOF) && (number_count != SUDOKU_ARRAY_LENGTH))
    {
        if (isdigit(c))
        {
            result.numbers[number_count] = (c - '0');
            number_count++;
        }
    }

    if (ferror(f) != 0)
    {
        fprintf(stderr, "Error: Unable to read file '%s'!\n", filename);
        exit(EXIT_FAILURE);
    }

    fclose(f);

    if (number_count != SUDOKU_ARRAY_LENGTH)
    {
        fprintf(stderr, "Error: Not enough numbers in file '%s' to define sudoku!\n", filename);
        exit(EXIT_FAILURE);
    }

    if (!isSudokuValid(&result))
    {
        fprintf(stderr, "Error: Sudoku defined in file '%s' is invalid!\n", filename);
        exit(EXIT_FAILURE);
    }

    return result;
}

void printSudoku(Sudoku *s)
{
    for (size_t i = 0; i < SUDOKU_SQUARE_SIZE; i++)
    {
        for (size_t j = 0; j < SUDOKU_SQUARE_SIZE; j++)
        {
            size_t current_element = i * SUDOKU_SQUARE_SIZE + j;

            if (s->numbers[current_element] != 0)
            {
                printf("%i", s->numbers[current_element]);
            }
            else
            {
                printf("-");
            }

            if (j != (SUDOKU_SQUARE_SIZE - 1))
            {
                printf(" | ");
            }
            else
            {
                printf("\n");
            }
        }
    }
}

bool isSudokuBounded(Sudoku *s)
{
    for (size_t i = 0; i < SUDOKU_ARRAY_LENGTH; i++)
    {
        if ((s->numbers[i] < 0) || (s->numbers[i] > 9))
        {
            return false;
        }
    }

    return true;
}


bool isSudokuValid(Sudoku *s)
{
    if (!isSudokuBounded(s))
    {
        return false;
    }

    bool check_rows[9];
    bool check_columns[9];
    bool check_squares[9];

    for (size_t i = 0; i < SUDOKU_SQUARE_SIZE; i++)
    {
        size_t current_square_origin_x = (i % SUDOKU_SMALL_SQUARE_SIZE) * SUDOKU_SMALL_SQUARE_SIZE;
        size_t current_square_origin_y = (i / SUDOKU_SMALL_SQUARE_SIZE) * SUDOKU_SMALL_SQUARE_SIZE;

        clearNumberSet(check_rows, SUDOKU_SQUARE_SIZE);
        clearNumberSet(check_columns, SUDOKU_SQUARE_SIZE);
        clearNumberSet(check_squares, SUDOKU_SQUARE_SIZE);

        for (size_t j = 0; j < SUDOKU_SQUARE_SIZE; j++)
        {
            size_t current_element_row = i * SUDOKU_SQUARE_SIZE + j;
            if (s->numbers[current_element_row] != 0)
            {
                if (isNumberPresent(check_rows, s->numbers[current_element_row], SUDOKU_SQUARE_SIZE))
                {
                    return false;
                }
                addNumberSet(check_rows, s->numbers[current_element_row], SUDOKU_SQUARE_SIZE);
            }

            size_t current_element_column = j * SUDOKU_SQUARE_SIZE + i;
            if (s->numbers[current_element_column] != 0)
            {
                if (isNumberPresent(check_columns, s->numbers[current_element_column], SUDOKU_SQUARE_SIZE))
                {
                    return false;
                }
                addNumberSet(check_columns, s->numbers[current_element_column], SUDOKU_SQUARE_SIZE);
            }

            size_t current_square_offset_x = j % SUDOKU_SMALL_SQUARE_SIZE;
            size_t current_square_offset_y = j / SUDOKU_SMALL_SQUARE_SIZE;

            size_t current_square_x = current_square_origin_x + current_square_offset_x;
            size_t current_square_y = current_square_origin_y + current_square_offset_y;

            size_t current_element_square = current_square_y * SUDOKU_SQUARE_SIZE + current_square_x;
            if (s->numbers[current_element_square] != 0)
            {
                if (isNumberPresent(check_squares, s->numbers[current_element_square], SUDOKU_SQUARE_SIZE))
                {
                    return false;
                }
                addNumberSet(check_squares, s->numbers[current_element_square], SUDOKU_SQUARE_SIZE);
            }
        }
    }

    return true;
}

bool isSudokuComplete(Sudoku *s)
{
    for (size_t i = 0; i < SUDOKU_ARRAY_LENGTH; i++)
    {
        if (s->numbers[i] == 0)
        {
            return false;
        }
    }

    return true;
}

bool isSudokuSolved(Sudoku *s)
{
    if (!isSudokuComplete(s))
    {
        return false;
    }

    if (!isSudokuValid(s))
    {
        return false;
    }

    return true;
}

void clearNumberSet(bool *set, size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        set[i] = false;
    }
}

void addNumberSet(bool *set, uint8_t number, size_t length)
{
    if ((number <= 0) || (number > length))
    {
        fprintf(stderr, "Error: Trying to add to set number that is out of bounds!\n");
        exit(EXIT_FAILURE);
    }

    set[number - 1] = true;
}

bool isNumberPresent(bool *set, uint8_t number, size_t length)
{
    if ((number <= 0) || (number > length))
    {
        fprintf(stderr, "Error: Trying to check for set presence that is out of bounds!\n");
        exit(EXIT_FAILURE);
    }

    return set[number - 1];
}