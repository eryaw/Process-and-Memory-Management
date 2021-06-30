/**
 * @file      memoryAlloc.c
 * @author    Erya Wen (eryaw@student.unimelb.edu.au)
 * @brief     implementation Memory allocation module. It includes
 *              1. Simulate memory allocation
 *              2. print Execution transcript. including running, evict, finish
 *              3. evict pages
 *
 * @copyright created for COMP30023 Computer System 2020
 *
 */

#include "memoryAlloc.h"
#include "utilities.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


// ============================================================================
// == | Constant Definitions 
// ============================================================================
#define PAGE_SIZE               4
#define MIN_EXE_PAGE            4
#define LOAD_TIME               2


// ============================================================================
// == | Function Prototypes
// ============================================================================
// Evict number of pages from the processes until reach the requirement
void evcit(char mem_alloc_f, Queue *queue, CPUInfo *cpu,  
            long long int curr_require_page);
// Compare two long long integer values, a and b
int long_int_comparator(const void * a, const void *b);

// Free the pages in the memory that previously occupied by a process
long long int free_page(CPUInfo *cpu, ProcessInfo *p, long long int *page_list, 
                   long long int num_page,long long int start);

// Remove N element for a long long int list
long long int *remove_n_element(long long int *page_list, 
                                long long int n, long long int p_size);

// Print the currently execution message for the memory size is unlimited
void print_running(CPUInfo *cpu, ProcessInfo *p);

// Print the currently execution message for the memory size is limited
void print_mem_running(CPUInfo *cpu, ProcessInfo *p);

// Print the evicted message
void print_evicted(CPUInfo *cpu, long long int *page_list, long long int p_size);


// ============================================================================
// == | Main Functions
// ============================================================================
/**
 * @brief  Simulate memory allocation before a process can be executed on the CPU
 * 
 * @param  mem_alloc_f  a memory allocation method flag
 * @param  queue        a Queue of processes
 * @param  cpu          a CPUInfo data
 * @param  p            a ProcessInfo data
 */
void mem_alloc_func(char mem_alloc_f, Queue *queue, 
                    CPUInfo *cpu, ProcessInfo *p){
    
    /**
     * If the memory is unlimited, then memory management and 
     * any time delays associated with it should be ignored
     */
    if(mem_alloc_f == UN_M_FLAG){
        p->load_time = 0;

        // Print the process currently execution transcript
        print_running(cpu, p);
        return;
    }

    // Calculate how many page need to be allocated in total and for this time
    long long int total_require_page = p->mem_size_req / PAGE_SIZE;
    long long int curr_require_page = total_require_page - p->num_page_in_mem;

    //If all pages a process requires are in memory already, it can be executed
    if(curr_require_page == 0){
        p->load_time = 0;
        p->page_fault = 0;
        print_mem_running(cpu, p);
        return;
    } 


    /*
     * If there are not enough empty pages to fit a process, 
     * some or all pages need to be evicted to make space for this process 
     * until they reach the execution requiremnt 
     */
     if (curr_require_page > cpu->memory->num_holes){
        if(mem_alloc_f == SP_M_FLAG){
            /**
             * For Swapping, all pages need to be evicated
             */
            evcit(mem_alloc_f, queue, cpu, curr_require_page);
        } else if (mem_alloc_f == VM_M_FLAG || mem_alloc_f == CM_M_FLAG){
            /**
             * For Virtual Memory and Customised Memory Management (which 
             * followed the same rule of virtual memory), 
             * a process can be executed if it is allocated at 
             * least 16KB of its memory requirement (i.e., 4 pages) 
             * or all memory it requires if its requirement is less than 16KB
             * If there are more empty pages available, the process should be 
             * given either all of the empty pages or enough to meet its 
             * memory requirements
             */
            if (p->num_page_in_mem < MIN_EXE_PAGE){
                
                if(cpu->memory->num_holes < MIN_EXE_PAGE){
        
                    // Find the minimum allocation required to execute a page
                    if (p->num_page_in_mem + curr_require_page > MIN_EXE_PAGE){
                        curr_require_page = MIN_EXE_PAGE - p->num_page_in_mem;
                    } 

                     if (curr_require_page > cpu->memory->num_holes){
                         /**
                         * Evicting page only occur if a process memory 
                         * requirement is larger or equal to 4 pages, and the  
                         * sum of its memory allocation in memory currently 
                         * and empty pages now available is less than 4 pages
                         */
                        evcit(mem_alloc_f, queue, cpu, curr_require_page);
                    } else {
                        /**
                         * If the sum of its memory allocation in memory 
                         * currently and empty pages now available is equal 
                         * or larger than 4 pages, give all of the empty pages 
                         * to the process.
                         */
                        curr_require_page = cpu->memory->num_holes;
                    }

                    
                } else {
                    /**
                     * If there are enough empty pages available (i.e. more than 4 pages
                     */
                    curr_require_page = cpu->memory->num_holes;
                }
            } else {
                /**
                 * If the process has enough pages in memory or there are 
                 * enough empty pages available (i.e. more than 4 pages
                 */
                curr_require_page = cpu->memory->num_holes;
            }
            
        }

    }


    
    // Allocate the memory to the process
    long long int p_page_ind = 0;
    for(long long int i = 0; i < cpu->memory->total_page; i++){
        if (p_page_ind == curr_require_page){
            break;
        }

        if(cpu->memory->is_occupy_lst[i] == INVALID){
            cpu->memory->is_occupy_lst[i] = p->p_id;
            cpu->memory->num_holes -= 1;
            p_page_ind ++;
        } 
    }

    // Get the memory address (i.e. sets of pages) for the process
    long long int *p_page_lst = (long long int *)malloc(total_require_page 
                                            * (sizeof(long long int)));
    assert(p_page_lst != NULL);
    long long int occupy = curr_require_page + p->num_page_in_mem;
    p_page_ind = 0;
    for(long long int i = 0; i < cpu->memory->total_page; i++){
        if (p_page_ind == occupy){
            break;
        }

        if (cpu->memory->is_occupy_lst[i] == p->p_id){
            p_page_lst[p_page_ind] = i;
            p_page_ind ++;
        }
    }


    // Update the pages list which used by the process
    free(p->page_list);
    p->page_list = NULL;
    p->page_list = p_page_lst;

    // Update the required loading time 
    p->load_time = curr_require_page * LOAD_TIME;
    p->num_page_in_mem += curr_require_page;
    
    // Update the page fault
    p->page_fault = total_require_page - p->num_page_in_mem;
    p->remaining_time += p->page_fault;

    // Calcatue the current memory usage
    long long int new_usage = my_ceil(100 * 
                    (curr_require_page * 1.0 / cpu->memory->total_page * 1.0));
    p->mem_usage += new_usage;
    cpu->memory->memory_use += new_usage ;

    // Print the process currently execution transcript
    print_mem_running(cpu, p);
}

/**
 * @brief  Print the process finish message 
 * 
 * @param  mem_alloc_f  a memory allocation method flag
 * @param  cpu          a CPUInfo data
 * @param  p            a ProcessInfo data
 * @param  queue        a Queue of processes
 */
void print_finished(char mem_alloc_f, CPUInfo *cpu, 
                    ProcessInfo *p, Queue *queue){

    if(mem_alloc_f != UN_M_FLAG){
        print_evicted(cpu, p->page_list, p->num_page_in_mem);
        free_page(cpu, p, NULL, p->num_page_in_mem, -1);
    }

    fprintf(stdout, "%lld, FINISHED, id=%lld, proc-remaining=%lld\n",
        cpu->curr_time, p->p_id, get_queue_size(queue));
    
}


// ============================================================================
// == | Auxillary Functions 
// ============================================================================
/**
 * @brief  Evict number of pages from the processes until reach the requirement
 * 
 * @param  mem_alloc_f  a memory allocation method flag
 * @param  queue        a Queue of processes
 * @param  cpu          a CPUInfo data
 * @param  curr_require_page    total number of page need to be evicted
 */
void evcit(char mem_alloc_f, Queue *queue, CPUInfo *cpu, 
        long long int curr_require_page){
    
    
    Queue *resort_queue = new_queue();

    // Default page replacement method is least-recently-executed 
    char *prio_flag = PRIO_LAST_EXE_T;
    if (mem_alloc_f == CM_M_FLAG){
        /**
         * For Customerised Memory Mangement, evicting the pages of a process
         * which has shortest remaining time 
         */
        prio_flag = PRIO_MAX_PAGE;
    }

    /**
     * Sort the processes based on the page replacement in ascending order
     * (least-recently-executed or shortest-remaining-time)
     */
    for (long long int i = 0; i < get_queue_size(queue); i++){
 
        enqueue(resort_queue, get_queue_point(queue, i), prio_flag);
    }

    // A list to record evict pages
    long long int *evict_list = (long long int *)malloc(sizeof(long long int) * 
                                            cpu->memory->total_page);
    long long int evict_ind = 0;

    // Evict number of pages from the processes until reach the requirement
    while(curr_require_page > cpu->memory->num_holes){
         
        ProcessInfo *evict_p = dequeue(resort_queue);
        
        if(evict_p->page_list != NULL){
            /**
             * If a process has memory pages, a certain number of pages need to
             * be required, based on how many pages it has and how many required
             */

            // Calculate how many pages need be required for this process
            long long int num_free = evict_p->num_page_in_mem;
            if(mem_alloc_f == VM_M_FLAG || mem_alloc_f == CM_M_FLAG){
                if(num_free >= curr_require_page - cpu->memory->num_holes){
                    num_free = curr_require_page - cpu->memory->num_holes;
                }
            }

            if (num_free > 0){
                // evict pages
                evict_ind = free_page(cpu, evict_p, evict_list, 
                                    num_free, evict_ind);
            }
           
        }
        
    }

    // Sort the total evict pages in ascending order
    qsort(evict_list, evict_ind, sizeof(long long int), long_int_comparator);
    
    // Print the evicted message
    print_evicted(cpu, evict_list, evict_ind);


    // Free the memory allocated for unused list and queue
    free(evict_list);
    while(get_queue_size(resort_queue) > 0){
        dequeue(resort_queue);
    }
    free_queue(resort_queue);
    resort_queue = NULL;
}


/**
 * @brief  Compare two long long integer values, a and b
 * 
 * @param  a        a long long int value
 * @param  b        a long long int value
 * @return int      0 if they are equal
 *                  1 if a > b
 *                  -1 if a < b
 */
int long_int_comparator(const void * a, const void *b)
{
    const long long int ia = *(const long long int *)a;
    const long long int ib = *(const long long int *)b;

	if (ia == ib)
		return 0;
	if (ia > ib)
		return 1;
	return -1;
}

/**
 * @brief  Free the pages in the memory that previously occupied by a process
 * 
 * @param  cpu              a CPUInfo data
 * @param  p                a ProcessInfo data
 * @param  page_list        a page list record which pages be freed
 * @param  num_free         number of pages that needed free
 * @param  start            index of the page list before new free be made
 * @return long long int    index of the free page list after new free be made
 */
long long int free_page(CPUInfo *cpu, ProcessInfo *p, long long int *page_list, 
                    long long int num_free, long long int start){
    
    // Free the pages from the memory and record to free page list if necessary
    for(long long int i = 0; i < num_free; i++){
        long long int ind = p->page_list[i];
        cpu->memory->is_occupy_lst[ind] = INVALID;

        // Record the free pages
        if (page_list != NULL){
            page_list[start] = ind;
            start ++;
        }
    }

    // Update the number of holes in memory
    cpu->memory->num_holes += num_free;

    // Update the page list for a process occupied in memory and number of it
    if (num_free == p->num_page_in_mem){
        free(p->page_list);
        p->page_list = NULL;
    } else {
        p->page_list = remove_n_element(p->page_list, num_free, 
                                        p->num_page_in_mem);
    }
    p->num_page_in_mem -= num_free;

    // Update the memory usage of a process after some pages be freed
    long long int free_usage = my_ceil(100 * 
                            (num_free * 1.0 / cpu->memory->total_page * 1.0));
    p->mem_usage -= free_usage;
    cpu->memory->memory_use -= free_usage;


    // Return the current index of free page list 
    return start;
}

/**
 * @brief  Remove N element for a long long int list
 * 
 * @param  page_list        a long long int list
 * @param  n                number of elements (N) need be removed
 * @param  p_size           total number of elements of the long long int list
 * @return long long int*   the new list after remove N elements
 */
long long int *remove_n_element(long long int *page_list, 
                                long long int n, long long int p_size){

    // Remove N element for a long long int list
    long long int remain = p_size - n;
    long long int *new_p_list = (long long int *)malloc(remain 
                                                    * (sizeof(long long int)));
    for(long long int i = 0; i < remain; i++){
        new_p_list[i] = page_list[n + i];
    }

    free(page_list);
    page_list = NULL;

    // Return the new list after remove N elements
    return new_p_list;
}


/**
 * @brief  Print a process currently execution message for unlimited memory size
 * 
 * @param  cpu  a CPUInfo data
 * @param  p    a ProcessInfo data
 */
void print_running(CPUInfo *cpu, ProcessInfo *p){
    fprintf(stdout, "%lld, RUNNING, id=%lld, remaining-time=%lld\n",
            cpu->curr_time, p->p_id, p->remaining_time);
}

/**
 * @brief  Print a process currently execution message for limited memory size 
 * 
 * @param  cpu  a CPUInfo data 
 * @param  p    a ProcessInfo data
 */
void print_mem_running(CPUInfo *cpu, ProcessInfo *p){
   fprintf(stdout, "%lld, RUNNING, id=%lld, remaining-time=%lld, "
                    "load-time=%lld, mem-usage=%d%%, mem-addresses=[",
                    cpu->curr_time, p->p_id, p->remaining_time, 
                    p->load_time, cpu->memory->memory_use);

    for (long long int i = 0; i < p->num_page_in_mem; i++){
        fprintf(stdout, "%lld", p->page_list[i]);
        if (i != p->num_page_in_mem - 1){
            fprintf(stdout, ",");
        }
    }
    fprintf(stdout, "]\n");
}


/**
 * @brief  Print the evicted message
 * 
 * @param  cpu          a CPUInfo data
 * @param  page_list    a evict page list
 * @param  p_size       the size of the pages be evicted
 */ 
void print_evicted(CPUInfo *cpu, long long int *page_list, long long int p_size){
    fprintf(stdout, "%lld, EVICTED, mem-addresses=[", cpu->curr_time);

    for (long long int i = 0; i < p_size; i++){
        fprintf(stdout, "%lld", page_list[i]);
        if (i != p_size - 1){
            fprintf(stdout, ",");
        }
    }
    fprintf(stdout, "]\n");
}



