#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Initialize the mutex
pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;

// Initialize the conditional variables
pthread_cond_t myCond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t myCond2 = PTHREAD_COND_INITIALIZER;

//counting variable 
int myCount = 0;

void *producer(void *args)
{   
    int checkCount = 0;
    while (1)
    { 
        pthread_mutex_lock(&myMutex); 
        printf("PRODUCER: myMutex locked\n");
        
        if(checkCount == myCount)
        {
            //if myCount is not increase, wait for it to increase.
            printf("PRODUCER: waiting on myCond1\n");
            pthread_cond_wait(&myCond1, &myMutex);
        }
        //increament the checkCount as the myCount is increased
        checkCount += 1;
        printf("PRODUCER: myCount: %d -> %d\n", myCount-1, myCount);

        if(myCount == 10)
            break;
        pthread_mutex_unlock(&myMutex);
        printf("PRODUCER: myMutex unlocked\n");
        pthread_cond_signal(&myCond2);
        printf("PRODUCER: signaling myCond2\n");

    }
    return NULL;
}

void *consumer(void *args)
{
    while (1)
    { 
        pthread_mutex_lock(&myMutex); // Lock the mutex before checking if the buffer has data
        printf("CONSUMER: myMutex locked\n");
        
        myCount += 1;

        if(myCount == 10)
        {
            pthread_mutex_unlock(&myMutex);
            printf("CONSUMER: myMutex unlocked\n");
            break;
        }
        pthread_mutex_unlock(&myMutex);
        printf("CONSUMER: myMutex unlocked\n");

        pthread_cond_signal(&myCond1);
        printf("CONSUMER: signaling myCond1\n");
    }
    return NULL;
}

int main(int argc, char *argv[]){
    printf("PROGRAM START\n");
    pthread_t pThread, cThread;
    pthread_create(&pThread, NULL, producer, NULL);
    pthread_create(&cThread, NULL, consumer, NULL);
    printf("CONSUMER THREAD CREATED\n");
    
    pthread_join(pThread, NULL);
    pthread_join(cThread, NULL);
    printf("PROGRAM END\n");
    return 0;
}