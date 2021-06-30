/**
 * @file      parseArgu.c
 * @author    Erya Wen (eryaw@student.unimelb.edu.au)
 * @brief     Implementation of parsing arguments method. It includes
 *              1. parsing option arguments from command lines
 *              2. creating a new ArguInfo data
 *              3. destory and free a ArguInfo data
 *            The ArguInfo include filename, scheduling algorithm flags, and
 *            memory allocation flags, maximum memory size, quantum
 *
 * @copyright created for COMP30023 Computer System 2020
 *
 */

#include "parseArgu.h"
#include "utilities.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


// ============================================================================
// == | Constant Definitions 
// ============================================================================
#define DEFAULT_QUANTAM         10 


// ============================================================================
// == | Function Prototypes
// ============================================================================
// Validate scheduling algorithm choice and return the corresponding flags
char valid_sched_algo(char *sched_algo);

// Validate memory allocation choice and return the corresponding flags
char valid_mem_alloc(char *mem_alloc);


// ============================================================================
// == | Module Functions
// ============================================================================
/**
 * @brief   Parsing the option argument from command line arguments
 * 
 * @param  argc   Mumber of inputs
 * @param  argv   An array of inputs
 * @param  argu   An ArguInfo data which saves all option argument choices
 * @return int    If all option arguments are valid, return SUCCESS (0),
 *                Otherwise, return INVALID (-1).
 */
int parseArgument(int argc, char **argv, ArguInfo *argu){
    int c = INVALID;
    opterr = 0;

    // Parsing the option argument from command line arguments
    while ((c = getopt (argc, argv, "f:a:m:s:q:")) != INVALID){
        switch (c){
            case 'f':
                argu->filename = optarg;
                break;
            case 'a':
                argu->sched_algo = valid_sched_algo(optarg);
                break;
            case 'm':
                argu->mem_alloc = valid_mem_alloc(optarg);
                break;
            case 's':
                argu->mem_size = atoi(optarg);
                break;
            case 'q':
                argu->quantum = atoi(optarg);
                break;
            case '?':
                if (optopt == 'f' 
                    || optopt == 'a' 
                    || optopt == 'm' 
                    || optopt == 's'
                    || optopt == 'q'){
                    fprintf (stderr, 
                            "Option -%c requires an argument.\n", optopt);
                } else if (isprint (optopt)) {
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                } else {
                    fprintf (stderr,
                            "Unknown option character `\\x%x'.\n",
                            optopt);
                }
                    
                exit(EXIT_FAILURE);
            default:
                abort ();
        }
    }

    // Validate if the compulsory arguments have been parsed
    if(argu->filename == NULL
     || argu->sched_algo == NULL_CHAR
     || argu->mem_alloc == NULL_CHAR){
        return INVALID;
    }
    if(argu->mem_alloc != UN_M_FLAG && argu->mem_size == INVALID){
        return INVALID;
    }


    return SUCCESS;
}


/**
 * @brief  Create a new ArguInfo data
 * 
 * @return      return a pointer to the new ArguInfo data
 */
ArguInfo *new_ArguInfo() {

    ArguInfo *argu = (ArguInfo *)malloc(sizeof *argu);
    if (argu == NULL) {
        fprintf(stderr, "Error: new_ProcessInfo() malloc returned NULL\n");
        exit(EXIT_FAILURE);
    }

    // Initalise value of the ArguInfo data
    argu->filename = NULL;
    argu->sched_algo = NULL_CHAR;
    argu->mem_alloc = NULL_CHAR;
    argu->mem_size = INVALID;
    argu->quantum = DEFAULT_QUANTAM;

    return argu;
}


/**
 * @brief  Destroy and free the memory associated with a ArguInfo data
 * 
 * @param  resp   a ArguInfo data
 */
void free_ArguInfo(ArguInfo *argu) {

    // Error if the ArguInfo does not initalise
    assert(argu!= NULL);

    // Free the ArguInfo data itself
    free(argu);
    argu = NULL;
}


// ============================================================================
// == | Auxillary Functions 
// ============================================================================

/**
 * @brief   Validate scheduling algorithm choice and return the 
 *          corresponding flags
 * 
 * @param   sched_algo  Scheduling algorithm choice from arguments
 * @return  char        If the memory allocation is one of {ff,rr,cs}, 
 *                      return the corresponding flag
 *                      Otherwise, return NULL
 */
char valid_sched_algo(char *sched_algo){
    
    if(strcmp(sched_algo, "ff") == SUCCESS){
        return FF_A_FLAG;
    } else if (strcmp(sched_algo, "rr") == SUCCESS) {
        return RR_A_FLAG;
    } else if (strcmp(sched_algo, "cs") == SUCCESS) {
        return CS_A_FLAG;
    } else {
        return NULL_CHAR;
    }

}


/**
 * @brief   Validate memory allocation choice and return the corresponding flags
 * 
 * @param   mem_alloc   Memory allocation choice from arguments
 * @return  char        If the memory allocation is one of {u,p,v,cm}, 
 *                      return the corresponding flag
 *                      Otherwise, return NULL
 */
char valid_mem_alloc(char *mem_alloc){
    
    if(strcmp(mem_alloc, "u") == SUCCESS){
        return UN_M_FLAG;
    } else if (strcmp(mem_alloc, "p") == SUCCESS) {
        return SP_M_FLAG;
    } else if (strcmp(mem_alloc, "v") == SUCCESS) {
        return VM_M_FLAG;
    } else if (strcmp(mem_alloc, "cm") == SUCCESS) {
        return CM_M_FLAG;
    } else {
        return NULL_CHAR;
    }

}
