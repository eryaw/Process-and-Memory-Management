/**
 * @file      ProcessInfo.h
 * @author    Erya Wen (eryaw@student.unimelb.edu.au)
 * @brief     Process related information module. It includes
 *              1. creating a new Process data
 *              2. destory and free a Process data
 *
 * @copyright created for COMP30023 Computer System 2020
 *
 */

#ifndef PROCESSINFO_H
#define PROCESSINFO_H


// ============================================================================
// == | Data Type Definitions
// ============================================================================
typedef struct process ProcessInfo;
/**
 * @brief The ProcessInfo include process arrival time, id, required memory size 
 *        job time, time remaining to complete, time last been executed, 
 *        time be completed, 
 *        and time required to load to execute it, page fault, and
 *        a page list currently occupied by this process and the number of them,
 *        memory used by this process.
 */
struct process{
    long long int arrive_time;
    long long int p_id;
    long long int mem_size_req;
    long long int job_time;
	long long int remaining_time;

    long long int last_execution_time;
    long long int complete_time;

    long long int total_require_page;
    long long int num_page_in_mem;
    long long int page_fault;
    long long int load_time;
    
    long long int mem_usage;
    long long int *page_list;
};

// ============================================================================
// == | Module Functions
// ============================================================================
//  Create a new ProcessInfo data
ProcessInfo *new_ProcessInfo();

// Destroy and free the memory associated with a ProcessInfo data
void free_ProcessInfo(ProcessInfo *process);

#endif
