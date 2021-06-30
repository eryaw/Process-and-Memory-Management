/**
 * @file      simulateSched.c
 * @author    Erya Wen (eryaw@student.unimelb.edu.au)
 * @brief     Schedluing Simulation module. It includes
 *              1. simulate processes running on CPU based on scheduling algorithm
 *              2. update the performence statistic
 *
 * @copyright created for COMP30023 Computer System 2020
 *
 */


#ifndef SIMULATESCHED_H
#define SIMULATESCHED_H

#include "queue.h"
#include "memoryAlloc.h"
#include "processInfo.h"
#include "cpuInfo.h"


// ============================================================================
// == | Module Functions
// ============================================================================
/**
 * Simulate to allocates processes to a CPU and manages memory allocation 
 * among the running processes 
 */
Queue *simulation(char sched_algo_f, char mem_alloc_f, 
                    Queue *queue, CPUInfo *cpu);

#endif