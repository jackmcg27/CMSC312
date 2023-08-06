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
 
#define SIZE 20 // Size of buffer (how many jobs it can hold at one time)
#define MAX_THREADS 20 

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

/* queueStruct
    structure for the type queue -> uses semaphores to keep track of index of front and rear of circular queue as well as number of items in the queue and the max size of the queue
    size: semaphore that hodls the max size of the queue
    numEntries: semaphore that keeps track of how many jobs are in the queue
    head: keeps track of the front of the queue for where to remove the next job from
    rear: keeps track of the back of the queue for where to add the next job to
    buffer: array that holds all of the jobs that are in the queue
*/
typedef struct queueStruct {
    sem_t *numEntries, *head, *rear;
    buffer_t *buffer;
} queue;

queue *globalQueue;
int *totalJobs; // Keeps track of the total number of jobs added by the producers
int shmid1, shmid2, shmid3, shmid4, shmid5, shmid6, shmid7, shmid8, shmid9;

pthread_t thread[MAX_THREADS]; // Holds thread IDs
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

int queue_empty() {
    int temp;
    sem_getvalue(globalQueue->numEntries, &temp);
    return (temp == 0); // if number of items in the queue is 0
}

int queue_full() {
    int temp;
    sem_getvalue(globalQueue->numEntries, &temp);
    return(temp == SIZE); // if number of items in the queue = the size of the queue
}

void insertbuffer(buffer_t value) {
    if (queue_full()) {
        printf("Buffer overflow\n"); 
    } else {
        int temp;
        sem_getvalue(globalQueue->rear, &temp);
        globalQueue->buffer[temp % SIZE] = value; // index into the buffer using the rear index and place the new job there
        // NOTE: rear will only be posted (incremented) so % 20 will give the accurate index
        // increment numEntires and rear
        sem_post(globalQueue->numEntries);
        sem_post(globalQueue->rear);
    }
}
 
buffer_t dequeuebuffer() {
    buffer_t tempJob;
    if (queue_empty()) {
        printf("Buffer underflow\n");
        return tempJob;
    } else {
        int temp;
        sem_getvalue(globalQueue->head, &temp);
        tempJob = globalQueue->buffer[temp % 20]; // index into the buffer using the head index and return that job
        // NOTE: head will only be posted (incremented) so % 20 will give the accurate index
        sem_post(globalQueue->head); // Increment the head
        sem_wait(globalQueue->numEntries); // decrement the numEntries since 1 has been removed from the queue
        return tempJob;
    }
}
 
 
void producer(void *thread_n) {
    //int process_numb = *(int *)thread_n; // Use this if you want to print producer IDs as 0,1,2,3,4, etc
    buffer_t value;
    int num_jobs = (rand() % 20) + 1; // 1 - 20 jobs per process
    *totalJobs += num_jobs; // add number of jobs that this proudcer will create to the total jobs counter

    // Run this for loop for as many jobs this producer will make
    for (int i = 0; i < num_jobs; i++) {
        int sleepTime = (rand() % 900001) + 100000; // Generate a number between 100000 - 1000000 which is the number of microseconds (translates to 0.1 - 1 second)
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
        usleep(700000); //Sleep to show FIFO operation
        sem_wait(empty_sem);
        /* there could be race condition here, that could cause
           buffer underflow error */
        pthread_mutex_lock(buffer_mutex);
        value = dequeuebuffer();
        clock_gettime(CLOCK_MONOTONIC, &endTime); // Store the time in endTime now that the job has been completed
        avgTime += (endTime.tv_sec - value.startTime.tv_sec) +  (double) (endTime.tv_nsec - value.startTime.tv_nsec) / 1000000000; //calculate the time passed and add it to the avgTime
        printf("Consumer %lu dequeue Process %d, %d from buffer\n", pthread_self(), value.pid, value.size);
        pthread_mutex_unlock(buffer_mutex);
        sem_post(full_sem); // post (increment) fullbuffer semaphore
   }
    pthread_exit(0);
}
void sigintHandler(int); // prototype for ctrl+c handler for graceful termination

 
int main(int argc, char **argv) {
    
    // Ensure that correct arguments have been passed
    if (argc == 3) {
        numProd = atoi(argv[1]);
        numCons = atoi(argv[2]);
    } else {
        printf("ERROR - PLEASE PROVIDE 2 INTS\n");
        return 0;
    }
    

    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Static variables for testing
    //numCons = 2;
    //numProd = 2;

    signal(SIGINT, sigintHandler); // Set up the graceful termination handler

    // Set up shared memory segments
    shmid1 = shmget(2752, sizeof(queue), IPC_CREAT | 0600);
    globalQueue = (queue*)shmat(shmid1, NULL, 0);

    shmid2 = shmget(2753, sizeof(buffer_t) * SIZE, IPC_CREAT | 0600); // 20 is max size of print queue
    globalQueue->buffer = (buffer_t*)shmat(shmid2, NULL, 0);

    shmid3 = shmget(2754, sizeof(sem_t), IPC_CREAT | 0600);
    globalQueue->head = (sem_t*)shmat(shmid3, NULL, 0);

    shmid4 = shmget(2755, sizeof(sem_t), IPC_CREAT | 0600);
    globalQueue->rear = (sem_t*)shmat(shmid4, NULL, 0);

    shmid5 = shmget(2757, sizeof(sem_t), IPC_CREAT |0600);
    globalQueue->numEntries= (sem_t*)shmat(shmid5, NULL, 0);

    shmid6 = shmget(2758, sizeof(sem_t*), IPC_CREAT | 0600);
    full_sem = (sem_t*)shmat(shmid6, NULL, 0);

    shmid7 = shmget(2759, sizeof(sem_t*), IPC_CREAT | 0600);
    empty_sem = (sem_t*)shmat(shmid7,NULL,0);  

    shmid8 = shmget(2760, sizeof(pthread_mutex_t*), IPC_CREAT | 0600);
    buffer_mutex = (pthread_mutex_t*)shmat(shmid8, NULL, 0);

    shmid9 = shmget(2761, sizeof(int), IPC_CREAT |0600);
    totalJobs = (int*)shmat(shmid9,NULL,0);

    // Initalize variables
    *totalJobs = 0;
    avgTime = 0;

    // Initialize semaphores
    sem_init(globalQueue->head, 1, 0);
    sem_init(globalQueue->rear, 1, 0);
    sem_init(globalQueue->numEntries, 1, 0);

    pthread_mutex_init(buffer_mutex, NULL);
    sem_init(full_sem, // sem_t *sem
             1, // int pshared. 0 = shared between threads of process,  1 = shared between processes
             SIZE); // unsigned int value. Initial value
    sem_init(empty_sem,
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

    int temp2;

    // Check to see if all jobs have been dequeued. If the head is equal to the totalJobs, then all jobs have been removed
    while(temp2 != *totalJobs) {
        sem_getvalue(globalQueue->head, &temp2);
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
    sem_destroy(globalQueue->head);
    sem_destroy(globalQueue->rear);
    sem_destroy(globalQueue->numEntries);

    shmdt(full_sem);
    shmdt(empty_sem);
    shmdt(buffer_mutex);
    shmdt(totalJobs);

    shmdt(globalQueue->buffer);
    shmdt(globalQueue->head);
    shmdt(globalQueue->rear);
    shmdt(globalQueue->numEntries);
    shmdt(globalQueue);

    shmctl(shmid1, IPC_RMID,NULL);
    shmctl(shmid2, IPC_RMID,NULL);
    shmctl(shmid3, IPC_RMID,NULL);
    shmctl(shmid4,IPC_RMID,NULL);
    shmctl(shmid5,IPC_RMID,NULL);
    shmctl(shmid6, IPC_RMID, NULL); 
    shmctl(shmid7, IPC_RMID, NULL); 
    shmctl(shmid8, IPC_RMID, NULL);
    shmctl(shmid9, IPC_RMID, NULL);

    // Calculate total run time
    double totalTime;
    clock_gettime(CLOCK_MONOTONIC, &stop);
    totalTime = (stop.tv_sec - start.tv_sec) + (double) (stop.tv_nsec - start.tv_nsec) / 1000000000;
    printf("The total execution time was %f seconds\n", totalTime);
    
    return 0;
}

void sigintHandler(int sig_num) {

    printf("Ctrl C -- Clean exit\n");

    sleep(3);

    // Cancel threads
    for (int i = 0; i < numCons; i++) {
        pthread_cancel(thread[i]);
        pthread_join(thread[i], NULL);
    }

    int temp;
    sem_getvalue(globalQueue->head, &temp);
    printf("Total jobs completed: %d of %d\n",temp, *totalJobs);

    // Clean up memory
    pthread_mutex_destroy(buffer_mutex);
    sem_destroy(full_sem);
    sem_destroy(empty_sem);
    sem_destroy(globalQueue->head);
    sem_destroy(globalQueue->rear);
    sem_destroy(globalQueue->numEntries);

    shmdt(full_sem);
    shmdt(empty_sem);
    shmdt(buffer_mutex);
    shmdt(totalJobs);

    shmdt(globalQueue->buffer);
    shmdt(globalQueue->head);
    shmdt(globalQueue->rear);
    shmdt(globalQueue->numEntries);
    shmdt(globalQueue);

    shmctl(shmid1, IPC_RMID,NULL);
    shmctl(shmid2, IPC_RMID,NULL);
    shmctl(shmid3, IPC_RMID,NULL);
    shmctl(shmid4,IPC_RMID,NULL);
    shmctl(shmid5, IPC_RMID, NULL);
    shmctl(shmid6, IPC_RMID, NULL); 
    shmctl(shmid7, IPC_RMID, NULL); 
    shmctl(shmid8, IPC_RMID, NULL);
    shmctl(shmid9, IPC_RMID, NULL);
    

    exit(0);
}
