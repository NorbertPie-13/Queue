#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include "../includes/queue.h"

struct queue_t
{
    int front;
    int rear;
    int * array;
    size_t size;
    size_t capacity;
};



/**
 * @brief Create a queue object
 * 
 * @param number 
 * @return queue_t* 
 */
queue_t * create_queue(int number)
{
    if (number == 0 || number < 0)
    {
        fprintf(stderr, "Cannot create queue with 0 or > 0 objects.\n");
        return NULL;
    }
    // Allocate memory for a queue structure.
    queue_t * queue = calloc(1, sizeof(queue_t));
    if (NULL == queue)
    {
        fprintf(stderr, "Cannot allocate memory for queue.\n");
        return NULL;
    }
    // Capacity is the maximum number of numbers that may be held in the queue.
    queue->capacity = number;
    // There is no values so the Head is the tail
    queue->front = queue->rear;
    // Allocate an array of integers that are saved on the heap
    queue->array = calloc(number,  sizeof(int));
    if (NULL == queue->array)
    {
        fprintf(stderr, "Cannot allocate memory for file descriptors.\n");
        free(queue);
        return NULL;
    }
    return queue;
}

/**
 * @brief Static function to check if the queue is empty or not.
 * 
 * @param queue 
 * @param boolean 
 * @return int 
 */
static int is_queue_empty(queue_t * queue, bool * boolean)
{
    int ret_val = EXIT_FAILURE;
    if (NULL == queue || NULL == boolean)
    {
        fprintf(stderr, "NULL pointer given in %s.\n", __func__);
        goto EXIT;
    }
    *boolean = queue->size == 0 ? true : false;
    ret_val = EXIT_SUCCESS;
    EXIT:
        return ret_val;
}

/**
 * @brief Destroy queue object and free all memory. Return 1 on failure, 0 on success.
 * 
 * @param queue 
 * @return int 
 */
int destroy_queue(queue_t * queue)
{
    // Set the error code to failure
    int retval = EXIT_FAILURE;
    // If the array is NULL, return Error as cannot destory properly. We check before destroying anything.
    if (NULL == queue)
    {
        fprintf(stderr, "Cannot destroy NULL queue.\n");
        goto EXIT;
    }
    // Free structure last.
    FREE_STRUCT:
        free(queue->array);
        free(queue);
        queue = NULL;
    retval = EXIT_SUCCESS;
    EXIT:
        return retval;
}

/**
 * @brief Check that the queue is not full. Return 1 for failure, 0 on success. 
 * 
 * @param queue 
 * @param truth pointer to bool
 * @return int 
 */
int is_queue_full(queue_t * queue, bool * truth)
{
    // In this case it is expected to be a true or false, so the error value will be the INT32 Max;
    int retval = EXIT_FAILURE;
    if (NULL == queue || NULL == truth)
    {
        fprintf(stderr, "Failed %s, with NULL pointer.\n", __func__);
        goto EXIT;
    }
    *truth = (queue->size == queue->capacity) ? true : false;
    retval = EXIT_SUCCESS;
    EXIT:
        return retval;
}

/**
 * @brief Enqueue integer into queue. Overflow is checked. return 1 on error, 0 on success.
 * 
 * @param queue 
 * @param number 
 * @return int 
 */
int enqueue(queue_t * queue, int number)
{
    int retval = EXIT_FAILURE;
    bool is_full = false;
    // Check pointer
    if (NULL == queue)
    {
        fprintf(stderr, "Cannot enqueue with null queue.\n");
        goto EXIT;
    }
    // Ensure queue has room, if is full, then there is no room and print overfull.
    is_queue_full(queue, &is_full);
    if (true == is_full)
    {
        fprintf(stderr, "Queue overflow.\n");
        goto EXIT;
    }
    // Set the rear to the number.
    queue->array[queue->rear] = number;
    // Increase the rear by 1. Modulo to catch overflow.
    queue->rear = (queue->rear + 1) % queue->capacity;
    #ifdef DEBUG 
        printf("Rear: %d\n", queue->rear);
    #endif
    // Increase size.
    queue->size++;
    retval = EXIT_SUCCESS;

    EXIT:
        return retval;
}

/**
 * @brief Dequeue item from the queue and return item. Pass pointer for error check, 1 for failure, 0 for success.
 * 
 * @param queue 
 * @return int 
 */
int dequeue(queue_t * queue, int * error)
{
    int ret_val = EXIT_FAILURE;
    *error = EXIT_FAILURE;
    if (NULL == queue)
    {
        fprintf(stderr, "Cannot dequeue NULL value.\n");
        goto EXIT;
    }
    bool is_empty = true;
    is_queue_empty(queue, &is_empty);
    if (is_empty)
    {
        fprintf(stderr, "Cannot dequeue on empty queue.\n");
        goto EXIT;
    }
    #ifdef DEBUG
        printf("%d ", queue->array[queue->front]);
    #endif
    // set the return value to the front of the queue.
    ret_val = queue->array[queue->front];
    // set the new front to the next in line.
    queue->front = (queue->front + 1) % queue->capacity;
    // Decrease Size.
    queue->size--;
    *error = EXIT_SUCCESS;
    
    EXIT:
        return ret_val;
}



/**
 * @brief Give reference to the queue size. Return 1 on error, or 0 on sucess.
 * 
 * @param queue 
 * @param size 
 * @return int 
 */
int queue_size(queue_t * queue, int * size)
{
    int ret_val = EXIT_FAILURE;
    if (NULL == queue || NULL == size)
    {
        fprintf(stderr, "Cannot find size of NULL value in %s.\n", __func__);
        goto EXIT;
    }
    *size = queue->size;
    ret_val = EXIT_SUCCESS;
    EXIT:
        return ret_val;
}

/**
 * @brief Give reference to the first item in the queue. Return 1 on error, or 0 on success.
 * 
 * @param q 
 * @param num 
 * @return int 
 */
int front_item(queue_t * q, int * num)
{
    int ret_val = EXIT_FAILURE;
    if (NULL == q || NULL == num)
    {
        fprintf(stderr, "Cannot find head in %s.\n", __func__);
        goto EXIT;
    }
    *num = q->array[q->front];
    ret_val = EXIT_SUCCESS;

    EXIT:
        return ret_val;
}

