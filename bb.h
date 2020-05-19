#ifndef BB_H
#define BB_H

#include<stdio.h>
#include<stdlib.h>
#include<inttypes.h>

#define LENGTH_OF_PARTS 20

#define ERR_OK 0
#define ERR_SMALL_SHIFT -1
#define ERR_BIG_SHIFT 1
#define ERR_MEM_NOT_ALLOC 4
#define ERR_WRONG_CHARS 2
#define ERR_NULL_INPUT 3

//bb_errno is set by library functions when error occurs.
extern int bb_errno;

struct BigBool
{
    uint8_t parts[LENGTH_OF_PARTS];
    int last_bit;
    int last_part;
};

typedef struct BigBool bb;

/* -------------------------------------------------------------------------
 * Shifts
 * ------------------------------------------------------------------------- */

 /*
  * Funtion: left_shift
  * -------------------
  * Do left shift of bool vector
 *
 * The function allocates memory to a heap, so you need to clean it up
 *
  *        vector: pointer to a vector
  * size_of_shift: the number by which the left shift will be made
  *
  * returns: a pointer to a vector in case of success
  *          NULL in case of an error
  *
  * if an error occurs the bb_errno variable is set to:
  *     ERR_MEM_NOT_ALLOC — memory allocation error
  *     ERR_NULL_INPUT    — zero incoming data
  *     ERR_BIG_SHIFT     — too big a shift, going over the border
  *     ERR_SMALL_SHIFT   — shift less than 1
  */
bb* left_shift (bb* vector, int size_of_shift);

/*
 * Funtion: rigth_shift
 * --------------------
 * Do rigth shift of bool vector
 *
 * The function allocates memory to a heap, so you need to clean it up
 *
 *        vector: pointer to a vector
 * size_of_shift: the number by which the rigth shift will be made
 *
 * returns: a pointer to a vector in case of success
 *          NULL in case of an error
 *
 * if an error occurs the bb_errno variable is set to:
 *     ERR_MEM_NOT_ALLOC — memory allocation error
 *     ERR_NULL_INPUT    — zero incoming data
 *     ERR_BIG_SHIFT     — too big a shift, going over the border
 *     ERR_SMALL_SHIFT   — shift less than 1
 */
bb* rigth_shift (bb* vector, int size_of_shift);

/*
 * Funtion: cycle_left_shift
 * -------------------------
 * Do cyclic left shift of bool vector
 *
 * The function allocates memory to a heap, so you need to clean it up
 *
 *        vector: pointer to a vector
 * size_of_shift: the number by which the cyclic left shift will be made
 *
 * returns: a pointer to a vector in case of success
 *          NULL in case of an error
 *
 * if an error occurs the bb_errno variable is set to:
 *     ERR_MEM_NOT_ALLOC — memory allocation error
 *     ERR_NULL_INPUT    — zero incoming data
 *     ERR_BIG_SHIFT     — too big a shift, going over the border
 *     ERR_SMALL_SHIFT   — shift less than 1
 */
bb* cycle_left_shift (bb* vector, int size_of_shift);

/*
 * Funtion: cycle_rigth_shift
 * --------------------------
 * Do cyclic rigth shift of bool vector
 *
 * The function allocates memory to a heap, so you need to clean it up
 *
 *        vector: pointer to a vector
 * size_of_shift: the number by which the cyclic rigth shift will be made
 *
 * returns: a pointer to a vector in case of success
 *          NULL in case of an error
 *
 * if an error occurs the bb_errno variable is set to:
 *     ERR_MEM_NOT_ALLOC — memory allocation error
 *     ERR_NULL_INPUT    — zero incoming data
 *     ERR_BIG_SHIFT     — too big a shift, going over the border
 *     ERR_SMALL_SHIFT   — shift less than 1
 */
bb* cycle_rigth_shift (bb* vector, int size_of_shift);

/* -------------------------------------------------------------------------
 * Transformations
 * ------------------------------------------------------------------------- */

 /*
  * Funtion: bb_from_string
  * -----------------------
  * Makes a boolean vector from a string containing 0 and 1
  *
  * The function allocates memory to a heap, so you need to clean it up
  *
  * string: pointer to a string
  *
  * returns: a pointer to a vector in case of success
  *          NULL in case of an error
  *
  * if an error occurs the bb_errno variable is set to:
  *     ERR_MEM_NOT_ALLOC — memory allocation error
  *     ERR_NULL_INPUT    — zero incoming data
  *     ERR_WRONG_CHARS   — extraneous characters were encountered in the string
  */
bb* bb_from_string (char* string);

/*
 * Funtion: bb_from_string
 * -----------------------
 * Makes a string of 1 and 0 from a Boolean vector
 *
 * The function allocates memory to a heap, so you need to clean it up
 *
 * vector: pointer to a vector
 *
 * returns: pointer to a string in case of success
 *          NULL in case of an error
 *
 * if an error occurs the bb_errno variable is set to:
 *     ERR_MEM_NOT_ALLOC — memory allocation error
 *     ERR_NULL_INPUT    — zero incoming data
 */
char* bb_to_string (bb* vector);

/*
 * Funtion: bb_from_uint64_t
 * -------------------------
 * Makes a Boolean vector from uint64_t to the first significant bit
 *
 * The function allocates memory to a heap, so you need to clean it up
 *
 * num: the number of type uint64_t
 *
 * returns: a pointer to a vector in case of success
 *          NULL in case of an error
 *
 * if an error occurs the bb_errno variable is set to:
 *     ERR_MEM_NOT_ALLOC — memory allocation error
 */
bb* bb_from_uint64_t (uint64_t num);

/* -------------------------------------------------------------------------
 * Logical operations
 * ------------------------------------------------------------------------- */

 /*
  * Funtion: bb_disjunction
  * -----------------------
  * Makes a disjunction of two boolean vectors
  *
  * The function allocates memory to a heap, so you need to clean it up
  *
  * vector1: pointer to a first vector
  * vector2: pointer to a second vector
  *
  * returns: a pointer to a vector in case of success
  *          NULL in case of an error
  *
  * if an error occurs the bb_errno variable is set to:
  *     ERR_MEM_NOT_ALLOC — memory allocation error
  *     ERR_NULL_INPUT    — zero incoming data
  */
bb* bb_disjunction (bb* vector1, bb* vector2);

/*
 * Funtion: bb_conjunction
 * -----------------------
 * Makes a conjunction of two boolean vectors
 *
 * The function allocates memory to a heap, so you need to clean it up
 *
 * vector1: pointer to a first vector
 * vector2: pointer to a second vector
 *
 * returns: a pointer to a vector in case of success
 *          NULL in case of an error
 *
 * if an error occurs the bb_errno variable is set to:
 *     ERR_MEM_NOT_ALLOC — memory allocation error
 *     ERR_NULL_INPUT    — zero incoming data
 */
bb* bb_conjunction (bb* vector1, bb* vector2);

/*
 * Funtion: bb_xor
 * ---------------
 * Makes a xor of two boolean vectors
 *
 * The function allocates memory to a heap, so you need to clean it up
 *
 * vector1: pointer to a first vector
 * vector2: pointer to a second vector
 *
 * returns: a pointer to a vector in case of success
 *          NULL in case of an error
 *
 * if an error occurs the bb_errno variable is set to:
 *     ERR_MEM_NOT_ALLOC — memory allocation error
 *     ERR_NULL_INPUT    — zero incoming data
 */
bb* bb_xor (bb* vector1, bb* vector2);

/*
 * Funtion: bb_inversion
 * ---------------------
 * Makes a inversion of bool vector
 *
 * The function allocates memory to a heap, so you need to clean it up
 *
 * vector1: pointer to a vector
 *
 * returns: a pointer to a vector in case of success
 *          NULL in case of an error
 *
 * if an error occurs the bb_errno variable is set to:
 *     ERR_MEM_NOT_ALLOC — memory allocation error
 *     ERR_NULL_INPUT    — zero incoming data
 */
bb* bb_inversion (bb* vector1);

#endif // BB_H