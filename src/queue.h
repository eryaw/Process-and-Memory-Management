/**
 * @file      queue.h
 * @author    Erya Wen (eryaw@student.unimelb.edu.au)
 * @brief     Queue module. It includes
 *              1. creating new queue
 *              2. destory a queue
 *              3. inserting elements to the queue
 *              4. deleting elements from the start of the queue
 *              5. getting size of the queue
 *              6. getting specific position elements in the queue
 *
 * @copyright created for COMP30023 Computer System 2020
 *
 */

#ifndef QUEUE_H
#define QUEUE_H

#include "processInfo.h"


// ============================================================================
// == | Constant Definitions 
// ============================================================================
#define PRIO_ARRIVAL_T      "ARRT"
#define PRIO_LAST_EXE_T     "LASTET"
#define PRIO_JOB_T          "JOBT"
#define PRIO_MAX_PAGE       "MAXPAGE"
#define PRIO_COMPLETE_T     "COMPLETET" 


// ============================================================================
// == | Data Type Definitions
// ============================================================================
typedef struct queue Queue;


// ============================================================================
// == | Module Functions
// ============================================================================
// Create a new queue and return its pointer
Queue *new_queue();

// Destroy a queue and free its memory
void free_queue(Queue *queue);

// Add an element to the queue based on the priority
void enqueue(Queue *queue, ProcessInfo *p, char *prio_flag);

// Remove and return the front data element from a queue
ProcessInfo *dequeue(Queue *queue);

// Return the number of elements contained in a queue
long long int get_queue_size(Queue *queue);

// Return the data of a given index ProcessInfo from a Queue
ProcessInfo *get_queue_point(Queue *queue, long long int index);

#endif