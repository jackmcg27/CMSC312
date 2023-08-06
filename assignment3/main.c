#include "memory_management.h"
#include <stdio.h>
#include <unistd.h>

void main() {

    int allocatedBytes = 0;
    void *startHeap = sbrk(0);
    printf("Starting heap address: %p\n", sbrk(0));

    // Calling malloc 10 times
    void *mallocArray1 = malloc(5 * 1);
    //printf("Allocated memory %p\n", mallocArray1);
    allocatedBytes += 5 * 1 + META_SIZE;

    void *mallocArray2 = malloc(10 * 1);
    //printf("Allocated memory %p\n", mallocArray2);
    allocatedBytes += 10 * 1 + META_SIZE;

    void *mallocArray3 = malloc(8 * 1);
    //printf("Allocated memory %p\n", mallocArray3);  
    allocatedBytes += 8 * 1 + META_SIZE;

    void *mallocArray4 = malloc(15 * 1);
    //printf("Allocated memory %p\n", mallocArray4);
    allocatedBytes += 15 * 1 + META_SIZE;

    void *mallocArray5 = malloc(3 * 1);
    //printf("Allocated memory %p\n", mallocArray5);
    allocatedBytes += 3 * 1 + META_SIZE;

    void *mallocString1 = malloc(50 * 1);
    //printf("Allocated memory %p\n", mallocString1);
    allocatedBytes += 50 * 1 + META_SIZE;

    void *mallocString2 = malloc(10 * 1);
    //printf("Allocated memory %p\n", mallocString2);
    allocatedBytes += 10 * 1 + META_SIZE;

    void *mallocString3 = malloc(20 * 1);
    //printf("Allocated memory %p\n", mallocString3);
    allocatedBytes += 20 * 1 + META_SIZE;

    void *mallocString4 = malloc(25 * 1);
    //printf("Allocated memory %p\n", mallocString4);
    allocatedBytes += 25 * 1 + META_SIZE;

    void *mallocString5 = malloc(9 * 1);
    //printf("Allocated memory %p\n", mallocString5);
    allocatedBytes += 9 * 1 + META_SIZE;

    // Calling calloc 10 times
    void *callocString1 = calloc(20, 1);
    //printf("Allocated memory %p\n", callocString1);
    allocatedBytes += 20 * 1 + META_SIZE;

    void *callocString2 = calloc(5, 1);
    //printf("Allocated memory %p\n", callocString2);
    allocatedBytes += 5 * 1 + META_SIZE;

    void *callocString3 = calloc(10, 1);
    //printf("Allocated memory %p\n", callocString3);
    allocatedBytes += 10 * 1 + META_SIZE;

    void *callocArray1 = calloc(5, 1);
    //printf("Allocated memory %p\n", callocArray1);
    allocatedBytes += 5 * 1 + META_SIZE;

    void *callocArray2 = calloc(10, 1);
    //printf("Allocated memory %p\n", callocArray2);
    allocatedBytes += 10 * 1 + META_SIZE;

    void *callocArray3 = calloc(8, 1);
    //printf("Allocated memory %p\n", callocArray3);
    allocatedBytes += 8 * 1 + META_SIZE;

    void *callocArray4 = calloc(15, 1);
    //printf("Allocated memory %p\n", callocArray4);
    allocatedBytes += 15 * 1 + META_SIZE;

    void *callocArray5 = calloc(3, 1);
    //printf("Allocated memory %p\n", callocArray5);
    allocatedBytes += 3 * 1 + META_SIZE;

    void *callocArray6 = calloc(7, 1); 
    //printf("Allocated memory %p\n", callocArray6);
    allocatedBytes += 7 * 1 + META_SIZE;

    void *callocArray7 = calloc(6, 1);
    //printf("Allocated memory %p\n", callocArray7);
    allocatedBytes += 6 * 1 + META_SIZE;

    printf("Heap address after malloc and calloc: %p\n", sbrk(0));
    printf("Bytes on the heap: %ld\n", (sbrk(0) - startHeap));
    printf("Number of bytes allocated: %d\n", allocatedBytes);

    // Calling realloc 10 times
    mallocArray1 = realloc(mallocArray1, 10); // Bigger than original (5)
    //printf("Allocated memory %p\n", mallocArray1);
    allocatedBytes += 10 * 1 + META_SIZE;
    mallocArray2 = realloc(mallocArray2, 5); // Smaller than original (10)
    allocatedBytes += 5 * 1 + META_SIZE;
    //printf("Allocated memory %p\n", mallocArray2);
    mallocArray3 = realloc(mallocArray3, 4); // Smaller than original (8)
    allocatedBytes += 4 * 1 + META_SIZE;
    //printf("Allocated memory %p\n", mallocArray3);
    callocArray1 = realloc(callocArray1, 8); // Bigger than original (5)
    allocatedBytes += 8 * 1 + META_SIZE;
    //printf("Allocated memory %p\n", callocArray1);
    callocArray2 = realloc(callocArray2, 6); // Smaller than original (10)
    allocatedBytes += 6 * 1 + META_SIZE;
    //printf("Allocated memory %p\n", callocArray2);
    mallocString1 = realloc(mallocString1, 25); // Smaller than original (50)
    allocatedBytes += 25 * 1 + META_SIZE;
    mallocString2 = realloc(mallocString2, 20); // Bigger than original (10)
    allocatedBytes += 20 * 1 + META_SIZE;
    mallocString3 = realloc(mallocString3, 15); // Smaller than original (20)
    allocatedBytes += 15 * 1 + META_SIZE;
    callocString1 = realloc(callocString1, 10); // Smaller than original (20)
    allocatedBytes += 10 * 1 + META_SIZE;
    callocString2 = realloc(callocString2, 10); // Bigger than original (5)
    allocatedBytes += 10 * 1 + META_SIZE;

    printf("End of heap address: %p\n", sbrk(0));
    printf("Number of bytes allocated after realloc: %d\n", allocatedBytes);
    printf("Bytes of memory Leak: %ld\n", (sbrk(0) - startHeap) - allocatedBytes);

    // Freeing all data
    free(mallocArray1);
    free(mallocArray2);
    free(mallocArray3);
    free(mallocArray4);
    free(mallocArray5);
    free(mallocString1);
    free(mallocString2);
    free(mallocString3);
    free(mallocString4);
    free(mallocString5);

    free(callocString1);
    free(callocString2);
    free(callocString3);
    free(callocArray1);
    free(callocArray2);
    free(callocArray3);
    free(callocArray4);
    free(callocArray5);
    free(callocArray6);
    free(callocArray7);

    //printf("Heap address after freeing things up: %p\n", sbrk(0));
}
