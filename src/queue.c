#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "queue.h"

static struct queue_t
{
    int front;
    int rear;
    int * array;
    pthread_mutex_t lock;
    size_t size;
};



// create the queue to hold file descriptors.
queue_t * create_queue(int number)
{
    if (number == 0 || number < 0)
    {
        fprintf(stderr, "Cannot create queue with 0 or > 0 objects.\n");
        return NULL;
    }

    queue_t * queue = calloc(1, sizeof(queue_t));
    if (NULL == queue)
    {
        fprintf(stderr, "Cannot allocate memory for queue.\n");
        return NULL;
    }
    queue->size = number;
    queue->front = queue->rear;
    queue->array = calloc(number, (number * sizeof(int)));
    if (0 != pthread_mutex_init(&queue->lock, NULL))
    {
        fprintf(stderr, "Mutex lock failed to create for queue.\n");
        free(queue);
        return NULL;
    }
    if (NULL == queue->array)
    {
        fprintf(stderr, "Cannot allocate memory for file descriptors.\n");
        free(queue);
        return NULL;
    }

    return queue;
}

// destroy the queue
int destroy_queue(queue_t * queue)
{
    // Set the error code to failure
    int retval = EXIT_FAILURE;
    // If the array is NULL, return Error as cannot destory properly. We check before destroying anything.
    if (NULL == queue || NULL == queue->array)
    {
        fprintf(stderr, "Cannot destroy NULL queue.\n");
        goto EXIT;
    }
    
    // If the mutex lock cannot be destroyed, return error code because structure cannot be destroyed properly.
    if (0 != pthread_mutex_destroy(&queue->lock))
    {
        fprintf(stderr, "Failed to destory mutex lock in queue.\n");
        goto EXIT;
    }
    // Free after everthing has been checked if it can be destroyed. If start destroying and run into issues can raise more issues
    // Free file descriptor array after mutex lock
    FREE_ARRAY:
        free(queue->array);
        queue->array = NULL;
    // Free structure last.
    FREE_STRUCT:
        free(queue);
        queue = NULL;
    retval = EXIT_SUCCESS;

    EXIT:
        return retval;
}

/**
 * @brief Check that the queue is not full. Return INT32_MAX on error, else 1 for True and 0 for False. This is a thread locking function.
 * 
 * @param queue 
 * @return int 
 */
int is_queue_full(queue_t * queue)
{
    // In this case it is expected to be a true or false, so the error value will be the INT32 Max;
    int retval = __INT32_MAX__;
    if (0 != pthread_mutex_lock(&queue->lock))
    {
        fprintf(stderr, "Failed mutex lock in %s\n", __func__);
        goto EXIT;
    }
    if (NULL == queue)
    {
        fprintf(stderr, "Failed %s, with NULL pointer.\n", __func__);
        goto EXIT;
    }
    retval = (((queue->rear + 1) % queue->size) == queue->front) ? 1 : 0;
    pthread_mutex_unlock(&queue->lock);

    EXIT:
        return retval;
}

// Enqueue
int enqueue(queue_t * queue, int number)
{
    int retval = 1;
    if (NULL == queue)
    {
        fprintf(stderr, "Cannot enqueue with null queue.\n");
        goto EXIT;
    }
    if (1 == is_queue_full(queue))
    {
        fprintf(stderr, "Queue overflow.\n");
        goto EXIT;
    }
    if (0 != pthread_mutex_lock(&queue->lock))
    {
        fprintf(stderr, "Failed to lock during enqueue.\n");
        goto EXIT;
    }
    queue->rear = (queue->rear + 1) % queue->size;
    queue->array[queue->rear] = number;
    if (-1 == queue->front)
    {
        queue->front = queue->rear;
    }
    retval = 0;
    pthread_mutex_unlock(&queue->lock);

    EXIT:
        return retval;
}

//Dequeue

// IsEmpty

// IsFull

// Queue size

// Front Item

