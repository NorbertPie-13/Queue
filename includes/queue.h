#ifndef QUEUE_H
#define QUEUE_H
#include <stdbool.h>

typedef struct queue_t queue_t;

/**
 * @brief Create a queue object
 * 
 * @param number 
 * @return queue_t* 
 */
queue_t * create_queue(int number);

/**
 * @brief Destroy queue object and free all memory. Return 1 on failure, 0 on success.
 * 
 * @param queue 
 * @return int 
 */
int destroy_queue(queue_t * queue);

/**
 * @brief Check that the queue is not full. Return 1 for failure, 0 on success. 
 * 
 * @param queue 
 * @param truth pointer to bool
 * @return int 
 */
int is_queue_full(queue_t * queue, bool * truth);

/**
 * @brief Enqueue integer into queue. Overflow is checked. return 1 on error, 0 on success.
 * 
 * @param queue 
 * @param number 
 * @return int 
 */
int enqueue(queue_t * queue, int number);

/**
 * @brief Dequeue item from the queue and return item. Pass pointer for error check, 1 for failure, 0 for success.
 * 
 * @param queue 
 * @return int 
 */
int dequeue(queue_t * queue);

/**
 * @brief Give reference to the queue size. Return 1 on error, or 0 on sucess.
 * 
 * @param queue 
 * @param size 
 * @return int 
 */
int queue_size(queue_t * queue, int * size);

/**
 * @brief Give reference to the first item in the queue. Return 1 on error, or 0 on success.
 * 
 * @param q 
 * @param num 
 * @return int 
 */
int front_item(queue_t * q, int * num);
#endif