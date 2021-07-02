#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <time.h>
#include "queue.h"
#include "semaphore.h"
#include "io.h"

using namespace std;


struct Consumer
{
    unsigned int Lucy_box_time = 0;
    unsigned int Ethel_box_time = 0;
    int eaten  = 0;
    int max_eaten = 100;

    Queue *belt;
    Semaphore *sem;
};


void * L (void * voidptr)
{
    Consumer *consumers = (Consumer *) voidptr;
    timespec tp;
    tp.tv_nsec = (consumers->Lucy_box_time % 1000) * 1000000;
    tp.tv_sec = 0;
    
    int eaten;

    while (consumers->eaten != consumers->max_eaten)
    {
        sem_wait(&consumers->sem->full);
        pthread_mutex_lock(&consumers->sem->mutex);

        eaten = consumers->belt->remove();
        //decrements number of the type of eaten candy on the belt
        consumers->belt->on_belt[eaten] -= 1;

        // increments the number of type of candy eaten
        consumers->belt->type_consumed_lucy[eaten] += 1;
        consumers->eaten += 1;

        // prints out the type of that candy has been eaten and who ate it
        io_remove_type(Lucy, (ProductType) eaten, consumers->belt->on_belt, consumers->belt->type_consumed_lucy);

        pthread_mutex_unlock(&consumers->sem->mutex);
        sem_post(&consumers->sem->empty);

        // if the eaten candy was a frog bite, decrement semaphore
        if (eaten == FrogBite)
        {
            sem_post(&consumers->sem->three_cfb);
        }
        
        nanosleep(&tp, NULL);
    }
    sem_post(&consumers->sem->barrier);
    return NULL;
}

void * E (void * voidptr)
{
    Consumer *consumers = (Consumer *) voidptr;
    timespec tp;
    tp.tv_nsec = (consumers->Ethel_box_time % 1000) * 1000000;
    tp.tv_sec = 0;
    
    int eaten;

    while (consumers->eaten != consumers->max_eaten)
    {
        sem_wait(&consumers->sem->full);
        pthread_mutex_lock(&consumers->sem->mutex);

        eaten = consumers->belt->remove();
        //decrements number of the type of eaten candy on the belt
        consumers->belt->on_belt[eaten] -= 1;

        // increments the type of candy consumed
        consumers->belt->type_consumed_ethel[eaten] += 1;
        consumers->eaten += 1;

        // prints out the type of that candy has been eaten and who ate it
        io_remove_type(Ethel, (ProductType) eaten, consumers->belt->on_belt, consumers->belt->type_consumed_ethel);

        pthread_mutex_unlock(&consumers->sem->mutex);
        sem_post(&consumers->sem->empty);

        // if the eaten candy was a frog bite, decrement semaphore
        if (eaten == FrogBite)
        {
            sem_post(&consumers->sem->three_cfb);
        }
        
        nanosleep(&tp, NULL);
    }
    sem_post(&consumers->sem->barrier);
    return NULL;
}
