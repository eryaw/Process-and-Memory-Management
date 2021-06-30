/**
 * @file      processInfo.c
 * @author    Erya Wen (eryaw@student.unimelb.edu.au)
 * @brief     Implementation of Process related information module. It includes
 *              1. creating a new Process data 
 *              2. destory and free a Process data 
 *
 * @copyright created for COMP30023 Computer System 2020
 *
 */

#include "processInfo.h"
#include "utilities.h"

#include <stdio.h>
#include <stdlib.h>

#include <assert.h>
#include <string.h>


// ============================================================================
// == | Module Functions
// ============================================================================
/**
 * @brief  Create a new ProcessInfo data
 * 
 * @return      return a pointer to the new ProcessInfo data
 */
ProcessInfo *new_ProcessInfo() {

    ProcessInfo *p = (ProcessInfo *)malloc(sizeof *p);
    if (p == NULL) {
        fprintf(stderr, "Error: new_ProcessInfo() malloc returned NULL\n");
        exit(EXIT_FAILURE);
    }

    // Initalise value of the CPUInfo data
    p->arrive_time = INVALID;
    p->p_id = INVALID;
    p->mem_size_req = INVALID;
    p->job_time = INVALID;

    p->last_execution_time = INITIAL;
    
    p->total_require_page = INVALID;
    p->num_page_in_mem = INITIAL;
    p->page_fault = INITIAL;
    p->load_time = INITIAL;

    p->mem_usage = INITIAL;
    
    p->page_list = NULL;
	p->remaining_time = INITIAL;
    

    return p;
}


/**
 * @brief  Destroy and free the memory associated with a ProcessInfo data
 * 
 * @param  resp   a ProcessInfo data
 */
void free_ProcessInfo(ProcessInfo *p) {

    // Error if the ProcessInfo does not initalise
    assert(p!= NULL);

    // Free the ProcessInfo data itself
    free(p);
    p = NULL;
}

