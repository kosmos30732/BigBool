#ifndef BB_H
#define BB_H

#include<stdio.h>
#include<stdlib.h>
#include<inttypes.h>

#define legth_of_parts 20

#define ERR_OK 0
#define ERR_SMALL_SIZE -1
#define ERR_BIG_SHIFT 1

//bb_errno is set by library functions when error occurs.
extern int bb_errno;

struct BigBool
{
    uint8_t parts[legth_of_parts];
    int last_bit;
    int last_part;
};
 
typedef struct BigBool bb;

//shifts
int left_shift (bb* vector, int size_of_shift); //makes a left shift by size_of_shift and return zero in case of successful execution
int rigth_shift (bb* vector, int size_of_shift); //makes a rigth shift by size_of_shift and return zero in case of successful execution
int cycle_left_shift (bb* vector, int size_of_shift); //makes a cycle left shift by size_of_shift and return zero in case of successful execution
int cycle_rigth_shift (bb* vector, int size_of_shift); //makes a cycle rigth shift by size_of_shift and return zero in case of successful execution

//transformations
bb* bb_from_string (char* string); //makes a big bool vector from a string and return pointer to a big bool vector
char* bb_to_string (bb* vector); //makes a string from a big bool vector and return pointer to a string

//logical operations
bb* bb_disjunction (bb* vector1, bb* vector2); //makes a disjunction of two big bool vectors and return pointer to a big bool vector
bb* bb_conjunction (bb* vector1, bb* vector2); //makes a conjunction of two big bool vectors and return pointer to a big bool vector
bb* bb_xor (bb* vector1, bb* vector2); //makes a xor of two big bool vectors and return pointer to a big bool vector
bb* bb_inversion (bb* vector1); //makes a inversion of big bool vector and return pointer to a big bool vector

#endif // BB_H