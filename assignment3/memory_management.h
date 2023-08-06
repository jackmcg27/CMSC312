#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define META_SIZE sizeof(struct block_meta)

void *mallocNew(size_t size);
void freeNew(void *ptr);
void *reallocNew(void *ptr, size_t size);
void *callocNew(size_t nelem, size_t elsize);
struct block_meta *find_free_block(struct block_meta **last, size_t size);
struct block_meta *request_space(struct block_meta* last, size_t size);
struct block_meta *get_block_ptr(void *ptr);
int calculate_bytes_allocated();
