#include "memory_management.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> // Including this so standard functions like print use regular malloc

// void main() {

//     int allocatedBytes = 0;
//     void *startHeap = sbrk(0);
//     printf("Starting heap address: %p\n", sbrk(0));

//     void *mallocNewArray1 = mallocNew(20 * 1);
//     //printf("Allocated memory %p\n", mallocNewArray1);
//     //allocatedBytes += 5 * 1 + META_SIZE;

//     void *mallocNewArray2 = mallocNew(10 * 1);
//     //printf("Allocated memory %p\n", mallocNewArray2);
//     //allocatedBytes += 10 * 1 + META_SIZE;

//     void *mallocNewArray3 = mallocNew(8 * 1);
//     //printf("Allocated memory %p\n", mallocNewArray3);  
//     //allocatedBytes += 8 * 1 + META_SIZE;

//     void *mallocNewArray4 = mallocNew(15 * 1);
//     //printf("Allocated memory %p\n", mallocNewArray4);
//     //allocatedBytes += 15 * 1 + META_SIZE;

//     freeNew(mallocNewArray1);
//     freeNew(mallocNewArray2);
//     freeNew(mallocNewArray3);
//     freeNew(mallocNewArray4);

//     void *mallocNewArray5 = mallocNew(3 * 1);
//     //printf("Allocated memory %p\n", mallocNewArray5);
//     //allocatedBytes += 3 * 1 + META_SIZE;

//     void *mallocNewString1 = mallocNew(50 * 1);
//     //printf("Allocated memory %p\n", mallocNewString1);
//     //allocatedBytes += 50 * 1 + META_SIZE;

//     void *mallocNewString2 = mallocNew(10 * 1);
//     //printf("Allocated memory %p\n", mallocNewString2);
//     //allocatedBytes += 10 * 1 + META_SIZE;

//     void *mallocNewString3 = mallocNew(20 * 1);
//     //printf("Allocated memory %p\n", mallocNewString3);
//     //allocatedBytes += 20 * 1 + META_SIZE;

//     void *mallocNewString4 = mallocNew(25 * 1);
//     //printf("Allocated memory %p\n", mallocNewString4);
//     //allocatedBytes += 25 * 1 + META_SIZE;

//     void *mallocNewString5 = mallocNew(9 * 1);
//     //printf("Allocated memory %p\n", mallocNewString5);
//    //allocatedBytes += 9 * 1 + META_SIZE;

//     // Calling callocNew 10 times
//     void *callocNewString1 = callocNew(20, 1);
//     //printf("Allocated memory %p\n", callocNewString1);
//     //allocatedBytes += 20 * 1 + META_SIZE;

//     void *callocNewString2 = callocNew(5, 1);
//     //printf("Allocated memory %p\n", callocNewString2);
//     //allocatedBytes += 5 * 1 + META_SIZE;

//     void *callocNewString3 = callocNew(10, 1);
//     //printf("Allocated memory %p\n", callocNewString3);
//     //allocatedBytes += 10 * 1 + META_SIZE;

//     void *callocNewArray1 = callocNew(5, 1);
//     //printf("Allocated memory %p\n", callocNewArray1);
//     //allocatedBytes += 5 * 1 + META_SIZE;

//     void *callocNewArray2 = callocNew(10, 1);
//     //printf("Allocated memory %p\n", callocNewArray2);
//     //allocatedBytes += 10 * 1 + META_SIZE;

//     void *callocNewArray3 = callocNew(8, 1);
//     //printf("Allocated memory %p\n", callocNewArray3);
//     //allocatedBytes += 8 * 1 + META_SIZE;

//     void *callocNewArray4 = callocNew(15, 1);
//     //printf("Allocated memory %p\n", callocNewArray4);
//     //allocatedBytes += 15 * 1 + META_SIZE;

//     void *callocNewArray5 = callocNew(3, 1);
//     //printf("Allocated memory %p\n", callocNewArray5);
//     //allocatedBytes += 3 * 1 + META_SIZE;

//     void *callocNewArray6 = callocNew(7, 1); 
//     //printf("Allocated memory %p\n", callocNewArray6);
//     //allocatedBytes += 7 * 1 + META_SIZE;

//     void *callocNewArray7 = callocNew(6, 1);
//     //printf("Allocated memory %p\n", callocNewArray7);
//     //allocatedBytes += 6 * 1 + META_SIZE;

//     // Calling reallocNew 10 times
//     // mallocNewArray1 = reallocNew(mallocNewArray1, 10); // Bigger than original (5)
//     // //printf("Allocated memory %p\n", mallocNewArray1);
//     // allocatedBytes += 10 * 1 + META_SIZE;
//     // mallocNewArray2 = reallocNew(mallocNewArray2, 5); // Smaller than original (10)
//     // allocatedBytes += 5 * 1 + META_SIZE;
//     // //printf("Allocated memory %p\n", mallocNewArray2);
//     // mallocNewArray3 = reallocNew(mallocNewArray3, 4); // Smaller than original (8)
//     // allocatedBytes += 4 * 1 + META_SIZE;
//     //printf("Allocated memory %p\n", mallocNewArray3);
//     callocNewArray1 = reallocNew(callocNewArray1, 8); // Bigger than original (5)
//     //allocatedBytes += 8 * 1 + META_SIZE;
//     //printf("Allocated memory %p\n", callocNewArray1);
//     callocNewArray2 = reallocNew(callocNewArray2, 6); // Smaller than original (10)
//     //allocatedBytes += 6 * 1 + META_SIZE;
//     //printf("Allocated memory %p\n", callocNewArray2);
//     mallocNewString1 = reallocNew(mallocNewString1, 25); // Smaller than original (50)
//     //allocatedBytes += 25 * 1 + META_SIZE;
//     mallocNewString2 = reallocNew(mallocNewString2, 20); // Bigger than original (10)
//     //allocatedBytes += 20 * 1 + META_SIZE;
//     mallocNewString3 = reallocNew(mallocNewString3, 15); // Smaller than original (20)
//     //allocatedBytes += 15 * 1 + META_SIZE;
//     callocNewString1 = reallocNew(callocNewString1, 10); // Smaller than original (20)
//     //allocatedBytes += 10 * 1 + META_SIZE;
//     callocNewString2 = reallocNew(callocNewString2, 10); // Bigger than original (5)
//     //allocatedBytes += 10 * 1 + META_SIZE;

//     printf("End of heap address: %p\n", sbrk(0));
//     printf("End - start: %ld\n", sbrk(0) - startHeap);
//     //printf("Number of bytes allocated after reallocNew: %d\n", allocatedBytes);
//     printf("Bytes of memory Leak: %ld\n", (sbrk(0) - startHeap) - calculate_bytes_allocated());

//     // freeNewing all data
//     // freeNew(mallocNewArray1);
//     // freeNew(mallocNewArray2);
//     // freeNew(mallocNewArray3);
//     // freeNew(mallocNewArray4);
//     freeNew(mallocNewArray5);
//     freeNew(mallocNewString1);
//     freeNew(mallocNewString2);
//     freeNew(mallocNewString3);
//     freeNew(mallocNewString4);
//     freeNew(mallocNewString5);

//     freeNew(callocNewString1);
//     freeNew(callocNewString2);
//     freeNew(callocNewString3);
//     freeNew(callocNewArray1);
//     freeNew(callocNewArray2);
//     freeNew(callocNewArray3);
//     freeNew(callocNewArray4);
//     freeNew(callocNewArray5);
//     freeNew(callocNewArray6);
//     freeNew(callocNewArray7);
// }
void main() {
     void *startHeap = sbrk(0);
    
// Testing mallocNew
    int *ptr1 = (int*) mallocNew(sizeof(int) * 50);
    int *ptr2 = (int*) mallocNew(sizeof(int) * 25);
    int *ptr3 = (int*) mallocNew(sizeof(int) * 15);
    int *ptr4 = (int*) mallocNew(sizeof(int) * 10);
    int *ptr5 = (int*) mallocNew(sizeof(int) * 5);


    freeNew(ptr3);
    freeNew(ptr2);
    freeNew(ptr1);
    //freeNew(ptr4);
    //freeNew(ptr5);

    int *ptr6 = (int*) mallocNew(sizeof(int) * 5);
    int *ptr7 = (int*) mallocNew(sizeof(int) * 10);
    int *ptr8 = (int*) mallocNew(sizeof(int) * 15);
    int *ptr9 = (int*) mallocNew(sizeof(int) * 25);
    int *ptr10 = (int*) mallocNew(sizeof(int) * 50);

    ptr6 = reallocNew(ptr6, sizeof(int) * 6);
    ptr7 = reallocNew(ptr7, sizeof(int) * 11);
    ptr8 = reallocNew(ptr8, sizeof(int) * 16);
    ptr9 = reallocNew(ptr9, sizeof(int) * 26);
    ptr10 = reallocNew(ptr10, sizeof(int) * 51);


    freeNew(ptr10);
    freeNew(ptr7);
    freeNew(ptr9);
    freeNew(ptr8);
    freeNew(ptr6);

    // Test callocNew
    int *ptr11 = (int*) callocNew(50, sizeof(int) * 10);
    int *ptr12 = (int*) callocNew(2, sizeof(int) * 9);
    int *ptr13 = (int*) callocNew(2, sizeof(int) * 8);
    int *ptr14 = (int*) callocNew(2, sizeof(int) * 7);
    int *ptr15 = (int*) callocNew(2, sizeof(int) * 6);

    freeNew(ptr15);
    freeNew(ptr12);
    freeNew(ptr14);
    freeNew(ptr13);
    freeNew(ptr11);

    int *ptr16 = (int*) callocNew(2, sizeof(int) * 6);
    int *ptr17 = (int*) callocNew(2, sizeof(int) * 7);
    int *ptr18 = (int*) callocNew(2, sizeof(int) * 8);
    int *ptr19 = (int*) callocNew(2, sizeof(int) * 9);
    int *ptr20 = (int*) callocNew(50, sizeof(int) * 10);

    ptr16 = reallocNew(ptr16, sizeof(int) * 7);
    ptr17 = reallocNew(ptr17, sizeof(int) * 8);
    ptr18 = reallocNew(ptr18, sizeof(int) * 9);
    ptr19 = reallocNew(ptr19, sizeof(int) * 10);
    ptr20 = reallocNew(ptr20, sizeof(int) * 11);

    freeNew(ptr20);
    freeNew(ptr17);
    freeNew(ptr18);
    freeNew(ptr19);
    freeNew(ptr16);
    
    void *endHeap = sbrk(0);
    int bytesAllocated = calculate_bytes_allocated();
    
    printf("Start of heap address: %p\n", startHeap);
    printf("End of heap address: %p\n", sbrk(0));
    printf("End - start: %ld\n", endHeap - startHeap);
    printf("Bytes of memory Leak: %ld\n", (endHeap - startHeap) - calculate_bytes_allocated());
}