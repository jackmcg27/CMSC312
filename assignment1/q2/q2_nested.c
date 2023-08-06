#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h> /* for exit */
#include <unistd.h>
#include <sys/wait.h>

/*
 * shm-server - not sure of the origin of code segment.
 * Old source, not sure of the origin
 *  possibly: David Marshalls course : http://www.cs.cf.ac.uk/Dave/C/CE.html
 * or Steve Holmes : http://www2.its.strath.ac.uk/courses/c/
 */

#define SHMSZ 27

int main()
{
    char c;
    int shmid1, shmid2;
    key_t key;
    char *shm1;
    int *shm2;
    int pid1, pid2;

    /*
     * We'll name our shared memory segment
     * "5678".
     */
    key = 2752; // Key for string

    /*
     * Create the segment.
     */
    if ((shmid1 = shmget(key, SHMSZ, IPC_CREAT | 0600)) < 0)
    {
        perror("shmget");
        exit(1);
    }

    /*
     * Now we attach the segment to our data space.
     */
    if ((shm1 = shmat(shmid1, NULL, 0)) == (char *)-1)
    {
        perror("shmat");
        exit(1);
    }

    sprintf(shm1, "%s", "I am Process A");

    key = 2753; // Key for int

    if ((shmid2 = shmget(key, sizeof(int), IPC_CREAT | 0600)) < 0)
    {
        perror("shmget");
        exit(1);
    }

    if ((shm2 = shmat(shmid2, NULL, 0)) == (int *)-1)
    {
        perror("shmat");
        exit(1);
    }

    *shm2 = 10;

    printf("%s\n", shm1);
    sleep(1);

    pid1 = fork();

    if (pid1 == 0)
    {      
         while( *shm2 != 10 ) {
            sleep(1);
        }

        sprintf(shm1, "%s", "I am Process B");
        sleep(1);
        *shm2 = 20;

        pid2 = fork();

        if (pid2 == 0) {
            while( *shm2 != 20 ) {
                sleep(1);
            }
            sleep(1);
            sprintf(shm1, "%s", "I am Process C");
            sleep(1);
            *shm2 = 30;
            //pause();
            exit(0);
        }
        //pause();
        exit(0);
    }
    else
    {
        while(*shm2 != 20) {
            sleep(1);
        }

        printf("%s\n", shm1);
        sleep(1);
        
        while(*shm2 != 30) {
            sleep(1);
        }

        printf("%s\n", shm1);
        printf("Goodbye\n");

        if (shmdt(shm1) == -1) {
            printf("String data not detached");
        }

        if (shmdt(shm2) == -1) {
            printf("Integer data not detached");
        }

        shmctl(shmid1, IPC_RMID,NULL);
        shmctl(shmid2, IPC_RMID,NULL);
    }

    



    return 0;
}
