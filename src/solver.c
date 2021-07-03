//
// Created by Reiqy on 01.07.2021.
//

#include <stdio.h>

#include "solver.h"

#include "memory.h"

void initPossibleNumbers(PossibleNumbers *p_nums)
{
    p_nums->count = 0;
}

void addPossibleNumber(PossibleNumbers *p_nums, uint8_t number)
{
    p_nums->numbers[p_nums->count] = number;
    p_nums->count++;
}


bool solveSudoku(Sudoku *s)
{
    if (solveDeterministically(s, NULL) == false)
    {
        return solveNondeterministically(s);
    }

    return true;
}

bool solveDeterministically(Sudoku *s, PlacementPath *path)
{
    bool is_solvable;
    do
    {
        is_solvable = false;
        PossibleNumbers possible_numbers[SUDOKU_ARRAY_LENGTH];
        for (size_t i = 0; i < SUDOKU_SQUARE_SIZE; i++)
        {
            for (size_t j = 0; j < SUDOKU_SQUARE_SIZE; j++)
            {
                size_t current_element = i * SUDOKU_SQUARE_SIZE + j;
                if (s->numbers[current_element] == 0)
                {
                    initPossibleNumbers(&possible_numbers[current_element]);
                    findPossibleNumbers(s, &possible_numbers[current_element], j, i);
                    is_solvable = (possible_numbers[current_element].count == 1) || is_solvable;
                    if (possible_numbers[current_element].count == 1)
                    {
                        s->numbers[current_element] = possible_numbers[current_element].numbers[0];
                        if (path != NULL)
                        {
                            Placement p = {.pos_x = j, .pos_y = i, .number = possible_numbers[current_element].numbers[0]};
                            addPlacementPath(path, p);
                        }
                    }
                }
            }
        }
    } while (is_solvable);

    return isSudokuComplete(s);
}

bool solveNondeterministically(Sudoku *s)
{
    NondeterministicNode root;
    initNondeterministicNode(&root, NULL);
    NondeterministicNode *current = &root;

    NodeManager manager;
    initNodeManager(&manager);

    Placement p;
    while (true)
    {
        if (!findNextGuess(s, &p, current->attempts))
        {
            if (!revertSudoku(s, current))
            {
                return false;
            }
            current = current->previous;
            continue;
        }

        current = getNondeterministicNode(&manager, current);
        addPlacementPath(&current->path, p);
        s->numbers[p.pos_y * SUDOKU_SQUARE_SIZE + p.pos_x] = p.number;

        if (solveDeterministically(s, &current->path))
        {
            freeNondeterministicNode(&root);
            freeNodeManager(&manager);
            return true;
        }
    }
}

void findPossibleNumbers(Sudoku *s, PossibleNumbers *p_nums, size_t pos_x, size_t pos_y)
{
    size_t square_origin_x = (pos_x / SUDOKU_SMALL_SQUARE_SIZE) * SUDOKU_SMALL_SQUARE_SIZE;
    size_t square_origin_y = (pos_y / SUDOKU_SMALL_SQUARE_SIZE) * SUDOKU_SMALL_SQUARE_SIZE;

    uint8_t checked_number;
    bool impossible_numbers[SUDOKU_SQUARE_SIZE];
    clearNumberSet(impossible_numbers, SUDOKU_SQUARE_SIZE);
    for (size_t i = 0; i < SUDOKU_SQUARE_SIZE; i++)
    {
        checked_number = s->numbers[pos_y * SUDOKU_SQUARE_SIZE + i];
        if (checked_number != 0)
        {
            addNumberSet(impossible_numbers, checked_number, SUDOKU_SQUARE_SIZE);
        }

        checked_number = s->numbers[i * SUDOKU_SQUARE_SIZE + pos_x];
        if (checked_number != 0)
        {
            addNumberSet(impossible_numbers, checked_number, SUDOKU_SQUARE_SIZE);
        }

        size_t square_offset_x = i % SUDOKU_SMALL_SQUARE_SIZE;
        size_t square_offset_y = i / SUDOKU_SMALL_SQUARE_SIZE;
        size_t square_current_x = square_origin_x + square_offset_x;
        size_t square_current_y = square_origin_y + square_offset_y;
        checked_number = s->numbers[square_current_y * SUDOKU_SQUARE_SIZE + square_current_x];

        if (checked_number != 0)
        {
            addNumberSet(impossible_numbers, checked_number, SUDOKU_SQUARE_SIZE);
        }
    }

    for (size_t i = 0; i < SUDOKU_SQUARE_SIZE; i++)
    {
        if (!impossible_numbers[i])
        {
            addPossibleNumber(p_nums, i + 1);
        }
    }
}

void initPlacementPath(PlacementPath *path)
{
    path->capacity = 0;
    path->count = 0;
    path->path = NULL;
}

void freePlacementPath(PlacementPath *path)
{
    path->capacity = 0;
    path->count = 0;
    path->path = deallocate(path->path);
}

void addPlacementPath(PlacementPath *path, Placement p)
{
    if (path->capacity < path->count + 1)
    {
        path->capacity = growCapacity(path->capacity);
        path->path = reallocate(path->path, sizeof(Placement) * path->capacity);
    }

    path->path[path->count] = p;
    path->count++;
}

void initNondeterministicNode(NondeterministicNode *node, NondeterministicNode *previous)
{
    node->attempts = 0;
    node->previous = previous;
    initPlacementPath(&node->path);
}

void freeNondeterministicNode(NondeterministicNode *node)
{
    node->previous = NULL;
    freePlacementPath(&node->path);
}

bool findNextGuess(Sudoku *s, Placement *p, int attempt)
{
    PossibleNumbers possible_numbers[SUDOKU_ARRAY_LENGTH];

    for (size_t i = 0; i < SUDOKU_SQUARE_SIZE; i++)
    {
        for (size_t j = 0; j < SUDOKU_SQUARE_SIZE; j++)
        {
            size_t current_element = i * SUDOKU_SQUARE_SIZE + j;
            initPossibleNumbers(&possible_numbers[current_element]);
            if (s->numbers[current_element] == 0)
            {
                findPossibleNumbers(s, &possible_numbers[current_element], j, i);
    /*
                if (attempt < possible_numbers.count)
                {
                    p->pos_x = j;
                    p->pos_y = i;
                    p->number = possible_numbers.numbers[attempt];
                    return true;
                }
                else
                {
                    attempt -= (int) possible_numbers.count;
                }
                */
            }
            else
            {
                possible_numbers[current_element].count = 0;
            }
        }
    }

    size_t current_max_entropy = 2;
    while (current_max_entropy <= SUDOKU_SQUARE_SIZE)
    {
        for (size_t i = 0; i < SUDOKU_SQUARE_SIZE; i++)
        {
            for (size_t j = 0; j < SUDOKU_SQUARE_SIZE; j++)
            {
                size_t current_element = i * SUDOKU_SQUARE_SIZE + j;
                if (possible_numbers[current_element].count == current_max_entropy)
                {
                    if (attempt < possible_numbers[current_element].count)
                    {
                        p->pos_x = j;
                        p->pos_y = i;
                        p->number = possible_numbers[current_element].numbers[attempt];
                        return true;
                    }
                    else
                    {
                        attempt -= (int) possible_numbers[current_element].count;
                    }
                }
            }
        }
        current_max_entropy++;
    }

    return false;
}

bool revertSudoku(Sudoku *s, NondeterministicNode *node)
{
    if (node->previous == NULL)
    {
        printf("Cannot revert root node!\n");
        return false;
    }
    PlacementPath *path = &node->path;
    for (size_t i = path->count; i > 0; i--)
    {
        Placement *revert = &path->path[i - 1];
        s->numbers[revert->pos_y * SUDOKU_SQUARE_SIZE + revert->pos_x] = 0;
    }

    return true;
}

void initNodeManager(NodeManager *manager)
{
    manager->capacity = 0;
    manager->count = 0;
    manager->nodes = NULL;
}

void freeNodeManager(NodeManager *manager)
{
    for (size_t i = 0; i < manager->count; i++)
    {
        freeNondeterministicNode(manager->nodes[i]);
    }

    manager->capacity = 0;
    manager->count = 0;
    manager->nodes = deallocate(manager->nodes);
}

NondeterministicNode *getNondeterministicNode(NodeManager *manager, NondeterministicNode *from_node)
{
    if (manager->capacity < manager->count + 1)
    {
        manager->capacity = growCapacity(manager->capacity);
        manager->nodes = reallocate(manager->nodes, sizeof(NondeterministicNode *) * manager->capacity);
    }

    from_node->attempts++;

    NondeterministicNode *new = allocate(sizeof(NondeterministicNode));
    initNondeterministicNode(new, from_node);
    manager->nodes[manager->count] = new;
    manager->count++;

    return new;
}

