//
// Created by Reiqy on 01.07.2021.
//

#ifndef SUDOKUSOLVER_MEMORY_H
#define SUDOKUSOLVER_MEMORY_H

#include <stdlib.h>

void *allocate(size_t size);
void *deallocate(void *ptr);
void *reallocate(void *ptr, size_t new_size);

size_t growCapacity(size_t oldCapacity);

#endif //SUDOKUSOLVER_MEMORY_H
