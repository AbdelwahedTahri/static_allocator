#ifndef STATIC_ALLOCATION_H
#define STATIC_ALLOCATION_H

#include <stdlib.h>
#include <stddef.h>

#define MEMORY_SIZE             (1048576UL)
#define MAX_ALIGNMENT           (_Alignof(max_align_t))
#define SIZE_OF_HEADING         adjust_size(sizeof(struct heading))

struct heading {

    size_t block_size;
    struct heading * next_free_block;
};

extern size_t adjust_size(size_t size);
extern void *static_malloc(size_t size);
extern void static_free(void* block);




#endif
