/**
 * @file      parseArgu.h
 * @author    Erya Wen (eryaw@student.unimelb.edu.au)
 * @brief     Parsing arguments method. It includes
 *              1. parsing option arguments from command lines
 *              2. creating a new ArguInfo data
 *              3. destory and free a ArguInfo data
 *            The ArguInfo include filename, scheduling algorithm flags, and
 *            memory allocation flags, maximum memory size, quantum
 *
 * @copyright created for COMP30023 Computer System 2020
 *
 */

#ifndef PARSEARGU_H
#define PARSEARGU_H


// ============================================================================
// == | Data Type Definitions
// ============================================================================
typedef struct argu ArguInfo;
/**
 * @brief   The ArguInfo include filename, scheduling algorithm flags, and
 *          memory allocation flags, maximum memory size, quantum
 */
struct argu {
    char *filename;
    char sched_algo;
    char mem_alloc;
    long long int mem_size;
    long long int quantum;
};


// ============================================================================
// == | Module Functions
// ============================================================================
// Parsing the option argument from command line arguments
int parseArgument(int argc, char **argv, ArguInfo *argu);

//  Create a new ArguInfo data
ArguInfo *new_ArguInfo();

// Destroy and free the memory associated with a ArguInfo data
void free_ArguInfo(ArguInfo *argu);


#endif
