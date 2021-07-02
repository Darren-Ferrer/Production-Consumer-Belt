/*
We the undersigned promise that we have in good faith attempted to follow the principles of pair programming. 
Although we were free to discuss ideas with others, the implementation is our own. 
We have shared a common workspace and taken turns at the keyboard for the majority of the work that we are submitting. 
Furthermore, any non programming portions of the assignment were done independently. 
We recognize that should this not be the case, we will be subject to penalties as outlined in the course syllabus.

Late penalty waived as per e-mail with Professor Roch, April 8, 7:53 PM

Darren Ferrer
821644649

Ethan Ha
821685521
*/

#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include "consumer.h"
#include "producer.h"
#include "production.h"
#include <semaphore.h>
#include "semaphore.h"

using namespace std;

int main(int argc, char* argv[])
{
    int Option;
    int max_consumed = 100;

    Consumer consumers;
    Producer products;

    while ((Option = getopt(argc, argv, "E:L:f:e:")) != 1)
    {
        switch(Option)
        {
            case 'E':
                consumers.Ethel_box_time = atoi(argv[optind - 1]);
                
                break;

            case 'L':
                consumers.Lucy_box_time = atoi(argv[optind - 1]);
                
                break;

            case 'f':
                products.frog_bite = atoi(argv[optind - 1]);

                break;

            case 'e':
                products.escargot_sucker = atoi(argv[optind - 1]);

                break;

            default:
                cout << "Please Enter Values" << endl;
                exit(0);
        }

        // if optind is NULL, break
        if (argv[optind] == NULL)
            break;

    }

    // intializing queue
    Queue q(10);

    // both producers and consumers have the same queue
    products.belt = &q;
    consumers.belt = &q;

    // initializing semaphore
    Semaphore sem;

    // sem_init(address to semaphore, is it shared? 0 if yes, inital value for semaphore)
    sem_init(&sem.empty, 0, 10);
    sem_init(&sem.full, 0, 0);
    sem_init(&sem.barrier, 0, 0);
    sem_init(&sem.three_cfb, 0, 0);

    // assigning semaphore to both product and consumer
    products.sem = &sem;
    consumers.sem = &sem;
    
    // initializing the mutex
    pthread_mutex_init(&sem.mutex, NULL);

    pthread_t thread_id;
    pthread_attr_t pthread_attributes;

    //initializing thread attributes
    pthread_attr_init(&pthread_attributes);

    // creating producer threads
    pthread_create(&thread_id, &pthread_attributes, CFB, (void *) &products);
    pthread_create(&thread_id, &pthread_attributes, EES, (void *) &products);

    // creating consumer threads
    pthread_create(&thread_id, &pthread_attributes, L, (void *) &consumers);
    pthread_create(&thread_id, &pthread_attributes, E, (void *) &consumers);

    // waiting for consumers to finish eating 100th candy, then continue past this point
    sem_wait(&sem.barrier);

    // destroy the mutex
    pthread_mutex_destroy(&sem.mutex);

    // destroying the semaphores
    sem_destroy(&sem.empty);
    sem_destroy(&sem.full);
    
    // displays info on eaten and consumed
    io_production_report(q.type_produced, q.consumers);

    exit(0);
}