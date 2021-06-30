/**
 * @file      utilities.c
 * @author    Erya Wen (eryaw@student.unimelb.edu.au)
 * @brief     Utilities module. It includes
 *              1. Constants used for whole program
 *              2. self defined ceil function
 *
 * @copyright created for COMP30023 Computer System 2020
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>


// ============================================================================
// == | Module Functions
// ============================================================================
/**
 * @brief   Maps a number to the least integer greater than or equal to it
 * 
 * @param   num             a long double number
 * @return  long long int   a long long interger the double number rounded up to 
 */
long long int my_ceil(long double num) {
    long long int ld_num = (long long int)num;
    if (num == (long double)ld_num) {
        return ld_num;
    }
    return ld_num + 1;
}


