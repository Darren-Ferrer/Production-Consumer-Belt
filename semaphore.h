#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <semaphore.h>
#include <pthread.h>
#include <fcntl.h>

struct Semaphore
{
    int buffer[10];
    pthread_mutex_t mutex;
    sem_t empty;
    sem_t full;
    sem_t barrier;
    sem_t three_cfb;
};

#endif