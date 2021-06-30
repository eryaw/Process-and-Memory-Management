/**
 * @file      queue.c
 * @author    Erya Wen (eryaw@student.unimelb.edu.au)
 * @brief     Implementation of Queue module. It includes
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

#include "queue.h"

#include "processInfo.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// ============================================================================
// == | Constant Definitions 
// ============================================================================
#define INSERT_LAST         -1
#define INSERT_MIDDLE       1
#define INSERT_START        0


// ============================================================================
// == | Data Type Definitions
// ============================================================================
typedef struct node Node;
/**
 * @brief  A queue node points to the next and previous node in the queue,
 *         which data is ProcessInfo datatype
 */
struct node {
    struct node *prev;
    struct node *next;
    ProcessInfo *process;
};


/**
 * @brief  A queue points to its first and last nodes,
 *         and stores its size (number of nodes)
 */
struct queue {
    Node *head;
    Node *last;
    long long int size;
};


// ============================================================================
// == | Function Prototypes
// ============================================================================
// Create a new node and return its address
Node *new_node(ProcessInfo *process);

// Get the priority value based on the priority choice
long long int get_priority(ProcessInfo *p, char *prio_flag);


// ============================================================================
// == | Module Functions
// ============================================================================
/**
 * @brief  Create a new empty Queue 
 * 
 * @return        the pointer of new empty Queue
 */
Queue *new_queue() {

    Queue *queue = (Queue *)malloc(sizeof *queue);
    if (queue == NULL) {
        fprintf(stderr, "Error: new_queue() malloc returned NULL\n");
        exit(EXIT_FAILURE);
    }

    // Initalise value of the queue
    queue->head = NULL;
    queue->last = NULL;
    queue->size = 0;

    return queue;
}


/**
 * @brief  Destroy and free the memory associated with a Queue
 *
 * @param  queue  a Queue
 */
void free_queue(Queue *queue) {

    // Error if the list does not initalise
    assert(queue != NULL);

    // Free ProcessInfo data in each nodes.
    long long int size = get_queue_size(queue);
    while (size > 0) {
        ProcessInfo *process = dequeue(queue);
        free_ProcessInfo(process);
        size = get_queue_size(queue);
    }

    // Free the last node and queue itself
    free(queue->last);
    queue->head = NULL;
    queue->last = NULL;

    free(queue);
    queue = NULL;
}



/**
 * @brief  Add an element to the queue based on the priority 
 * 
 * @param  queue      a Queue
 * @param  process    a ProcessInfo data
 * @param  prio_flag  a Priority flag
 */
void enqueue(Queue *queue, ProcessInfo *process, char *prio_flag) {

    // Error if the list or process does not initalise
    assert(queue != NULL);
    assert(process != NULL);

    // Creat a new queue node to store this data
    Node *node = new_node(process);

    // Connect the node with the current last node
    node->prev = queue->last;
  

    if (queue->size == 0) {
        // If the queue was empty, new node is now the first node
        queue->head = node;
        queue->last = node;
    } else {
         // Otherwise, it goes after the current last node
        Node *last = queue->last;

        int insert_flag = INSERT_LAST;
        
        /**
         * Enqueue the process based on the priority value in ascending order
         * If the 2 processes priority value are the same, 
         * based on the process id in ascening oreder
         */
        while (get_priority(last->process, prio_flag) 
                > get_priority(node->process, prio_flag)
            || (get_priority(last->process, prio_flag) 
                == get_priority(node->process, prio_flag)
            && last->process->p_id > node->process->p_id)){

            if (last -> prev == NULL){
                insert_flag = INSERT_START;
                node->next = last;
                last->prev = node;
                queue->head = node;
                break;
            } 
            insert_flag = INSERT_MIDDLE;
            last = last->prev;
        }
    

        if (insert_flag == INSERT_START){
            // Place this new node at the start of the queue
            node->next = last;
            last->prev = node;
            queue->head = node;
            node->prev = NULL;

        } else if (insert_flag == INSERT_MIDDLE){
            // Place this new node at the middle of the queue
            node->next = last->next;
            node->prev = last;
            last->next->prev = node;
            last->next = node;

        } else{
            // Place this new node at the end of the queue
            last->next = node;
            queue->last = node;
        }
    }

    // Update the queue size
    queue->size++;
}


/**
 * @brief  Remove and return the first ProcessInfo data from a Queue, 
 *         and free the memory of the node
 * 
 * @param  queue  a Queue
 * @return        the first ProcessInfo data from a Queue
 */
ProcessInfo *dequeue(Queue *queue) {

    // Error if the list does not initalise or it is empty
    assert(queue != NULL);
    assert(queue->size > 0);

    // Set a ProcessInfo to save the data will be removed
    ProcessInfo *process = queue->head->process;

    if (queue->size == 1) {
        // If it was the last node in the queue, the last needs to be cleared
        // and free the memory
        free(queue->head);
        queue->last = NULL;
    } else {
        // Otherwise, the next node becomes the first node
        queue->head = queue->head->next;

        // free the memory
        free(queue->head->prev);
        queue->head->prev = NULL;
    }

    // Update the queue size
    queue->size--;

    // Return the first ProcessInfo data from the queue
    return process;
}


/**
 * @brief  Get the number of elements in a Queue
 * 
 * @param  queue  a Queue
 * @return        the number of elements in a Queue
 */
long long int get_queue_size(Queue *queue) {

    // Error if the list does not initalise
    assert(queue != NULL);

    // Return the number of Points in a Queue
    return queue->size;
}


/**
 * @brief  Get the data of a given index ProcessInfo from a Queue
 * 
 * @param  queue  a Queue
 * @param  index  an index
 * @return        the ProcessInfo data of a given index from a Queue
 */
ProcessInfo *get_queue_point(Queue *queue, long long int index) {
    long long int start;
    long long int i;
    Node *curr;

    // Error if the list does not initalise or it is empty, or index is invalid
    assert(queue != NULL);
    assert(queue->size > index);
    assert(index >= 0);

    if (index >= (queue->size / 2)) {
        // If the index is near the end, find the node from the end of the queue
        start = queue->size - 1;
        curr  = queue->last;

        for (i = start; i > index; i--) {
            curr = curr->prev;
        }
    } else {
        // Otherwise, find the node from the start of the queue
        start = 0;
        curr  = queue->head;

        for (i = start; i < index; i++) {
            curr = curr->next;
        }
    }

    // Return the ProcessInfo data of a given index from a Queue
    return curr->process;
}

// ============================================================================
// == | Auxillary Functions 
// ============================================================================
/**
 * @brief  Create a new node and return its address
 * 
 * @param  process    a ProcessInfo data
 * @return        the address of a new node, with process as data
 */
Node *new_node(ProcessInfo *process) {
    
    Node *node = (Node *)malloc(sizeof(*node));
    if (node == NULL) {
        fprintf(stderr, "Error: new_node() malloc returned NULL\n");
        exit(EXIT_FAILURE);
    }


    // Assign the data value
    // Initialise the previous and next node as NULL
    node->process  = process;
    node->prev = NULL;
    node->next = NULL;

    // Return the new node address
    return node;
}


/**
 * @brief  Get the priority value based on priority flag
 * 
 * @param  p            a process
 * @param  prio_flag    a priority flag
 * @return long long int     a priority value
 */
long long int get_priority(ProcessInfo *p, char *prio_flag){

    if (strcmp(prio_flag, PRIO_ARRIVAL_T) == 0){
        // Priority value is process arrival time
        return p->arrive_time;

    } else if (strcmp(prio_flag, PRIO_JOB_T) == 0){
        // Priority value is process job time
        return p->job_time;

    } else if (strcmp(prio_flag, PRIO_LAST_EXE_T) == 0){
        // Priority value is process last execution time
        return p->last_execution_time;

    } else if (strcmp(prio_flag, PRIO_MAX_PAGE) == 0){
        // Priority value is process pages in memory
        return p->num_page_in_mem;

    } else if (strcmp(prio_flag, PRIO_COMPLETE_T) == 0){
        // Priority value is process completion time
        return p->complete_time;

    } else {
        fprintf(stderr, "Invalid priority flag");
        exit(EXIT_FAILURE);
    }
}




