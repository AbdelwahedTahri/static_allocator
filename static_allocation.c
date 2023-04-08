#include "static_allocation.h"

static _Alignas(max_align_t) char static_memory[MEMORY_SIZE];

static void* list_of_free_blocks;

size_t adjust_size(size_t size)
{
    /**
     * Make size a multiple of MAX_ALIGNMENT
    */
    size_t rest = size % MAX_ALIGNMENT;

    if(rest)
        size += MAX_ALIGNMENT - rest;
    
    return size;
}

_Bool are_successive(struct heading * start_address, struct heading *next_address)
{
    return ((char *)(start_address) + SIZE_OF_HEADING + start_address->block_size == (char*)(next_address));
}

void *static_malloc(size_t size)
{
    // Points to the first address of the area that is not used yet
    static char *blank_area = static_memory;

    // Check if a previous freed block can be used
    for(struct heading * free_block = list_of_free_blocks; free_block != NULL; free_block = free_block->next_free_block){

        if(free_block->block_size >= size){

            return ((char*)(free_block) + SIZE_OF_HEADING);

        }else{

            struct heading * sub_sweep = free_block;
            size_t size_combined = free_block->block_size;

            while(are_successive(sub_sweep, sub_sweep->next_free_block)){

                size_combined += sub_sweep->next_free_block->block_size + SIZE_OF_HEADING;
                if(size_combined >= size){

                    free_block->block_size += size_combined;
                    free_block->next_free_block = sub_sweep->next_free_block->next_free_block;
                    return  ((char*)(free_block) + SIZE_OF_HEADING);
                }

                sub_sweep = sub_sweep->next_free_block;
            }
            
        }
    }

    // if the memory is full return NULL
    if((unsigned)(blank_area - static_memory) >= MEMORY_SIZE)
        return NULL;

    // allocate a new block from the blank_area
    struct heading * heading = (void*) blank_area;
    size_t block_size = adjust_size(size);
    char * allocated_block = blank_area + SIZE_OF_HEADING;

    heading->block_size = block_size;
    blank_area += SIZE_OF_HEADING + block_size;

    return allocated_block;
}

void static_free(void* block_)
{
    if(block_ == NULL)
        return;
    
    struct heading *block_address = (void *)((char*)block_ - SIZE_OF_HEADING);

    // if empty list then "block" will be the first element
    if(list_of_free_blocks == NULL){
        list_of_free_blocks = block_address;
        return;
    }
    
    //else add "block_" to an ordered list
    struct heading * list_sweeper = list_of_free_blocks;

    if(block_address < list_sweeper){// if it should be swapped with the first element

        block_address->next_free_block = list_sweeper;
        list_of_free_blocks = block_address;
        return;
    }

    while(list_sweeper->next_free_block != NULL){ //while it's not the last element

        if(list_sweeper->next_free_block > block_address){//if it should be placed before an element

            struct heading * aux = list_sweeper->next_free_block;
            list_sweeper->next_free_block = block_address;
            block_address->next_free_block = aux;
            return;
        }
        list_sweeper = list_sweeper->next_free_block;// go to the next element

    }
    list_sweeper->next_free_block = block_address;//if nothing of what is before, add it to the tail

}
