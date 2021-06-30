/**
 * @file      main.c
 * @author    Erya Wen (eryaw@student.unimelb.edu.au)
 * @brief     P
 *
 * @copyright created for COMP30023 Computer System 2020
 *
 */

#include "parseArgu.h"
#include "queue.h"
#include "simulateSched.h"
#include "memoryAlloc.h"
#include "cpuInfo.h"
#include "utilities.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>


// ============================================================================
// == | Constant Definitions 
// ============================================================================
#define MIN_NUM_ARGUMENT        7
#define BIN_LEN                 60.0


// ============================================================================
// == | Function Prototypes
// ============================================================================
// Read Processes from file
void read_file(Queue *queue, char *filename);

// Print performance statistics
void print_statistics(CPUInfo *cpu, Queue *complete_queue);

// Print program usage
void print_usage(char *prog_name);


// ============================================================================
// == | Main Functions
// ============================================================================
/**
 * @brief   A simulator that allocates processes to a CPU 
 *          and manages memory allocation among the running processes 
 * 
 * @param  argc   number of inputs
 * @param  argv   an array of inputs
 * @return        if no fail exits, return 0
 */

int main (int argc, char **argv){

    // If the input is incorrect, exits
    if (argc < MIN_NUM_ARGUMENT){
        print_usage(argv[0]);
		exit(EXIT_FAILURE);
    }

    ArguInfo *argu = new_ArguInfo();
    
    // Parse option arguments
    if (parseArgument(argc, argv, argu) == INVALID) {
        // If option arguments are invalid, print program usage and exits
        print_usage(argv[0]);
        free_ArguInfo(argu);
        exit(EXIT_FAILURE);
    }

    // read the processes from the file
    Queue *queue = new_queue();
    CPUInfo *cpu = new_CPUInfo();
    read_file(queue, argu->filename);

    // Initialise the quantum which is fixed and maximum memory size for cpu
    cpu->quantam = argu->quantum;
    cpu->memory = new_MemoryInfo(argu->mem_size / KB_PAGES);

    // Simulation the process allocation and memory management
    Queue *complete = simulation(argu->sched_algo, argu->mem_alloc, queue, cpu);
    // Print performance statistics after simulation finished.
    print_statistics(cpu, complete);

    // Free memory allocation
    free_ArguInfo(argu);
    free_queue(queue);
    free_queue(complete);
    free_CPUInfo(cpu);

    return 0;
}


// ============================================================================
// == | Auxillary Functions 
// ============================================================================
/**
 * @brief Read Processes from file
 * 
 * @param queue     a queue to store processes
 * @param filename  file name
 */
void read_file(Queue *queue, char *filename){

    long long int arrive_time;
    long long int p_id;
    long long int mem_size_req;
    long long int job_time;

    // Open the file in read mode
    FILE *fp = fopen(filename, "r"); 
    if (fp == NULL) {
        perror("Error while opening the file");
        exit(EXIT_FAILURE);
    }

    /*
     * Read the process information from the file 
     * The standard file should contain 4 integers per line, which represents
     * process arrival time, process id, memory required for a process
     * and process job time (in order).
     */
    while(fscanf(fp, "%lld %lld %lld %lld\n", 
                &arrive_time, &p_id, &mem_size_req, &job_time) != EOF )
    {
        ProcessInfo *p = new_ProcessInfo();
        p->arrive_time = arrive_time;
        p->p_id = p_id;
        p->mem_size_req = mem_size_req;
        p->job_time = job_time;
        p->remaining_time = job_time;
        enqueue(queue, p, PRIO_ARRIVAL_T);
        
    }

   fclose(fp);  
}


/**
 * @brief   Print performance statistics, including Throughput, Turnaround time,
 *          Time overhead, Makespan 
 */
void print_statistics(CPUInfo *cpu, Queue *complete_queue){

    long long int num_process = cpu->complete_num_process;

    /**
     * Initialise the bin which bin width is 60 seconds 
     */
    long long int bin_size; 
    bin_size = my_ceil(cpu->curr_time * 1.0 / BIN_LEN);
    long long int *bin = (long long int *)malloc(bin_size * (sizeof(long long int)));
    for(int i = 0; i < bin_size; i++){
        bin[i] = 0;
    }

    /**
     * Put the completed process in sequential non-overlapping 60 second 
     * intervals, with the first interval starting at 1. 
     */
    for(long long int i = 0; i < num_process; i++){
        long long int bin_ind;
        ProcessInfo *p = dequeue(complete_queue);
        bin_ind = (my_ceil(p->complete_time * 1.0 / BIN_LEN)) - 1;
        bin[bin_ind] += 1;
        free_ProcessInfo(p);
    }

    // Calculate average, maximum and minimum throughput
    long long int max_throughput = bin[0]; 
    long long int min_throughput = bin[0]; 
    long long int avg_throughput = bin[0];
    for(int i = 1; i < bin_size; i++){
        
        avg_throughput += bin[i];
        if (bin[i] > max_throughput){
            max_throughput = bin[i];
        }
        if (bin[i] < min_throughput){
            min_throughput = bin[i];
        } 
    }
    avg_throughput = my_ceil(avg_throughput * 1.0 / bin_size * 1.0);
    
    // Calculate turnaround time
    long long int avg_turnaround_time = 0;
    double total_turnaround = cpu->total_turnaround * 1.0;
    avg_turnaround_time = my_ceil(total_turnaround / num_process * 1.0);

    // Calcuate average time overheads
    long double avg_time_overh = 0;
    avg_time_overh = cpu->time_overh / num_process;

    // Print out performance statistics
    fprintf(stdout, "Throughput %lld, %lld, %lld\n", 
                    avg_throughput, min_throughput, max_throughput);
    fprintf(stdout, "Turnaround time %lld\n", avg_turnaround_time);
    fprintf(stdout, "Time overhead %.2Lf %.2Lf\n", 
                    cpu->max_time_overh, avg_time_overh);
    fprintf(stdout, "Makespan %lld\n", cpu->curr_time);

    free(bin);
    bin = NULL;
}


/**
 * @brief Print the usage information for the command line tool
 * 
 * @param prog_name     Program name
 */
void print_usage(char *prog_name)
{
	fprintf(stderr, "Usage: %s ", prog_name);
    fprintf(stderr, "-f <filename> ");
    fprintf(stderr, "-a <scheduling algorithm> ");
    fprintf(stderr, "-m <memory allocation> ");
    fprintf(stderr, "-s <memory size> ");
    fprintf(stderr, "[-q <quatum>]");
    fprintf(stderr, "\n");
    fprintf(stderr, "Notice:\n");
    fprintf(stderr, "\tOption arguments can be passed in any order\n");
    fprintf(stderr, "\t<scheduling algorithm>\tOne of {ff, rr, cs}\n");
    fprintf(stderr, "\t<memory allocation>\tOne of {u, p, vm ,cs}\n");
    fprintf(stderr, "\t-s <memory size>\tIf '-m u', this can be optional\n");
    fprintf(stderr, "\t[-q <quatum>]   \tOptional\n");
}




