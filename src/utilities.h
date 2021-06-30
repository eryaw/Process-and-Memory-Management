/**
 * @file      utilities.h
 * @author    Erya Wen (eryaw@student.unimelb.edu.au)
 * @brief     Implementation of Utilities module. It includes
 *              1. Constants used for whole program
 *              2. self defined ceil function
 *
 * @copyright created for COMP30023 Computer System 2020
 *
 */

#ifndef UTILITIES_H
#define UTILITIES_H


// ============================================================================
// == | Constant Definitions 
// ============================================================================
#define SUCCESS                 0
#define INITIAL                 0
#define INVALID                 -1
#define KB_PAGES                4
#define NULL_CHAR               '\0'
#define FF_A_FLAG               'F'
#define RR_A_FLAG               'R'
#define CS_A_FLAG               'S'
#define UN_M_FLAG               'U'
#define SP_M_FLAG               'P'
#define VM_M_FLAG               'V'
#define CM_M_FLAG               'M'


// ============================================================================
// == | Module Functions
// ============================================================================
// Maps a number to the least integer greater than or equal to it
long long int my_ceil(long double num);

#endif