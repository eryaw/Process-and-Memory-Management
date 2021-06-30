/**
 * @file      cpuInfo.c
 * @author    Erya Wen (eryaw@student.unimelb.edu.au)
 * @brief     Implementation of CPU related information module. It includes
 *              1. creating a new CPU data and Memory data
 *              2. destory and free a CPU data and Memory data
 *
 * @copyright created for COMP30023 Computer System 2020
 *
 */

#include "cpuInfo.h"
#include "utilities.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


// ============================================================================
// == | Main Functions
// ============================================================================
/**
 * @brief  Create a new CPUInfo data
 * 
 * @return      return a pointer to the new CPUInfo data
 */
CPUInfo *new_CPUInfo() {

    CPUInfo *cpu = (CPUInfo *)malloc(sizeof *cpu);
    if (cpu == NULL) {
        fprintf(stderr, "Error: new_CPUInfo() malloc returned NULL\n");
        exit(EXIT_FAILURE);
    }

    // Initalise value of the CPUInfo data
    cpu->complete_num_process = INITIAL;
    cpu->curr_time = INITIAL;
    cpu->total_turnaround = INITIAL;
    cpu->time_overh = INITIAL;
    cpu->max_time_overh= INITIAL;
    cpu->quantam = INITIAL;
    cpu->memory = NULL;

    return cpu;
}


/**
 * @brief  Destroy and free the memory associated with a CPUInfo data
 * 
 * @param  resp   a CPUInfo data
 */
void free_CPUInfo(CPUInfo *cpu) {

    // Error if the CPUInfo does not initalise
    assert(cpu!= NULL);

    // Free the memory associated with a CPUInfo
    free_MemoryInfo(cpu->memory);
    cpu->memory = NULL;

    // Free the CPUInfo data itself
    free(cpu);
    cpu = NULL;
}


/**
 * @brief  Create a new MemoryInfo data
 * 
 * @return      return a pointer to the new MemoryInfo data
 */
MemoryInfo *new_MemoryInfo(long long int size) {

    MemoryInfo *memory = (MemoryInfo *)malloc(sizeof *memory);
    if (memory == NULL) {
        fprintf(stderr, "Error: new_MemoryInfo() malloc returned NULL\n");
        exit(EXIT_FAILURE);
    }

    // Initalise value of the MemoryInfo data
    memory->memory_use = INITIAL;
    memory->total_page = size;
    memory->num_holes = size;

    // Initalise value of the page list 
    memory->is_occupy_lst = (long long int *)malloc(size * (sizeof(long long int)));
    if (memory->is_occupy_lst == NULL) {
        fprintf(stderr, 
            "Error: new_MemoryInfo() malloc of is_occupy_lst returned NULL\n");
        exit(EXIT_FAILURE);
    }
    for (long long int i = 0; i < size; i++){
        memory->is_occupy_lst[i] = INVALID; 
    }

    return memory;
}


/**
 * @brief  Destroy and free the memory associated with a MemoryInfo data
 * 
 * @param  resp   a MemoryInfo data
 */
void free_MemoryInfo(MemoryInfo *memory) {

    // Error if the MemoryInfo does not initalise
    assert(memory!= NULL);

    // Free the memory associated with a MemoryInfo
    free(memory->is_occupy_lst);
    memory->is_occupy_lst = NULL;

    // Free the MemoryInfo data itself
    free(memory);
    memory = NULL;
}
