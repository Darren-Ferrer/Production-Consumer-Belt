#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <time.h>
#include "queue.h"
#include "io.h"
#include "semaphore.h"

using namespace std;

//ProductType are producers, 0 for frog_bites, 1 for escargot_suckers

struct Producer
{
    unsigned int frog_bite = 0;
    unsigned int escargot_sucker = 0;
    int produced = 0;
    int max_produced = 100;
    int frogs_produced = 0;
    int escargot_produced = 0;

    // identifiers for the queue
    int f = 0;
    int e = 1;

    Queue *belt;
    Semaphore *sem;
};

// will use to add candies onto the belt
void * CFB (void * voidptr)
{
    Producer *products = (Producer *) voidptr;

    timespec tp;
    tp.tv_nsec = (products->frog_bite % 1000) * 1000000;
    tp.tv_sec = 0;

    // executes forever
    while (products->produced != products->max_produced)
    {
        if (products->belt->on_belt[0] == 3)
        {
            sem_wait(&products->sem->three_cfb);
        }
        else
        {
            // if the belt is full, wait until there is room
            sem_wait(&products->sem->empty);
            pthread_mutex_lock(&products->sem->mutex);
            
            // adds a frog bite to the belt
            products->belt->add(products->f);

            // increments number of cfb produced
            products->frogs_produced += 1;
            products->belt->type_produced[FrogBite] += 1;

            // increments total produced
            products->produced += 1;
            // increments number of the type of candy created on the belt
            products->belt->on_belt[FrogBite] += 1;

            // prints out cfb is being added to belt
            io_add_type(FrogBite, products->belt->on_belt, products->belt->type_produced);

            pthread_mutex_unlock(&products->sem->mutex);
            sem_post(&products->sem->three_cfb);
            sem_post(&products->sem->full);

            nanosleep(&tp, NULL);
        }

        //cout << "Num produced: " << products->produced << endl;
        
    }
    return NULL;
}

void * EES (void * voidptr)
{
    Producer *products = (Producer *) voidptr;

    timespec tp;
    tp.tv_nsec = (products->escargot_sucker % 1000) * 1000000;
    tp.tv_sec = 0;

    // executes forever
    while (products->produced != products->max_produced)
    {
        
        sem_wait(&products->sem->empty);
        pthread_mutex_lock(&products->sem->mutex);

        // adds an escargot on the belt
        products->belt->add(products->e);

        // increments number of ees produced
        products->escargot_produced += 1;
        products->belt->type_produced[Escargot] += 1;

        // increments total produced
        products->produced += 1;

        // increments number of the type of candy created on the belt
        products->belt->on_belt[Escargot] += 1;

        // prints out ees is being added to the belt
        io_add_type(Escargot, products->belt->on_belt, products->belt->type_produced);

        // cout << "num Produced: " << products->produced << endl;

        pthread_mutex_unlock(&products->sem->mutex);
        sem_post(&products->sem->full);

        nanosleep(&tp, NULL);
    
    }


    return NULL;
}
