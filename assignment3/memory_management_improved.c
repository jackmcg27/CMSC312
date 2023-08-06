#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "memory_management.h"

struct block_meta {
    size_t size;
    struct block_meta *next, *prev;
    int free;
    int magic; // for debugging only -- TODO: Remove this in non-debug mode
};

void *global_base = NULL;

void *mallocNew(size_t size) {
    struct block_meta *block;
    //TODO: align size?

    if (size <= 0) {
        return NULL;
    }

    if (!global_base) { //  First call
        block = request_space(NULL, size);
        if (!block) {
            return NULL;
        }
        global_base = block;
    } else {
        struct block_meta *last = global_base;
        block = find_free_block(&last, size);
        if (!block) { // Failed to find free block
            block = request_space(last, size);
            if (!block) {
                return NULL;
            }
        } else { // Found free block
            // TODO: consider splitting block here
            block->free = 0;
            block->magic = 0x77777777;
        }
    }
    return(block + 1);
}

void freeNew(void *ptr) {
    if (!ptr) {
        return;
    }
    // TODO: consider merging blocks once splitting blocks is implemented
    struct block_meta* block_ptr = get_block_ptr(ptr);
    assert(block_ptr->free == 0);
    assert(block_ptr->magic == 0x77777777 || block_ptr->magic == 0x12345678);
    block_ptr->free = 1;
    block_ptr->magic = 0x55555555;
}

void *reallocNew(void *ptr, size_t size) {
    if (!ptr) {
        // NULL ptr - realloc should act like malloc
        return mallocNew(size);
    }

    struct block_meta* block_ptr = get_block_ptr(ptr);
    if (block_ptr->size >= size) {
        //We have enough space - could free some once we implement split
        return ptr;
    }

    // Need to really realloc - Malloc new space and free old space.
    // Then copy old data to new space
    void *new_ptr;
    new_ptr = mallocNew(size);
    if (!new_ptr) {
        return NULL; // TODO: set errno on failure
    }
    memcpy(new_ptr, ptr, block_ptr->size);
    freeNew(ptr);
    return new_ptr;
}

void *callocNew(size_t nelem, size_t elsize) {
    size_t size = nelem * elsize; // TODO: check for overflow
    void *ptr = mallocNew(size);
    memset(ptr, 0, size);
    return ptr;
}

struct block_meta *find_free_block(struct block_meta **last, size_t size) {
    struct block_meta *current = global_base;
    struct block_meta *bestFit = NULL;
    while (current) {
        if (current->free == 1) { // If current is free
            if (current->size >= size) { // If current is free and current->size can hold size
                if (bestFit != NULL) { // If bestFit is not NULL
                    if (current->size < bestFit->size) { // If current->size is smaller than bestFit->size
                        bestFit = current; // set bestFit to current since current is a better fir
                    }
                } else { // if bestFit is NULL, place current in bestFit since it can hold the size
                    bestFit = current;
                }
            }
        }
        *last = current;
        current = current->next; // Go until current is NULL
    }
    return bestFit;
}

struct block_meta *request_space(struct block_meta* last, size_t size) {
    struct block_meta *block;
    block = sbrk(0);
    void *request = sbrk(size + META_SIZE);
    assert((void*)block == request); // Not thread safe
    if (request == (void*) -1) {
        return NULL; // sbrk failed
    }
    if (last) { // NULL on first request
        last->next = block;
    }
    block->size = size;
    block->next = NULL;
    block->prev = last;
    block->free = 0;
    block->magic = 0x12345678;
    return block;
}

struct block_meta *get_block_ptr(void *ptr) {
    return (struct block_meta*)ptr - 1;
}

int calculate_bytes_allocated() {
    struct block_meta *current = global_base;
    int bytesUsed = 0;
    while(current) {
        if (current->free == 0) {
            bytesUsed += current->size; // Ignore structure size
            //leakAmmount += current->size + META_SIZE; // Factor in the size of the structure
        }
        current = current->next;
    }
    return bytesUsed;
}