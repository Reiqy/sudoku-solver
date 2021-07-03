//
// Created by Reiqy on 01.07.2021.
//

#include "memory.h"

#include <stdio.h>

void *allocate(size_t size)
{
    void *ptr = malloc(size);

    if (ptr == NULL)
    {
        fprintf(stderr, "Error: Out of memory!\n");
        exit(EXIT_FAILURE);
    }

    return ptr;
}

void *deallocate(void *ptr)
{
    free(ptr);
    return NULL;
}

void *reallocate(void *ptr, size_t new_size)
{
    if (new_size == 0)
    {
        return deallocate(ptr);
    }

    void *new = realloc(ptr, new_size);

    if (new == NULL)
    {
        fprintf(stderr, "Error: Out of memory!\n");
        exit(EXIT_FAILURE);
    }

    return new;
}

size_t growCapacity(size_t oldCapacity)
{
    return (oldCapacity == 0) ? 8 : 2 * oldCapacity;
}


