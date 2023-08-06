/*
    Name: David Adams (adamsdj2)
    V#: V00956999
    Course#: CMSC312
    Assignment: assignment-1
    Section: Question 2 -- fork.c
*/

#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

#define SHMSZ     27

int
main()
{

    int     pid1,pid2;

    /* the identifier for the shared memory segment */
    key_t   key_1, key_2;
    int     shmid_1, shmid_2;

    /* a pointer to the shared memory segment */
    char    *shm_1, *s;
    int     *shm_2;

    /*We'll name our shared memory segment */
    key_1 = 6555;
    key_2 = 1112;

    /*
     * Create the string segment.
     */
    if( (shmid_1 = shmget(IPC_PRIVATE, SHMSZ, IPC_CREAT | 0600)) < 0 )
    {
        perror("shmget");
        exit(1);
    }

    /*
     * Now we attach the segment to our data space.
     */
    if( (shm_1 = shmat(shmid_1, NULL, 0)) == (char *) -1 )
    {
        perror("shmat");
        exit(1);
    }

         /*
     * Create the integer segment.
     */
    if( (shmid_2 = shmget(key_2, SHMSZ, IPC_CREAT | 0600)) < 0 )
    {
        perror("shmget");
        exit(1);
    }

    /*
     * Now we attach the segment to our data space.
     */
    if( (shm_2 = shmat(shmid_2, NULL, 0)) == (int *) -1 )
    {
        perror("shmat");
        exit(1);
    }
    
    	//printf("shared memory segment %d attached at address %p\n", shmid_1, shm_1);
        //printf("shared memory segment %d attached at address %p\n", shmid_2, shm_2);

    /* fork another process */

    pid1 = fork();
    pid2 = fork();
    if (pid1 == 0) {		/* child process B*/
        s = shm_1;

        while(*shm_2 != 10) {
            sleep(1);
        }
        
        sprintf(s,"%s","I am Process B");
        sleep(1);
        *shm_2 = 20;
        //pause();
        exit(0);
    } 
    if (pid2 == 0) {		/* child process C*/

        s = shm_1;

        while(*shm_2 != 20)
            sleep(1);
        
        sleep(1);
        sprintf(s,"%s","I am Process C");
        sleep(2);
        *shm_2 = 30;
        sleep(1);
        exit(0);
    } 
    if((pid1 != 0) && (pid2 != 0)) 	{	/* parent process */

        s = shm_1;
        sleep(3);
        *shm_2 = 10;

        sprintf(s,"%s", "I am Process A");
        printf("%s\n", shm_1); //string

        //printf("int: %d\n", *shm_2); //integer

        /*
        * Finally, we wait until the other process 
        * changes the first character of our memory
        * to '*', indicating that it has read what 
        * we put there.
        */
        while( *shm_2 != 20 )
            sleep(1);

        printf("%s\n", shm_1); //string
        //printf("int: %d\n", *shm_2); //integer


        while( *shm_2 != 30 )
            sleep(1);

        printf("%s\n", shm_1); //string
        //printf("int: %d\n", *shm_2); //integer

        sleep(1);
        printf("GoodBye\n");

        /** now detach the shared memory segment */
        if (shmdt(shm_1) == -1) {
            fprintf(stderr, "Unable to detach\n");
        }
        /** now remove the shared memory segment */
        shmctl(shmid_1, IPC_RMID, NULL);

        /** now detach the shared memory segment */
        if ( shmdt(shm_2) == -1) {
            fprintf(stderr, "Unable to detach\n");
        }
        /** now remove the shared memory segment */
        shmctl(shmid_2, IPC_RMID, NULL);
	}

        return 0;
    
}
