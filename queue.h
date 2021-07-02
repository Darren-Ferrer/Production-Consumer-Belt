#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>

using namespace std;

struct Queue
{
    int front;
    int rear;
    int capacity;

    // initializes 2d array for storing values
    int on_belt[2] = {0, 0};

    // keeps track of overall cfb and ees produced
    int type_produced[2] = {0, 0};

    // tracks what type of candies lucy has consumed
    int type_consumed_lucy[2] = {0, 0};

    // tracks what type of candies ethel has consumed
    int type_consumed_ethel[2] = {0, 0};

    // used for accessing lucy and ethel information
    int *consumers[2] = {type_consumed_lucy, type_consumed_ethel};
    
    // the conveyor belt
    int *queue;

    Queue(int c)
    {
        front = rear = 0;
        capacity = c;
        queue = new int[c];
    }

    // to Queue
    ~Queue()
    {
        delete[] queue;
    }

    // adding data to the front of the queue
    void add(int data)
    {
        if (capacity == rear)
        {
            //cout << "Queue is full.";
            return;
        }

        else
        {
            queue[rear] = data;
            rear++;
        }
        return;
    }

    // removes front of queue
    int remove()
    {
        int temp;
        if (front == rear)
        {
            //cout << "Queue is empty.";
            return 3;
        }

        else
        {
            temp = queue[0];
            for (int i = 0; i < rear; i++)
            {
                queue[i] = queue[i + 1];
            }
            rear--;
        }
        return temp;
    }

    // checks if queue is empty
    bool empty()
    {
        if (front == rear)
        {
            return true;
        }
        else
            return false;
    }

    // checks if queue is full
    bool full()
    {
        if (capacity == rear)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};
#endif