/**
 * @file      cpuInfo.h
 * @author    Erya Wen (eryaw@student.unimelb.edu.au)
 * @brief     CPU related information module. It includes
 *              1. creating a new CPU data and Memory data
 *              2. destory and free a CPU data and Memory data
 *
 * @copyright created for COMP30023 Computer System 2020
 *
 */

#ifndef CPUINFO_H
#define CPUINFO_H

#include "queue.h"


// ============================================================================
// == | Data Type Definitions
// ============================================================================
typedef struct memory MemoryInfo;
/**
 * @brief The MemoryInfo include number of total memory pages, number of holes,
 *        current memory use, and memory page list 
 *        occupied information (record page currently occupied by which process). 
 */
struct memory {
    long long int total_page;
    long long int num_holes;
    int memory_use;
    long long int *is_occupy_lst;
};

typedef struct cpu CPUInfo;
/**
 * @brief The CPUInfo include quantam, number of process is current finished, 
 *        current running time, memory information
 *        and turnaround, time overhead, current maximum time overhead 
 *        for statistics usage.
 */
struct cpu {
    long long int complete_num_process;
    long long int curr_time;
    long long int total_turnaround;
    long double time_overh;
    long double max_time_overh;
    long long int quantam;
    MemoryInfo *memory;
};


// ============================================================================
// == | Module Functions
// ============================================================================
// Create a new CPUInfo data
CPUInfo *new_CPUInfo();

// Destroy and free the memory associated with a CPUInfo data
void free_CPUInfo(CPUInfo *p);

// Create a new MemoryInfo data
MemoryInfo *new_MemoryInfo(long long int size);

// Destroy and free the memory associated with a MemoryInfo data
void free_MemoryInfo(MemoryInfo *p);

#endif