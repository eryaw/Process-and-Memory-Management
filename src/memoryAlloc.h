/**
 * @file      memoryAlloc.h
 * @author    Erya Wen (eryaw@student.unimelb.edu.au)
 * @brief     Memory allocation module. It includes
 *              1. Simulate memory allocation
 *              2. print Execution transcript
 *
 * @copyright created for COMP30023 Computer System 2020
 *
 */

#ifndef MEMORYALLOC_H
#define MEMORYALLOC_H

#include "queue.h"
#include "cpuInfo.h"
#include "processInfo.h"


// ============================================================================
// == | Module Functions
// ============================================================================
// Simulate memory allocation before a process can be executed on the CPU
void mem_alloc_func(char mem_alloc_f, Queue *queue, 
                    CPUInfo *cpu, ProcessInfo *p);

// Print the process finish message 
void print_finished(char mem_alloc_f, CPUInfo *cpu, 
                    ProcessInfo *p, Queue *queue);


#endif