/**
 * @file      simulateSched.c
 * @author    Erya Wen (eryaw@student.unimelb.edu.au)
 * @brief     Implementation of Schedluing Simulation module. It includes
 *              1. simulate processes running on CPU based on scheduling algorithm
 *              2. update the performence statistic
 *
 * @copyright created for COMP30023 Computer System 2020
 *
 */


#include "simulateSched.h"
#include "utilities.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


// ============================================================================
// == | Function Prototypes
// ============================================================================
// Update current time after process execution finished
void add_curr_time(char sched_algo_f, CPUInfo *cpu, ProcessInfo *p);

// Update the turnaround time and time overhead for statistics
void record_statistic(CPUInfo *cpu, ProcessInfo *p);


// ============================================================================
// == | Main Functions
// ============================================================================
/**
 * @brief  Simulate to allocates processes to a CPU 
 *         and manages memory allocation among the running processes 
 * 
 * @param  sched_algo_f     a scheduling algorithm flag
 * @param  mem_alloc_f      a memory allocation flag
 * @param  queue            a queue
 * @param  cpu              a CPUInfo data
 * @return Queue*           a queue with completed process
 */
Queue *simulation(char sched_algo_f, char mem_alloc_f, 
                Queue *queue, CPUInfo *cpu){

    // Error if the Queue or CPUInfo does not initalise
    assert(queue != NULL);
    assert(cpu != NULL);

    // A queue that record the process waiting to be executed
    Queue *waiting_queue = new_queue();
    if (get_queue_size(queue) > 0){
        enqueue(waiting_queue, dequeue(queue), PRIO_ARRIVAL_T);
    }
    
    // A queue that record the completed process 
    Queue *complete_queue = new_queue();
    
    // Simulation of loading and executing the process
    while(get_queue_size(waiting_queue) > 0 || get_queue_size(queue) > 0){
        
        if (get_queue_size(waiting_queue) > 0){
            /** 
             * If currently there are process in the waiting queue, 
             * run the first process
             */ 
            ProcessInfo *p = dequeue(waiting_queue);

            // Allocate process to CPU
            mem_alloc_func(mem_alloc_f, waiting_queue, cpu, p);

            // Record current time after the process loading and executing 
            add_curr_time(sched_algo_f, cpu, p);
            
            /**
             * If there are other processes arrival during a process running,
             * put there into waiting queue based on the sheduling algorithms
             */
            while(get_queue_size(queue) > 0){
                ProcessInfo *p = get_queue_point(queue, 0);
                if (p->arrive_time <= cpu->curr_time){
                    if(sched_algo_f == CS_A_FLAG){
                        /**
                         * If scheduling algorithm is Customised Scheduling, which 
                         * is Short Job First in this project, a process with
                         * short job time will run first
                         */
                        enqueue(waiting_queue, dequeue(queue), PRIO_JOB_T);
                    } else if (sched_algo_f == FF_A_FLAG 
                            || sched_algo_f == RR_A_FLAG){

                        /**
                         * If scheduling algorithm is First-come first-served, 
                         * or Round-robin, a process with early arrival time
                         * will run first
                         */
                        enqueue(waiting_queue, dequeue(queue), PRIO_ARRIVAL_T);
                    }
                    
                } else {
                    break;
                }
            } 

            // Update the process lastest execution time
            p->last_execution_time = cpu->curr_time;

            if(sched_algo_f == RR_A_FLAG && p->remaining_time != 0){
                /**
                 * When the total running time for a process 
                 * haven't reached its specificed job-time
                 */
                enqueue(waiting_queue, p, PRIO_LAST_EXE_T);

            } else {
                // When a process finised running its specificed job-time
                record_statistic(cpu, p);
                print_finished(mem_alloc_f, cpu, p, waiting_queue);

                p->complete_time = cpu->curr_time;
                enqueue(complete_queue, p, PRIO_COMPLETE_T);
                cpu->complete_num_process += 1;
            }

        } else {
            /**
             * If currently there are not process in the waiting queue, 
             * wait till next process arrival.
             */
            if (get_queue_size(queue) > 0){
                ProcessInfo *p = dequeue(queue);
                enqueue(waiting_queue, p, PRIO_ARRIVAL_T);
                cpu->curr_time = p->arrive_time;
            }
        }
       
    }


    free_queue(waiting_queue);
    return complete_queue;
}


// ============================================================================
// == | Auxillary Functions 
// ============================================================================
/**
 * @brief  Update current time after process execution finished
 * 
 * @param  sched_algo_f     scheduling algorithm flag
 * @param  cpu              a CPUInfo data
 * @param  p                a ProcessInfo data
 */
void add_curr_time(char sched_algo_f, CPUInfo *cpu, ProcessInfo *p){

    if (sched_algo_f == FF_A_FLAG){
        /**
         * If scheduling algorithm is First-come first-served, a process loads
         * and executs until total running time reaches the specified job-time
         */
        cpu->curr_time += p->job_time + p->load_time;

    } else if (sched_algo_f == RR_A_FLAG){
        /**
         * If scheduling algorithm is Round Robin, a process loads
         * and executs shorter or equal to the maximum limited time (quantam)
         */
        if (p->remaining_time >= cpu->quantam){ 
            p->remaining_time -= cpu->quantam;
            cpu->curr_time += cpu->quantam + p->load_time; 
        } else {
            cpu->curr_time += p->remaining_time + p->load_time;
            p->remaining_time = 0;
        }

    } else if (sched_algo_f == CS_A_FLAG){
        /**
         * If scheduling algorithm is Customised Scheduling, which is 
         * Short Job First in this project, a process loads and executs until 
         * total running time reaches the specified job-time
         */
        cpu->curr_time += p->job_time + p->load_time;
    } 
}


/**
 * @brief  Update the turnaround time and time overhead for statistics
 * 
 * @param  cpu      a CPUInfo data
 * @param  p        a ProcessInfo data
 */
void record_statistic(CPUInfo *cpu, ProcessInfo *p){
    // update the turnaround time
    double curr_turnaround = cpu->curr_time - p->arrive_time;
    cpu->total_turnaround += curr_turnaround;

    // update the total and maximum time overhead 
    double time_overh = curr_turnaround / (double) p->job_time;
    cpu->time_overh += time_overh;
    if(time_overh > cpu->max_time_overh){
        cpu->max_time_overh = time_overh;
    }
}
