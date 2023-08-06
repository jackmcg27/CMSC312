#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>
 
#define MAX_THREADS 20
#define SIZE 20 // Size of buffer (how many jobs it can hold at one time)

/* jobStruct:
    structure for the type job. Jobs will be added to the buffer as the producers make them
    size: number of bytes of the job -> random number between 100 and 1000
    pid: ID of the producer that created the job
    startTime: holds the time that the job was created
    endTime: holds the time that the job was dequeued from the buffer
*/
typedef struct jobStruct {
    int size;
    int pid;
    struct timespec startTime;
} job;

typedef job buffer_t;
buffer_t *buffer; // buffer where the jobs will be added to
int *buffer_index; // keeps track of where the buffer is at
int *totalJobs; // Keeps track of the total number of jobs added by the producers
int shmid1,shmid2,shmid3, shmid4, shmid5, shmid6, shmid7;
pthread_t thread[MAX_THREADS];
int numCons, numProd; // Number of consumers and number of producers
double avgTime; // Used to calculate average waiting time of jobs

pthread_mutex_t *buffer_mutex;
/* initially buffer will be empty.  full_sem
   will be initialized to buffer SIZE, which means
   SIZE number of producer threads can write to it.
   And empty_sem will be initialized to 0, so no
   consumer can read from buffer until a producer
   thread posts to empty_sem */
sem_t *full_sem;  /* when 0, buffer is full */
sem_t *empty_sem; /* when 0, buffer is empty. Kind of
                    like an index for the buffer */
sem_t *count_sem;
 
void insertbuffer(buffer_t value) {
    if (*buffer_index < SIZE) { 
        buffer[(*buffer_index)++] = value; 
    } else {
        printf("Buffer overflow\n");
    }
}
 
buffer_t dequeuebuffer() {
    buffer_t emptyJob;
    if (*buffer_index > 0) {
        int hold = *buffer_index;
        *buffer_index = --hold;
        return buffer[*buffer_index]; // buffer_index-- would be error!
    } else {
        printf("Buffer underflow\n");
    }
    return emptyJob;
}
 
 
void producer(void *thread_n) {
    //int process_numb = *(int *)thread_n; // Use this if you want to print producer IDs as 0,1,2,3,4, etc
    buffer_t value;
    int num_jobs = (rand() % 20) + 1; // 1 - 20 jobs per process
    *totalJobs += num_jobs; // add number of jobs that this proudcer will create to the total jobs counter

    // Run this for loop for as many jobs this producer will make
    for (int i = 0; i < num_jobs; i++) {
        // Generate a number between 100000 - 1000000 which is the number of microseconds (translates to 0.1 - 1 second)
        int sleepTime = (rand() % 900001) + 100000; 
        usleep(sleepTime); // sleep for sleepTime microseconds
        //value.pid = process_numb; // Make pid = 0,1,2,3,4,5, etc
        value.pid = getpid(); // Set pid to the real process ID of the system
        value.size = (rand() % 901) + 100; // 100 - 1000 bytes
        clock_gettime(CLOCK_MONOTONIC, &value.startTime); // Store the start time
        sem_wait(full_sem); // sem=0: wait. sem>0: go and decrement it
        /* possible race condition here. After this thread wakes up,
           another thread could aqcuire mutex before this one, and add to list.
           Then the list would be full again
           and when this thread tried to insert to buffer there would be
           a buffer overflow error */
        
        pthread_mutex_lock(buffer_mutex); /* protecting critical section */
        insertbuffer(value);
        printf("Producer %d added %d to buffer\n", value.pid, value.size);
        pthread_mutex_unlock(buffer_mutex);
        sem_post(empty_sem); // post (increment) emptybuffer semaphore
    }
}
 
void *consumer(void *thread_n) {
    buffer_t value;
    struct timespec endTime;

    // While loop is always true. main will cancel the threads once all jobs are completed
    while (1) {
        usleep(700000); //Sleep to show LIFO operation
        sem_wait(empty_sem);
        /* there could be race condition here, that could cause
           buffer underflow error */
        pthread_mutex_lock(buffer_mutex);
        value = dequeuebuffer();
        clock_gettime(CLOCK_MONOTONIC, &endTime);
        avgTime += (double) (endTime.tv_sec - value.startTime.tv_sec) +  (double) (endTime.tv_nsec - value.startTime.tv_nsec) / 1000000000 ;
        printf("Consumer %lu dequeue Process %d, %d bytes from buffer\n", pthread_self(), value.pid, value.size);
        pthread_mutex_unlock(buffer_mutex);
        sem_post(full_sem); // post (increment) fullbuffer semaphore
        sem_post(count_sem);
   }
    pthread_exit(0);
}

void sigintHandler(int); // prototype for ctrl+c handler for graceful termination

 
int main(int argc, char **argv) {

    // Static variables for testing
    //numCons = 4;
    //numProd = 4;

    // Ensure that correct arguments have been passed
    if (argc == 3) {
        numCons = atoi(argv[2]);
        numProd = atoi(argv[1]);
    } else {
        printf("ERROR - PLEASE PROVIDE 2 INTS\n");
        return 0;
    }
    

    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);

    signal(SIGINT, sigintHandler); // Set up the graceful termination handler

    // Set up shared memory segments
    shmid1 = shmget(2752, sizeof(buffer_t) * SIZE, IPC_CREAT | 0600); // 20 is max size of print queue
    buffer = (buffer_t*)shmat(shmid1, NULL, 0);

    shmid2 = shmget(2753, sizeof(sem_t*), IPC_CREAT | 0600);
    full_sem = (sem_t*)shmat(shmid2, NULL, 0);

    shmid3 = shmget(2754, sizeof(sem_t*), IPC_CREAT | 0600);
    empty_sem = (sem_t*)shmat(shmid3,NULL,0);

    shmid4 = shmget(2755, sizeof(int), IPC_CREAT | 0600);
    buffer_index = (int*)shmat(shmid4, NULL, 0);

    shmid5 = shmget(2756, sizeof(pthread_mutex_t*), IPC_CREAT | 0600);
    buffer_mutex = (pthread_mutex_t*)shmat(shmid5, NULL, 0);

    shmid6 = shmget(2757, sizeof(int), IPC_CREAT |0600);
    totalJobs = (int*)shmat(shmid6,NULL,0);

    shmid7 = shmget(2758, sizeof(sem_t*), IPC_CREAT |0600);
    count_sem = (sem_t*)shmat(shmid7, NULL, 0);

    // Initialize variables
    *buffer_index = 0;
    *totalJobs = 0;
    avgTime = 0;

    // Initialize semaphores
    pthread_mutex_init(buffer_mutex, NULL);
    sem_init(full_sem, // sem_t *sem
             1, // int pshared. 0 = shared between threads of process,  1 = shared between processes
             SIZE); // unsigned int value. Initial value
    sem_init(empty_sem,
             1,
             0);
    sem_init(count_sem,
             1,
             0);
    /* full_sem is initialized to buffer size because SIZE number of
       producers can add one element to buffer each. They will wait
       semaphore each time, which will decrement semaphore value.
       empty_sem is initialized to 0, because buffer starts empty and
       consumer cannot take any element from it. They will have to wait
       until producer posts to that semaphore (increments semaphore
       value) */

    int thread_numb[numCons];

    // Create processes for producers
    for (int i = 0; i < numProd; i++) {
        if (fork() == 0) {
            srand(getpid()); // seed the randomizer to get different size jobs
            producer(&i);
            exit(0);
        }
    }

    // Create threads for consumers
    for (int i = 0; i < numCons; i++) {
        thread_numb[i] = i;
        pthread_create(&thread[i], // pthread_t *t
                       NULL, // const pthread_attr_t *attr
                       consumer, // void *(*start_routine) (void *)
                       &thread_numb[i]);  // void *arg
    }

    // Waiting for all processes to complete
    for (int j = 0; j < numProd; j++) {
        waitpid(-1, NULL, 0); // -1 as first argument means it will wait for any child process to complete
    }

    int temp2 = 0;
    
    // Check to see if all jobs have been dequeued
    while(temp2 != *totalJobs) {
        sem_getvalue(count_sem, &temp2);
    }
 
    // Cancel all threads since all jobs are completed
    for (int i = 0; i < numCons; i++) {
        pthread_cancel(thread[i]);
        pthread_join(thread[i], NULL);
    }

    printf("Total jobs completed: %d\n", *totalJobs);
    printf("Average time for a job to be completed: %f seconds\n", avgTime / (double)(*totalJobs));

    // Clean up shared memory
    pthread_mutex_destroy(buffer_mutex);
    sem_destroy(full_sem);
    sem_destroy(empty_sem);
    sem_destroy(count_sem);

    if (shmdt(buffer) == -1) {
        printf("buffer data not detached\n");
    }
    if (shmdt(full_sem) == -1) {
        printf("full_sem data not detached\n");
    }
    if (shmdt(empty_sem) == -1) {
        printf("empty_sem data not detached\n");
    }
    if (shmdt(buffer_index) == -1) {
        printf("buffer_index data not detached\n");
    }
    if (shmdt(buffer_mutex) == -1) {
        printf("buffer_mutex data not detached\n");
    }
    if (shmdt(totalJobs) == -1) {
        printf("totalJobs data not detached\n");
    }
    if (shmdt(count_sem) == -1) {
        printf("count_sem data not detached\n");
    }

    shmctl(shmid1, IPC_RMID,NULL);
    shmctl(shmid2, IPC_RMID,NULL);
    shmctl(shmid3, IPC_RMID,NULL);
    shmctl(shmid4,IPC_RMID,NULL);
    shmctl(shmid5, IPC_RMID, NULL);
    shmctl(shmid6, IPC_RMID, NULL); 
    shmctl(shmid7, IPC_RMID, NULL);

    // Calculate total run time
    double totalTime;
    clock_gettime(CLOCK_MONOTONIC, &stop);
    totalTime = (stop.tv_sec - start.tv_sec) + (double) (stop.tv_nsec - start.tv_nsec) / 1000000000;
    printf("The total execution time was %f seconds\n", totalTime);

    return 0;
}

void sigintHandler(int sig_num) {

    sleep(3);

    // Cancel threads
    for (int i = 0; i < numCons; i++) {
        pthread_cancel(thread[i]);
        pthread_join(thread[i], NULL);
    }
    printf("Ctrl C -- Clean exit\n");

    int temp;
    sem_getvalue(count_sem, &temp);

    printf("Total jobs completed: %d of %d\n",temp, *totalJobs);

    // Clean up memory
    pthread_mutex_destroy(buffer_mutex);
    sem_destroy(full_sem);
    sem_destroy(empty_sem);
    sem_destroy(count_sem);

    shmdt(buffer);
    shmdt(full_sem);
    shmdt(empty_sem);
    shmdt(buffer_index);
    shmdt(buffer_mutex);
    shmdt(totalJobs);
    shmdt(count_sem);
    shmctl(shmid1, IPC_RMID,NULL);
    shmctl(shmid2, IPC_RMID,NULL);
    shmctl(shmid3, IPC_RMID,NULL);
    shmctl(shmid4,IPC_RMID,NULL);
    shmctl(shmid5, IPC_RMID, NULL);
    shmctl(shmid6, IPC_RMID, NULL); 
    shmctl(shmid7, IPC_RMID, NULL);

    exit(0);
}
