//
// Created by Reiqy on 01.07.2021.
//

#ifndef SUDOKUSOLVER_SOLVER_H
#define SUDOKUSOLVER_SOLVER_H

#include "sudoku.h"

typedef struct PossibleNumbers
{
    size_t count;
    uint8_t numbers[SUDOKU_SQUARE_SIZE];
} PossibleNumbers;

void initPossibleNumbers(PossibleNumbers *p_nums);
void addPossibleNumber(PossibleNumbers *p_nums, uint8_t number);

void findPossibleNumbers(Sudoku *s, PossibleNumbers *p_nums, size_t pos_x, size_t pos_y);

typedef struct Placement
{
    size_t pos_x;
    size_t pos_y;
    uint8_t number;
} Placement;

typedef struct PlacementPath
{
    size_t capacity;
    size_t count;
    Placement *path;
} PlacementPath;

void initPlacementPath(PlacementPath *path);
void freePlacementPath(PlacementPath *path);
void addPlacementPath(PlacementPath *path, Placement p);

typedef struct NondeterministicNode
{
    int attempts;
    struct NondeterministicNode *previous;
    PlacementPath path;
} NondeterministicNode;

void initNondeterministicNode(NondeterministicNode *node, NondeterministicNode *previous);
void freeNondeterministicNode(NondeterministicNode *node);

typedef struct NodeManager
{
    size_t capacity;
    size_t count;
    NondeterministicNode **nodes;
} NodeManager;

void initNodeManager(NodeManager *manager);
void freeNodeManager(NodeManager *manager);

NondeterministicNode * getNondeterministicNode(NodeManager *manager, NondeterministicNode *from_node);

bool findNextGuess(Sudoku *s, Placement *p, int attempt);
bool revertSudoku(Sudoku *s, NondeterministicNode *node);

bool solveSudoku(Sudoku *s);
bool solveDeterministically(Sudoku *s, PlacementPath *path);
bool solveNondeterministically(Sudoku *s);

#endif //SUDOKUSOLVER_SOLVER_H
