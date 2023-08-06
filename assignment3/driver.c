#include "memory_management.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> // Including this so standard functions like print use regular malloc

void main() {

     void *startHeap = sbrk(0);
    
    // Testing mallocNew (6 times then free some then 4 more)
    int *ptr1 = (int*) mallocNew(sizeof(int) * 20);
    int *ptr2 = (int*) mallocNew(sizeof(int) * 10);
    int *ptr3 = (int*) mallocNew(sizeof(int) * 8);
    int *ptr4 = (int*) mallocNew(sizeof(int) * 15);
    int *ptr5 = (int*) mallocNew(sizeof(int) * 3);
    int *ptr6 = (int*) mallocNew(sizeof(int) * 50);

    freeNew(ptr1);
    freeNew(ptr2);
    freeNew(ptr3);
    //freeNew(ptr4);
    //freeNew(ptr5);
    //freeNew(ptr6)

    int *ptr7 = (int*) mallocNew(sizeof(int) * 8);
    int *ptr8 = (int*) mallocNew(sizeof(int) * 2);
    int *ptr9 = (int*) mallocNew(sizeof(int) * 9);
    int *ptr10 = (int*) mallocNew(sizeof(int) * 20);

    // Testing reallocNew (5 times then free some)
    ptr6 = reallocNew(ptr6, sizeof(int) * 7);
    ptr7 = reallocNew(ptr7, sizeof(int) * 6);
    ptr8 = reallocNew(ptr8, sizeof(int) * 3);
    ptr9 = reallocNew(ptr9, sizeof(int) * 12);
    ptr10 = reallocNew(ptr10, sizeof(int) * 18);

    freeNew(ptr6);
    freeNew(ptr7);
    freeNew(ptr8);
    //freeNew(ptr9);
    //freeNew(ptr10);

    // Test callocNew (5 times then free then 5 more)
    int *ptr11 = (int*) callocNew(2, sizeof(int) * 9);
    int *ptr12 = (int*) callocNew(20, sizeof(int) * 17);
    int *ptr13 = (int*) callocNew(12, sizeof(int) * 20);
    int *ptr14 = (int*) callocNew(9, sizeof(int) * 4);
    int *ptr15 = (int*) callocNew(7, sizeof(int) * 8);

    freeNew(ptr11);
    freeNew(ptr12);
    freeNew(ptr13);
    freeNew(ptr14);
    freeNew(ptr15);

    int *ptr16 = (int*) callocNew(2, sizeof(int) * 9);
    int *ptr17 = (int*) callocNew(7, sizeof(int) * 6);
    int *ptr18 = (int*) callocNew(4, sizeof(int) * 12);
    int *ptr19 = (int*) callocNew(5, sizeof(int) * 8);
    int *ptr20 = (int*) callocNew(25, sizeof(int) * 4);

    // Testing reallocNew (5 times then free 3)
    ptr16 = reallocNew(ptr16, sizeof(int) * 5);
    ptr17 = reallocNew(ptr17, sizeof(int) * 2);
    ptr18 = reallocNew(ptr18, sizeof(int) * 13);
    ptr19 = reallocNew(ptr19, sizeof(int) * 8);
    ptr20 = reallocNew(ptr20, sizeof(int) * 6);

    freeNew(ptr16);
    freeNew(ptr17);
    freeNew(ptr18);
    freeNew(ptr19);
    freeNew(ptr20);
    
    void *endHeap = sbrk(0); // Store the end heap address
    int bytesAllocated = calculate_bytes_allocated(); // Determine how many bytes are storing data

    // Print reports    
    printf("Start of heap address: %p\n", startHeap);
    printf("End of heap address: %p\n", sbrk(0));
    printf("Bytes of memory Leak: %ld\n", (endHeap - startHeap) - calculate_bytes_allocated());
}