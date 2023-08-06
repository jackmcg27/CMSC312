
/*
 * shm-client - client program to demonstrate shared memory.
 * shm-client - not sure of the origin of these code segments.
 * possibly: David Marshalls course : http://www.cs.cf.ac.uk/Dave/C/CE.html
 * or Steve Holmes : http://www2.its.strath.ac.uk/courses/c/
 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SHMSZ     27

int
main()
{
    int shmid1, shmid2;
    key_t key;
    char *shm1;
    int *shm2;

    /*
     * We need to get the segment named
     * "5678", created by the server.
     */
    key = 2752; // String

    /*
     * Locate the segment.
     */
    if( (shmid1 = shmget(key, SHMSZ, 0600)) < 0 )
    {
        perror("shmget");
        exit(1);
    }

    if( (shm1 = shmat(shmid1, NULL, 0)) == (char *) -1 )
    {
        perror("shmat");
        exit(1);
    }

    key = 2753; // int

    if( (shmid2 = shmget(key, sizeof(int), 0600)) < 0 )
    {
        perror("shmget");
        exit(1);
    }

    if( (shm2 = shmat(shmid2, NULL, 0)) == (int *) -1 )
    {
        perror("shmat");
        exit(1);
    }

    while( *shm2 != 10) {
        sleep(1);
    }
    
    sprintf(shm1, "%s", "I am Process B");
    sleep(1);
    *shm2 = 20;

    return 0;
}
